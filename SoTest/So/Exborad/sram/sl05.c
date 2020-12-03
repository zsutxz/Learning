/********************************************************************
created:	2014/03/02
file base:	cmmn_sl05
file ext:	c
author:		����

purpose:	�������ݲ����߼�
*********************************************************************/

#include "hw_sl05.h"
#include "hw_sram.h"
#include "sl05.h"
#include "stdio.h"
#include "sram.h"
#include <string.h>

#define  SL05_FLG		0x12345671
#define  MAX_SL05_ID	50


static unsigned int MaxSl05ID = 0;
static int Sl05IDLen[MAX_SL05_ID][3] = {0};
static int Sl05IDAddr[MAX_SL05_ID] = {0};
int	Sl05UsingLen;				//sramʹ�ó���
unsigned char Sl05Dirty = 0;
unsigned int Sl05ChangeSAddr = 0;
unsigned int Sl05ChangeEAddr = 0;
unsigned char TempSl05Reg[SL05_SIZE*MAX_BACKUP] = {0};

//unsigned char TempSl05Backup[SL05_SIZE*MAX_BACKUP] = {0};
//unsigned char TempSramBackup[SL05_SIZE*3] = {0};
//unsigned char TempSramFlg[SL05_SIZE*3] = {0};
//unsigned char TempSl05Flg[SL05_SIZE*3] = {0};

static void ConfirmSl05(void);

/**************************************************************************************************************************
Function:		LittleBigConvert
Description:	��С��ת��
Input:			*dataҪת��������ָ��
***************************************************************************************************************************/
static void LittleBigConvert(unsigned int *data)
{
	if(data == NULL)
		return;
	else
	{
		*data = ((*data << 24)&0xff000000)|((*data << 8)&0x00ff0000) |((*data >> 8)&0x0000ff00) | ((*data >> 24)&0x000000ff);
	}
}

/**************************************************************************************************************************
Function:		InitSl05TempReg
Description:	��ʼ��sram����ʱ�洢dram������
***************************************************************************************************************************/
static void InitSl05TempReg(void)
{
	int count = 0;
	//int data = 0;
	for (count = 0; count < Sl05UsingLen; count++)
	{
		TempSl05Reg[count] = read_hw_sl05(SRAM_SIZE*MAX_BACKUP +count);
	}
}

/**************************************************************************************************************************
Function:		ConfirmU8Value
Description:	��������ֵ������������ͬ�򸲸�ʣ�µ�һ��
Return:		1����������������ͬ��0���������ݻ���ͬ��������������
***************************************************************************************************************************/
static int ConfirmU8Value(unsigned char *p,unsigned char  *p1,unsigned char  *p2)
{
	//����������ȣ�����ñ���
	if( (*p != *p1) && (*p != *p2)
		&& (*p1 != *p2) )
	{
		*p = *p1 = *p2 = 0;
		return 0;
	}

	//������ֵ��ȣ��򽫲���ȵ��Ǹ�ֵ��Ϊ��ȵ�ֵ
	if( (*p != *p1) || (*p != *p2)
		|| (*p1 != *p2) )
	{
		if( *p == *p1 )
		{
			*p2 = *p;
		}
		else
		{
			if( *p == *p2 )
			{
				*p1 = *p;
			}
			else
			{
				*p = *p1;
			}
		}
		return 1;
	}

	return 2;
}


/**************************************************************************************************************************
Function:		ConfirmSl05
Description:	����ʱ�����dram�����sram���ݽ���У�飬һ����һ�����ݳ����������ݻ��಻ͬ����������sram����
***************************************************************************************************************************/
static void ConfirmSl05(void)
{
	int count = 0;
	if (MAX_BACKUP == 1)
		return;
	for (count = 0; count < Sl05UsingLen; count++)
	{
		if (!ConfirmU8Value(&TempSl05Reg[count], &TempSl05Reg[SL05_SIZE + count], &TempSl05Reg[SL05_SIZE + count]))
		{
			logging("Confirm Sram Value Err At %d!\n", count);
			SL05SafeClear();
			break;
		}
		write_hw_sl05_group(TempSl05Reg, SL05_SIZE, SRAM_SIZE * MAX_BACKUP + 0);
		write_hw_sl05_group(TempSl05Reg + SL05_SIZE, SL05_SIZE, SRAM_SIZE * MAX_BACKUP + SL05_SIZE);
		write_hw_sl05_group(TempSl05Reg + 2 * SL05_SIZE, SL05_SIZE, SRAM_SIZE * MAX_BACKUP + 2 * SL05_SIZE);
	}
}

/**************************************************************************************************************************
Function:		InitSl05IdAddress
Description:	��ʼ��SL05�����ݵ�ַ
***************************************************************************************************************************/
int Sl05InitEx(int max_id,int length[][3])
{
	unsigned int i = 0,j = 0;
	if(max_id > MAX_SL05_ID)
		return 0;
	MaxSl05ID = max_id;
	for (i = 0; i < MaxSl05ID; i++)
	{
		for (j = 0; j < 3; j++)
		{
			Sl05IDLen[i][j] = length[i][j];
		}
	}
	//logging("sl05 init!\n");
	logging("sl05 init!\n");
#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
#else
	if(!hw_sl05_init())
		return 0;
#endif
	Sl05IDAddr[0]=0;
	for (i=1;i<MaxSl05ID;i++)
		Sl05IDAddr[i]=Sl05IDAddr[i-1]+Sl05IDLen[i-1][0]*Sl05IDLen[i-1][1];

	Sl05UsingLen = Sl05IDAddr[i-1]+Sl05IDLen[i-1][0]*Sl05IDLen[i-1][1];
	assert0(Sl05UsingLen<SL05_SIZE);
	InitSl05TempReg();
	ConfirmSl05();
	return 1;
}

/**************************************************************************************************************************
Function:		Sl05TempRead
Description:	��sram����ʱ�洢dram�ж�������
***************************************************************************************************************************/
static unsigned int Sl05TempRead(unsigned int id,int len)
{
	unsigned char t_value[4] = {0};
	unsigned int offset = 0;
	int count = 0;
	unsigned int temp = 0;
	assert0(len < Sl05IDLen[id][1]);
	offset = Sl05IDLen[id][0]*len;
	memcpy(t_value,TempSl05Reg+Sl05IDAddr[id]+offset,Sl05IDLen[id][0]);
	temp = 0;
	for (count = 0; count < Sl05IDLen[id][0]; count++)
	{
		temp = temp << 8;
		temp += t_value[Sl05IDLen[id][0]-1-count];
	}
	return temp;
}

/**************************************************************************************************************************
Function:		SramTempWrite
Description:	��len���ȵ��ֽڱ��浽sram��ʱ�洢��dram��
Input:			add�����ַ��buff�������ݣ�len���泤��
***************************************************************************************************************************/
static void Sl05TempWrite(unsigned int addr,unsigned int data,unsigned int len)
{
#ifndef WIN32
	LittleBigConvert(&data);
#endif
	memcpy(TempSl05Reg+addr,&data,len);
}

/**************************************************************************************************************************
Function:		SL05SafeReadInt
Description:	������������
Return:			����ֵ
***************************************************************************************************************************/
unsigned int Sl05SafeRead(int id,int len)
{
	return Sl05TempRead(id,len);
}

/**************************************************************************************************************************
 Function:		Sl05SafeReadEx
 Description:	
***************************************************************************************************************************/
unsigned int Sl05SafeReadEx(int id,int len)
{
	return Sl05SafeRead(id,len);
}

/**************************************************************************************************************************
 Function:		Sl05SafeChange
 Description:	
***************************************************************************************************************************/
void Sl05SafeChange()
{
	unsigned int count = 0;
	if(Sl05Dirty == 1)
	{
		//for (count = 0; count < SramChangeEAddr-SramChangeSAddr; count++)
		//{
		//	write_hw_sram(TempSramReg[SramChangeSAddr+count],(SramChangeSAddr+count));
		//	write_hw_sram(TempSramReg[SRAM_SIZE+SramChangeSAddr+count],(SRAM_SIZE+SramChangeSAddr+count));
		//	write_hw_sram(TempSramReg[2*SRAM_SIZE+SramChangeSAddr+count],(2*SRAM_SIZE+SramChangeSAddr+count));
		//}
		for (count = 0; count < MAX_BACKUP; count++)
		{
			write_hw_sl05_group(TempSl05Reg + SL05_SIZE * count + Sl05ChangeSAddr, Sl05ChangeEAddr - Sl05ChangeSAddr, (SRAM_SIZE*MAX_BACKUP + SL05_SIZE*count + Sl05ChangeSAddr));
		}

		Sl05Dirty = 0;
		Sl05ChangeEAddr = 0;
		Sl05ChangeSAddr = Sl05UsingLen;
	}
}

/**************************************************************************************************************************
Function:		Sl05SafeChangeEx
Description:	����ı��˵�sl05����,ÿ֡����
***************************************************************************************************************************/
void  Sl05SafeChangeEx()
{
	Sl05SafeChange();
}

/**************************************************************************************************************************
Function:		Sl05SafeWriteInt
Description:	д��������
Input:			idҪ�������ݶ�Ӧ��id��op������ʽ��len�����������������е�λ�ã�data��������
Return:			0д��ʧ�ܣ�1д��ɹ�
***************************************************************************************************************************/
unsigned int Sl05SafeWrite(unsigned int id,unsigned char op,int len,unsigned int data)
{
	unsigned int t_valua = 0;
	unsigned int offset = 0;
	int count = 0;
	assert0(id < MaxSl05ID);
	assert0(len < Sl05IDLen[id][1]);

	if(op == SL05_COPY)
		t_valua = data;
	else if(op == SL05_ADD)
	{
		t_valua = Sl05SafeRead(id,len);
		t_valua += data;
	}
	else if(op == SL05_SUB)
	{
		t_valua = Sl05SafeRead(id,len);
		assert0(t_valua >= data);
		if(t_valua >= data)
			t_valua -= data;
	}
	offset = len*Sl05IDLen[id][0];
	Sl05Dirty = 1;
	if(Sl05IDAddr[id]+offset < Sl05ChangeSAddr)
	{
		Sl05ChangeSAddr = Sl05IDAddr[id]+offset;
		if(Sl05ChangeSAddr%4 != 0)
		{
			do
			{
				Sl05ChangeSAddr--;
			} while (Sl05ChangeSAddr%4 != 0);
		}
	}
	if(Sl05IDAddr[id]+offset+Sl05IDLen[id][0] > Sl05ChangeEAddr)
		Sl05ChangeEAddr = Sl05IDAddr[id]+offset+Sl05IDLen[id][0];
	for (count = 0; count < MAX_BACKUP; count++)
	{
		Sl05TempWrite(count * SL05_SIZE + Sl05IDAddr[id] + offset, t_valua, Sl05IDLen[id][0]);
	}
	if(Sl05IDLen[id][2] == 1)
		Sl05SafeChangeEx();
	//write_hw_sl05(t_valua,Sl05IDLen[id][0],Sl05IDAddr[id]+offset);
	return 1;
}

/**************************************************************************************************************************
 Function:		Sl05SafeWriteEx
 Description:	
***************************************************************************************************************************/
unsigned int Sl05SafeWriteEx(unsigned int id,unsigned char op,int len,unsigned int data)
{
	return Sl05SafeWrite(id,op,len,data);
}

/**************************************************************************************************************************
Function:		SL05SafeClear
Description:	��SL05
***************************************************************************************************************************/
void SL05SafeClear(void)
{
	unsigned char count = 0, count1 = 0;
	for (count = 0;count < MaxSl05ID; count++)
	{
		if(count != SL05_U32_PRODUCT_ID && count != SL05_U32_DEVICE_ID && count != SL05_U32_DEALER_ID && count != SL05_U32_MACHINE_ID && count != SL05_U8_GVERSION && count != SL05_U32_SL05_FLG)
		{
			for (count1 = 0; count1 < Sl05IDLen[count][1]; count1++)
			{
				Sl05SafeWrite(count,SL05_COPY,count1,0);
			}
		}
	}
	Sl05SafeChangeEx();
	InitSl05TempReg();
	for (count = 0;count < MaxSl05ID; count++)
	{
		if(count != SL05_U32_PRODUCT_ID && count != SL05_U32_DEVICE_ID && count != SL05_U32_DEALER_ID && count != SL05_U32_MACHINE_ID && count != SL05_U8_GVERSION && count != SL05_U32_SL05_FLG)
		{
			for (count1 = 0; count1 < Sl05IDLen[count][1]; count1++)
			{
				if(Sl05SafeRead(count,count1) != 0)
					logging("Sl05 clear Err at %d!Please Restart!\n",count);
			}
		}
	}
}

/**************************************************************************************************************************
 Function:		
 Description:	
***************************************************************************************************************************/
void SL05SafeClearEx()
{
	SL05SafeClear();
}



/**************************************************************************************************************************
Function:		CheckSl05
Description:	���sl05�ı�־
***************************************************************************************************************************/
void Sl05Check(void)
{
	unsigned char count = 0, count1 = 0;
	if(Sl05SafeRead(SL05_U32_SL05_FLG,0) != SL05_FLG)
	{
		logging("sl05 flg is err,maybe sl05 is new,clear sl05!\n");
		for (count = 0;count < MaxSl05ID; count++)
		{
			for (count1 = 0; count1 < Sl05IDLen[count][1]; count1++)
			{
				Sl05SafeWrite(count,SL05_COPY,count1,0);
			}
		}
		Sl05SafeWrite(SL05_U32_SL05_FLG,SL05_COPY,0,SL05_FLG);
		Sl05SafeChangeEx();
		InitSl05TempReg();
		for (count = 0;count < MaxSl05ID; count++)
		{
			if(count != SL05_U32_SL05_FLG)
			{
				for (count1 = 0; count1 < Sl05IDLen[count][1]; count1++)
				{
					if(Sl05SafeRead(count,count1) != 0)
						logging("Sl05 check and clear Err at %d!Please Restart!\n",count);
				}
			}
		}
	}
	else
	{
		logging("sl05 flg is right!\n");
	}
}

/**************************************************************************************************************************
 Function:		Sl05CheckEx
 Description:	dll export ��
***************************************************************************************************************************/
void Sl05CheckEx()
{
	Sl05Check();
}

/**************************************************************************************************************************
Function:		Sl05CheckVersionEx
Description:	���汾��
Input:			version���汾�ţ�flg��0�汾����ʱ�����㣬ֻ�Ǳ���汾�ŵ�sl05��1�汾����ʱ���㣬ͬʱ����汾�ŵ�sl05
Return:			1�汾�и��£�0�޸���
***************************************************************************************************************************/
int Sl05CheckProductVersionEx(unsigned int product,char version[],int flg)
{
	int count = 0;
	int tflg = 0;
	unsigned int tsram_fix_coins = SramSafeRead(SRAM_U32_FIX_COINS,0);
	unsigned int tsram_fix_times = SramSafeRead(SRAM_U32_FIX_TIMES,0);	
	for (count = 0; count < 4; count++)
	{
		if(version[count] != Sl05SafeRead(SL05_U8_GVERSION,count))
		{
			tflg = 1;
			logging("version is error,now clear sram\n");
			break;
		}
	}
	if(tflg == 0)
	{
		logging("version is right\n");
		if(product != Sl05SafeRead(SL05_U32_PRODUCT_ID,0))
		{
			tflg = 1;
			logging("product id is error,now clear sram\n");
		}
		else
			logging("product id is right\n");
	}
	if(tflg == 1)
	{
		if(flg != 0)
		{

			SramSafeClear();
			SramSafeFlg();
			SramSafeWrite(SRAM_U32_FIX_COINS,SRAM_COPY,0,tsram_fix_coins);
			SramSafeWrite(SRAM_U32_FIX_TIMES,SRAM_COPY,0,tsram_fix_times);
		}
		for (count = 0; count < 4; count++)
		{
			Sl05SafeWrite(SL05_U8_GVERSION,SL05_COPY,count,version[count]);
		}
		Sl05SafeWrite(SL05_U32_PRODUCT_ID,SL05_COPY,0,product);
		return 1;
	}
	return 0;
}

/**************************************************************************************************************************
Function:		TestSl05
Description:	����sl05
***************************************************************************************************************************/
void Sl05TestEx()
{
#ifdef _DEBUG
	unsigned char count = 0;
	unsigned char U8Reg = 0xF0;
	unsigned short U16Reg = 0xFFF0;
	unsigned int U32Reg = 0xFFFFFFF0;
	logging("Start Test Sl05!\n");
	for (count = 0; count < 1; count++)
	{
		Sl05SafeWrite(SL05_U8_TEST_U8,SL05_COPY,count,U8Reg+count);
		Sl05SafeWrite(SL05_U16_TEST_U16,SL05_COPY,count,U16Reg+count);
		Sl05SafeWrite(SL05_U32_TEST_U32,SL05_COPY,count,U32Reg+count);
	}
	Sl05SafeChangeEx();
	InitSl05TempReg();
	for(count = 0; count < 1; count++)
	{
		logging("U8 reg%d = %x! U16 reg%d = %x! U32 reg%d = %x!\n",count,Sl05SafeRead(SL05_U8_TEST_U8,count),count,Sl05SafeRead(SL05_U16_TEST_U16,count),count,Sl05SafeRead(SL05_U32_TEST_U32,count));
	}
#endif
}


/**************************************************************************************************************************
 Function:		Sl05ExitEx
 Description:	Sl05�ر�
***************************************************************************************************************************/
void Sl05ExitEx()
{
#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
#else
	hw_sl05_exit();
#endif
}
