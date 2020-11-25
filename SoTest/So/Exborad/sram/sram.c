/********************************************************************
created:	2014/03/02
file base:	cmmn_sram
file ext:	c
author:		����

purpose:	sram���ݲ����߼�
*********************************************************************/
// #include "defined.h"
#include "sram.h"
#include "hw_sram.h"
#include "hw_sl05.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sl05.h"
// #include "output.h"
// #include "clock.h"

// #include "dll_define.h"




#define  SRAM_FLG		0x12345678
#define  MAX_SRAM_ID	100

static unsigned int MaxSramID = 0;
static int SramIDLen[MAX_SRAM_ID][3] = {0};
//static int SramIDAddr[MaxSramID] = {0};
static int SramIDAddr[MAX_SRAM_ID] = {0};
int	SramUsingLen;				//sramʹ�ó���
unsigned char SramDirty = 0;
unsigned int SramChangeSAddr = 0;
unsigned int SramChangeEAddr = 0;
unsigned char TempSramReg[SRAM_SIZE*MAX_BACKUP] = {0};

void SramSafeClear(void);

/**************************************************************************************************************************
Function:		LittleBigConvert
Description:	��С��ת��
Input��			*data:Ҫת��������
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

static void ConfirmSram(void);

static void InitSramTempReg(void);


static void SramSafeChange(void);

/**************************************************************************************************************************
 Function:		SramCheckFixRecordEx
 Description:	������ü�¼������
***************************************************************************************************************************/
void  SramCheckFixRecordEx()
{
	unsigned int tsram_fix_coins = SramSafeRead(SRAM_U32_FIX_COINS,0);
	unsigned int tsram_fix_times = SramSafeRead(SRAM_U32_FIX_TIMES,0);
	unsigned int tsl05_fix_coins = Sl05SafeRead(SL05_U32_FIX_COINS,0);
	unsigned int tsl05_fix_times = Sl05SafeRead(SL05_U32_FIX_TIMES,0);
	logging("%d,%d,%d,%d\n",tsram_fix_coins,tsram_fix_times,tsl05_fix_coins,tsl05_fix_times);
	//logging("%d,%d,%d,%d\n",tsram_fix_coins,tsram_fix_times,tsl05_fix_coins,tsl05_fix_times);
	if(tsram_fix_coins > tsl05_fix_coins)
	{
		Sl05SafeWrite(SL05_U32_FIX_COINS,SL05_COPY,0,tsram_fix_coins);
		Sl05SafeChange();
	}
	else if(tsram_fix_coins < tsl05_fix_coins)
	{
		SramSafeWrite(SRAM_U32_FIX_COINS,SRAM_COPY,0,tsl05_fix_coins);
		SramSafeChange();
	}
	if(tsram_fix_times > tsl05_fix_times)
	{
		Sl05SafeWrite(SL05_U32_FIX_TIMES,SL05_COPY,0,tsram_fix_times);
		Sl05SafeChange();
	}
	else if(tsram_fix_times < tsl05_fix_times)
	{
		SramSafeWrite(SRAM_U32_FIX_TIMES,SRAM_COPY,0,tsl05_fix_times);
		SramSafeChange();
	}
	tsram_fix_coins = SramSafeRead(SRAM_U32_FIX_COINS,0);
	tsram_fix_times = SramSafeRead(SRAM_U32_FIX_TIMES,0);
	tsl05_fix_coins = Sl05SafeRead(SL05_U32_FIX_COINS,0);
	tsl05_fix_times = Sl05SafeRead(SL05_U32_FIX_TIMES,0);
	logging("%d,%d,%d,%d\n",tsram_fix_coins,tsram_fix_times,tsl05_fix_coins,tsl05_fix_times);
}

/**************************************************************************************************************************
Function:		SramInit
Description:	��ʼ����������sram�еĵ�ַ����ʼ��dram���汣��sram���ݵ���ʱ������У��sram����
***************************************************************************************************************************/
int SramInitEx(int max_id,int length[][3])
{
	unsigned int i = 0,j = 0;
	logging("Sram init start\n");
	if(max_id > MAX_SRAM_ID)
		return 0;
	MaxSramID = max_id;
	for (i = 0; i < MaxSramID; i++)
	{
		for (j = 0; j < 3; j++)
		{
			SramIDLen[i][j] = length[i][j];
		}
	}
#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
#else
	if(!init_hw_sram())
		return 0;
#endif
	SramIDAddr[0]=0;
	for (i=1;i<MaxSramID;i++)
		SramIDAddr[i]=SramIDAddr[i-1]+SramIDLen[i-1][0]*SramIDLen[i-1][1];

	SramUsingLen = SramIDAddr[i-1]+SramIDLen[i-1][0]*SramIDLen[i-1][1];
	assert0(SramUsingLen<SRAM_SIZE);
	SramDirty = 0;
	SramChangeEAddr = 0;
	SramChangeSAddr = SramUsingLen;
	InitSramTempReg();
	ConfirmSram();
	//SramCheckFixRecordEx();
	logging("Sram init OK\n");
	return 1;
}

/**************************************************************************************************************************
Function:		InitSramTempReg
Description:	��ʼ������sram���������dram�еı���
***************************************************************************************************************************/
static void InitSramTempReg(void)
{
	int count = 0;
	for (count = 0; count < MAX_BACKUP; count++)
	{
		read_hw_sram_group(TempSramReg + SRAM_SIZE*count, SramUsingLen, SRAM_SIZE*count);
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
	}

	return 1;
}


/**************************************************************************************************************************
Function:		ConfirmSram
Description:	����ʱ�����dram�����sram���ݽ���У�飬һ����һ�����ݳ����������ݻ��಻ͬ����������sram����
***************************************************************************************************************************/
static void ConfirmSram(void)
{
	int count = 0;
	if (MAX_BACKUP == 1)
		return;
	for (count = SramIDLen[SRAM_U8_NONE][0]*SramIDLen[SRAM_U8_NONE][1]+SramIDLen[SRAM_U8_RANRAM][0]*SramIDLen[SRAM_U8_RANRAM][1]+SramIDLen[SRAM_U32_SEED][0]*SramIDLen[SRAM_U32_SEED][1]; count < SramUsingLen; count++)
	{
		if(!ConfirmU8Value(&TempSramReg[count],&TempSramReg[SRAM_SIZE+count],&TempSramReg[2*SRAM_SIZE+count]))
		{
			logging("Confirm Sram Value Err At %d!\n",count);
			SramSafeClear();
			break;
		}
	}
	write_hw_sram_group(TempSramReg,SRAM_SIZE,0);
	write_hw_sram_group(TempSramReg+SRAM_SIZE,SRAM_SIZE,SRAM_SIZE);
	write_hw_sram_group(TempSramReg+2*SRAM_SIZE,SRAM_SIZE,2*SRAM_SIZE);
	InitSramTempReg();
}

/**************************************************************************************************************************
Function:		SramTempRead
Description:	������ʱsram���ֵ
Input:			idҪ�������ݵ�id�ţ�len�����е�λ��
Return:			����ֵ
***************************************************************************************************************************/
static unsigned int SramTempRead(unsigned int id,int len)
{
	unsigned char t_value[4] = {0};
	unsigned int offset = 0;
	unsigned char count = 0;
	unsigned int temp = 0;
	if(len >= SramIDLen[id][1])
	{
		logging("id = %d\n",id);
	}
	assert0(len < SramIDLen[id][1]);
	offset = SramIDLen[id][0]*len;
	memcpy(t_value,TempSramReg+SramIDAddr[id]+offset,SramIDLen[id][0]);
	temp = 0;
	for (count = 0; count < SramIDLen[id][0]; count++)
	{
		temp = temp << 8;
		temp += t_value[SramIDLen[id][0]-1-count];
	}
	return temp;
}

/**************************************************************************************************************************
Function:		SramTempReadBk
Description:	����sram��ʱ�洢��dram��������ݵı���
Input:			����id��bk_id����id��len�������е�λ��
***************************************************************************************************************************/
static unsigned int SramTempReadBk(unsigned int id,unsigned int bk_id,int len)
{
	unsigned char t_value[4] = {0};
	unsigned int offset = 0;
	unsigned char count = 0;
	unsigned int temp = 0;
	assert0(len < SramIDLen[id][1]);
	offset = SramIDLen[id][0]*len;
	memcpy(t_value,TempSramReg+SRAM_SIZE*bk_id+SramIDAddr[id]+offset,SramIDLen[id][0]);
	temp = 0;
	for (count = 0; count < SramIDLen[id][0]; count++)
	{
		temp = temp << 8;
		temp += t_value[SramIDLen[id][0]-1-count];
	}
	return temp;
}


/**************************************************************************************************************************
Function:		SramTempWrite
Description:	��len���ȵ��ֽڱ��浽sram��ʱ�洢��dram������
Input:			add�����ַ��buff�������ݣ�len���泤��
***************************************************************************************************************************/
static void SramTempWrite(unsigned int addr,unsigned int data,unsigned int len)
{
#ifndef WIN32
	LittleBigConvert(&data);
#endif
	memcpy(TempSramReg+addr,&data,len);
}

/**************************************************************************************************************************
Function:		SramSafeRead
Description:	��sram
Input:			idҪ�������ݵ�id�ţ�len�����е�λ��
Return:			����ֵ
***************************************************************************************************************************/
unsigned int SramSafeRead(unsigned int id,int len)
{
	unsigned int temp = 0;
	//unsigned char count = 0;
	unsigned int offset = 0;
	if(len >= SramIDLen[id][1])
	{
		logging("id = %d, len = %d, SramIDLen = %d\n",id,len,SramIDLen[id][1]);
	}
	assert0(len < SramIDLen[id][1]);
	offset = SramIDLen[id][0]*len;
	temp = SramTempRead(id,len);
	return temp;
}

/**************************************************************************************************************************
 Function:		SramSafeReadEx
 Description:	dll export��
***************************************************************************************************************************/
unsigned int SramSafeReadEx(unsigned int id,int len)
{
	return SramSafeRead(id,len);
}

/**************************************************************************************************************************
Function:		SramSafeWrite
Description:	дsram
Input:			idд��������sram�е�id�ţ�opд������len��Ҫ��������id��������λ�ã�data������Ҫ��ֵ
Return:			0д�����1д����ȷ
***************************************************************************************************************************/
unsigned int SramSafeWrite(unsigned int id,unsigned char op,int len,unsigned int data)
{
	unsigned char count = 0;
	unsigned int t_valua = 0;
	unsigned int offset = 0;
	unsigned int tdata[MAX_BACKUP] = { 0 };
	//int temp = SramIDLen[id][1];
	assert0(id < MaxSramID);
	assert0(len < SramIDLen[id][1]);
	if (MAX_BACKUP == 3)
	{
		for (count = 0; count < MAX_BACKUP; count++)
		{
			tdata[count] = SramTempReadBk(id, count, len);
		}
		if (tdata[1] != tdata[2] || tdata[0] != tdata[1])
		{
			//assert("Data Err,Please Restart!");
			return 0;
		}
	}
	if(op == SRAM_COPY)
		t_valua = data;
	else if(op == SRAM_ADD)
	{
		t_valua = SramSafeRead(id,len);
		t_valua += data;
	}
	else if(op == SRAM_SUB)
	{
		t_valua = SramSafeRead(id,len);
		assert0(t_valua >= data);
		if(t_valua >= data)
			t_valua -= data;
	}
	offset = len*SramIDLen[id][0];
	SramDirty = 1;
	if(SramIDAddr[id]+offset < SramChangeSAddr)
	{
		SramChangeSAddr = SramIDAddr[id]+offset;
		if(SramChangeSAddr%4 != 0)
		{
			do
			{
				SramChangeSAddr--;
			} while (SramChangeSAddr%4 != 0);
		}
	}
	if(SramIDAddr[id]+offset+SramIDLen[id][0] > SramChangeEAddr)
		SramChangeEAddr = SramIDAddr[id]+offset+SramIDLen[id][0];
	//SramTempWrite(SramIDAddr[id]+offset,t_valua,SramIDLen[id][0]);
	//SramTempWrite(SRAM_SIZE+SramIDAddr[id]+offset,t_valua,SramIDLen[id][0]);
	//SramTempWrite(2*SRAM_SIZE+SramIDAddr[id]+offset,t_valua,SramIDLen[id][0]);
	for (count = 0; count < MAX_BACKUP; count++)
	{
		SramTempWrite(SRAM_SIZE*count + SramIDAddr[id] + offset, t_valua, SramIDLen[id][0]);
	}
	if(SramIDLen[id][2] == 1)
	{
		SramSafeChange();
	}
	return 1;
}

/**************************************************************************************************************************
 Function:		SramSafeWriteEx
 Description:	dll export��
***************************************************************************************************************************/
unsigned int SramSafeWriteEx(unsigned int id,unsigned char op,int len,unsigned int data)
{
	return SramSafeWrite(id,op,len,data);
}

/**************************************************************************************************************************
Function:		SramSafeChange
Description:	����ı��˵�sram����
***************************************************************************************************************************/
static void SramSafeChange(void)
{
	unsigned int count = 0;
	if(SramDirty == 1)
	{
		//for (count = 0; count < SramChangeEAddr-SramChangeSAddr; count++)
		//{
		//	write_hw_sram(TempSramReg[SramChangeSAddr+count],(SramChangeSAddr+count));
		//	write_hw_sram(TempSramReg[SRAM_SIZE+SramChangeSAddr+count],(SRAM_SIZE+SramChangeSAddr+count));
		//	write_hw_sram(TempSramReg[2*SRAM_SIZE+SramChangeSAddr+count],(2*SRAM_SIZE+SramChangeSAddr+count));
		//}
		for (count = 0; count < MAX_BACKUP; count++)
		{
			write_hw_sram_group(TempSramReg + count * SRAM_SIZE + SramChangeSAddr, SramChangeEAddr - SramChangeSAddr, (count * SRAM_SIZE + SramChangeSAddr));
		}
		SramDirty = 0;
		SramChangeEAddr = 0;
		SramChangeSAddr = SramUsingLen;
	}
}

/**************************************************************************************************************************
Function:		SramSafeChangeAfrterServalSec
Description:	���뱣��һ��sram����
***************************************************************************************************************************/
void SramSafeChangeAfrterServalSecEx()
{
	static unsigned int wt = 0;
	SramSafeChange();

	// if(system_time_ms() - wt >= 100)
	// {
	// 	SramSafeChange();
	// 	wt = system_time_ms();
	// }
}

/**************************************************************************************************************************
Function:		SramSafeClear
Description:	�������õ�sram��Ԫ
***************************************************************************************************************************/
void SramSafeClear(void)
{
	unsigned short count = 0;
	unsigned char tdata[SRAM_SIZE] = {0};
	//unsigned char count1 = 0;
	//unsigned int sram_use_len = 0;
	//unsigned char temp = 0;
	//for (count = 0; count < SRAM_SIZE; count++)
	//{
	//	write_hw_sram(0,count);
	//	write_hw_sram(0,SRAM_SIZE+count);
	//	write_hw_sram(0,2*SRAM_SIZE+count);
	//}
	//write_hw_sram_group(tdata,SRAM_SIZE,count);
	//write_hw_sram_group(tdata,SRAM_SIZE,SRAM_SIZE+count);
	//write_hw_sram_group(tdata,SRAM_SIZE,2*SRAM_SIZE+count);
	for (count = 0; count < MAX_BACKUP; count++)
	{
		write_hw_sram_group(tdata, SRAM_SIZE, SRAM_SIZE * count);
	}

	InitSramTempReg();
	for (count = 0; count < SRAM_SIZE*MAX_BACKUP; count++)
	{
		if(TempSramReg[count] != 0)
			logging("Sram clear Err at %d!Please Restart!\n",count);
	}
}

/**************************************************************************************************************************
 Function:		SramSafeClearEx
 Description:	dll export ��
***************************************************************************************************************************/
void SramSafeClearEx()
{
  SramSafeClear();
}


/**************************************************************************************************************************
Function:		SramCheck
Description:	���sram
***************************************************************************************************************************/
void SramCheck(void)
{
	unsigned int flg = 0;
	flg = SramSafeRead(SRAM_U32_SRAM_FLG,0);
	if(flg != SRAM_FLG)
	{
		logging("Sram flg is err,maybe sram is new,now init sram!\n");
		SramSafeClear();
		SramSafeWrite(SRAM_U32_SRAM_FLG,SRAM_COPY,0,SRAM_FLG);
	}
}

/**************************************************************************************************************************
 Function:		SramCheckEx
 Description:	dll export ��
***************************************************************************************************************************/
void SramCheckEx()
{
	SramCheck();
}

/**************************************************************************************************************************
Function:		SramSafeFlg
Description:	����sram��־
***************************************************************************************************************************/
void SramSafeFlg(void)
{
	SramSafeWrite(SRAM_U32_SRAM_FLG,SRAM_COPY,0,SRAM_FLG);
}

/**************************************************************************************************************************
 Function:		SramSafeFlgEx
 Description:	dll export ��
***************************************************************************************************************************/
void SramSafeFlgEx()
{
	SramSafeFlg();
}

/**************************************************************************************************************************
Function:		SramTest
Description:	����sram
***************************************************************************************************************************/
void SramTestEx()
{
#ifdef _DEBUG
	unsigned char count = 0;
	unsigned char U8Reg = 0xF0;
	unsigned short U16Reg = 0xFFF0;
	unsigned int U32Reg = 0xFFFFFFF0;
	logging("Start Test Sram!\n");
	for (count = 0; count < 10; count++)
	{
		SramSafeWrite(SRAM_U8_TEST_U8,SRAM_COPY,count,U8Reg+count);
		SramSafeWrite(SRAM_U16_TEST_U16,SRAM_COPY,count,U16Reg+count);
		SramSafeWrite(SRAM_U32_TEST_U32,SRAM_COPY,count,U32Reg+count);
	}
	SramSafeChange();
	InitSramTempReg();
	//ConfirmSram();
	for(count = 0; count < 10; count++)
	{
		logging("U8 reg%d = %x! U16 reg%d = %x! U32 reg%d = %x!\n",count,SramSafeRead(SRAM_U8_TEST_U8,count),count,SramSafeRead(SRAM_U16_TEST_U16,count),count,SramSafeRead(SRAM_U32_TEST_U32,count));
	}
#endif
}

/**************************************************************************************************************************
Function:		SramDetect
Description:	sram���
***************************************************************************************************************************/
int SramDetectEx()
{
	//int count = 0;
	//for (count = 0; count < 3*SRAM_SIZE; count++)
	//{
	//	write_hw_sram(count%254+1,count);
	//}
	//for (count = 0; count < 3*SRAM_SIZE; count++)
	//{
	//	if(read_hw_sram(count) != count%254+1)
	//	{
	//		return 0;
	//	}
	//}
	//for (count = 0; count < 3*SRAM_SIZE; count++)
	//{
	//	write_hw_sram(0,count);
	//}
	unsigned char twdata[MAX_BACKUP*SRAM_SIZE] = {0};
	unsigned char trdata[MAX_BACKUP*SRAM_SIZE] = {0};
	int count = 0;
	for (count = 0; count < MAX_BACKUP*SRAM_SIZE; count++)
	{
		twdata[count] = count%254+1;
	}
	write_hw_sram_group(twdata, MAX_BACKUP*SRAM_SIZE,0);
	read_hw_sram_group(trdata, MAX_BACKUP*SRAM_SIZE,0);
	for (count = 0; count < MAX_BACKUP*SRAM_SIZE; count++)
	{
		if(trdata[count] != count%254+1)
		{
			return 0;
		}
	}
	memset(twdata,0, MAX_BACKUP*SRAM_SIZE);
	write_hw_sram_group(twdata, MAX_BACKUP*SRAM_SIZE,0);
	return 1;
}

/**************************************************************************************************************************
 Function:		SramExit
 Description:	sram�ر�
***************************************************************************************************************************/
void SramExitEx()
{
#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)
#else
	hw_sram_exit();
#endif
}

void logging()
{

}
