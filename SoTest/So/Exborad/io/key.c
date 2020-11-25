/********************************************************************
	created:	2014/05/09
	file base:	key
	file ext:	c
	author:		����
	
	purpose:	io֮����ģ��
*********************************************************************/


#include "key.h"
#include "../sram/sram.h"
#include "ExBoard.h"

unsigned int key_buf = 0;
unsigned int key_buf2 = 0;
unsigned int key_val = 0;
unsigned int key_val2 = 0;
unsigned char key_cnt[GKEY_MAX] = {0};

/**************************************************************************************************************************
Function:		relate_keyboard_and_keyid
Description:	����������id��
Input:			key����ֵ��0����1��keyid����id
***************************************************************************************************************************/
static void RelateKeyAndKeyid(unsigned char key,unsigned char keyid)
{
	if(keyid < 32)
	{
		if(key)
		{
			key_buf |= 1 << keyid;
		}
		else
			key_buf &= ~(1<<keyid);
	}
	else
	{
		if(key)
		{
			key_buf2 |= 1 << (keyid-32);
		}
		else
			key_buf2 &= ~(1<<(keyid-32));
	}
}

/**************************************************************************************************************************
Function:		RelateKeyAndKeyidLowEx
Description:	����������id��key����ֵ��keyid������id
***************************************************************************************************************************/
void  RelateKeyAndKeyidLowEx(int key,int keyid)
{
	if(keyid < 32)
	{
		if((key_buf&(1<<keyid)) == 0)
		{
			if(key)
			{
				key_buf |= 1 << keyid;
			}
			else
			{
				key_buf &= ~(1<<keyid);
			}
		}
	}
	else
	{
		if((key_buf2&(1<<(keyid-32))) == 0)
		{
			if(key)
			{
				key_buf2 |= 1 << (keyid-32);
			}
			else
			{
				key_buf2 &= ~(1<<(keyid-32));
			}
		}
	}
}

/**************************************************************************************************************************
 Function:		RelateKeyboardKeyAndKeyid
 Description:	�������еİ���
***************************************************************************************************************************/
EXPORT_DLL void  RelateKeyboardKeyAndKeyidEx()
{
	RelateKeyAndKeyid(STKEY_ENTER,BACKGROUND);
	RelateKeyAndKeyid(STKEY_MIDDLE,SURE_P1);
	RelateKeyAndKeyid(STKEY_LEFT,LEFT_TURN_P1);
	RelateKeyAndKeyid(STKEY_RIGHT,RIGHT_TURN_P1);
}

/**************************************************************************************************************************
 Function:		Key_KeyScanEx
 Description:	����ɨ������
***************************************************************************************************************************/
void  Key_KeyScanEx()
{
	unsigned char count = 0;
	//logging("5key_buf = %x,",key_buf);
	//RelateKeyboardKeyAndKeyid();
	if(GKEY_MAX > 32)
	{
		for (count = 0;count < 32; count++)
		{
			if(key_buf &(1<<count))
			{
				if(++key_cnt[count] == 0x23)
					key_cnt[count] = 0x20;
				if(key_cnt[count] == 2 || (key_cnt[count]>=0x20 && !(key_cnt[count]&0x07)))
				{
					key_val |= 1<<count;
				}
				else
					key_val &= ~(1<<count);
			}
			else
			{
				key_cnt[count] = 0;
				key_val &= ~(1<<count);
			}
		}
		for (count = 0; count < GKEY_MAX-32; count++)
		{
			if(key_buf2 &(1<<count))
			{

				if(++key_cnt[count+32] == 0x23)
					key_cnt[count+32] = 0x20;
				if(key_cnt[count+32] == 2 || (key_cnt[count+32]>=0x20 && !(key_cnt[count+32]&0x07)))
				{
					key_val2 |= 1<<count;
				}
				else
					key_val2 &= ~(1<<count);
			}
			else
			{
				key_cnt[count+32] = 0;
				key_val2 &= ~(1<<count);
			}
		}
	}
	else
	{
		for (count = 0;count < GKEY_MAX; count++)
		{
			if(key_buf &(1<<count))
			{
				if(++key_cnt[count] == 0x20)
					key_cnt[count] = 0x23;
				if(key_cnt[count] == 2 || (key_cnt[count]>=0x20 && !(key_cnt[count]&0x07)))
				{
					key_val |= 1<<count;
				}
				else
					key_val &= ~(1<<count);
			}
			else
			{
				key_cnt[count] = 0;
				key_val &= ~(1<<count);
			}
		}
	}
}

/**************************************************************************************************************************
 Function:		Key_IsKeyDownEx
 Description:	�������£������ٶ�Խ��Խ��
 Input:			keyid:����id
 Return:		1����
***************************************************************************************************************************/
int  Key_IsKeyDownEx(int keyid) 
{
	if(keyid < 32)
		return key_val & 1<<keyid;
	else
		return key_val2&1<<(keyid-32);
}

/**************************************************************************************************************************
 Function:		Key_IsKeyOnEx
 Description:	�жϰ�������Ч�����������
 Input:			keyid:����id
 Return:		1����
***************************************************************************************************************************/
int  Key_IsKeyOnEx(int keyid) 
{
	if(keyid < 32)
		return key_buf & (1<<keyid);
	else
		return key_buf2 & (1<<(keyid-32));
}

/**************************************************************************************************************************
 Function:		Key_IsKeyPressEx
 Description:	������(˲��)��ֻ����һ��
 Input:			keyid:����id
 Return:		1����
***************************************************************************************************************************/
int  Key_IsKeyPressEx(int keyid) 
{
#ifdef EXTERN_IO
	return key_cnt[ keyid ] == 2;
#else
	return key_cnt[ keyid ] == 2;
#endif
}

/**************************************************************************************************************************
 Function:		Key_IsKeyLongPress
 Description:	��������(˲��)
 Input:			keyid:����id
 Return:		1����
***************************************************************************************************************************/
int  Key_IsKeyLongPressEx(int keyid) 
{
	return key_cnt[ keyid ] == 0x30;
}

/**************************************************************************************************************************
 Function:		Key_ClearKey
 Description:	���keyid��Ӧ�ļ�ֵ
 Input:			keyid:����id
***************************************************************************************************************************/
void  Key_ClearKeyEx(int keyid)
{
	if(keyid < 32)
	{
		key_buf &= (~(1<<keyid));
		key_val &= (~(1<<keyid));
	}
	else
	{
		key_buf2 &= (~(1<<(keyid-32)));
		key_val2 &= (~(1<<(keyid-32)));
	}
	key_cnt[keyid] = 0;
}

/**************************************************************************************************************************
Function:		Key_ClearAllKey	
Description:	������еİ���
***************************************************************************************************************************/
void  Key_ClearAllKeyEx()
{
	int count = 0;
	for (count = 0; count < GKEY_MAX; count++)
	{
		Key_ClearKeyEx(count);
	}
}

/**************************************************************************************************************************
 Function:		Key_PiaoOutEx
 Description:	��Ʊ
 Input:			ticketnum ��Ʊ��
***************************************************************************************************************************/
void  Key_CardOutEx(unsigned int cardnum)
{
	ExBoardSetCardOut(cardnum);
}

/**************************************************************************************************************************
 Function:		Key_GetPiaoOutEx
 Description:	�����Ʊ��
 Return:		�������ε���֮�����Ʊ��
***************************************************************************************************************************/
unsigned int  Key_GetCardOutEx()
{
	return ExBoardGetCardOut();
}

/**************************************************************************************************************************
 Function:		Key_ClearPiaoOutEx
 Description:	�����Ʊ��
***************************************************************************************************************************/
void  Key_ClearCardOutEx()
{
	ExBoardClearCardOut();
}




