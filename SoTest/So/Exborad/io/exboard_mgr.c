/********************************************************************
	created:	2014/05/26
	file base:	exboard_mgr
	file ext:	c
	author:		����
	
	purpose:	�����崦��ģ��
*********************************************************************/

#include "ExBoard.h"

#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
/**************************************************************************************************************************
Function:		
Description:	
***************************************************************************************************************************/
static void ChangeUartFlg(void)
{
	unsigned char send_buf[2] = {0};
	unsigned char rec_buf[2] = {0};
	send_buf[0] = 1;
	senddata_to_security(DUANHAO_CHANGE_UARTFLG,send_buf,1,rec_buf,1);
	if(rec_buf[0] != 1)
	{
		while(1);
	}
}
#endif

/**************************************************************************************************************************
 Function:		ExboardMgr_Init	
 Description:	�����崦����ʼ��
 Return:		0����ʼ��ʧ�ܣ�1��4���ɹ������ش��ں�
***************************************************************************************************************************/
int ExboardMgr_InitEx()
{
#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
	ChangeUartFlg();
#else
	//RandInit();
#endif
	return ExBoardInit();
}

/**************************************************************************************************************************
 Function:		ExbardMgr_Update
 Description:	�����崦������
***************************************************************************************************************************/
void  ExboardMgr_UpdateEx()
{
#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
#else
	// RandUpdate();
#endif
	 ExBoardProcess();
}

/**************************************************************************************************************************
 Function:		
 Description:	
***************************************************************************************************************************/
unsigned char ExboardMgr_GetCoinInEx()
{
	return ExBoardGetCoinIn();
}

/**************************************************************************************************************************
 Function:		
 Description:	
***************************************************************************************************************************/
int  ExboardMgr_IsConnectedEx()
{
	return isconsoleconnected;
}

/**************************************************************************************************************************
 Function:		
 Description:	
***************************************************************************************************************************/
int  ExboardMgr_GetVersionEx()
{
	//unsigned char *tversion;
	int temp;
	//tversion = ExBoardGetVersion();
	//temp = (tversion[0]-0x30) + (tversion[1]-0x30)*10 + (tversion[2]-0x30)*100;
	temp =	(ExboardVison[1] - 0x30)*100 + (ExboardVison[2] - 0x30)*10 + (ExboardVison[3] - 0x30);
	return temp;
}









 