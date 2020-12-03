/********************************************************************
created:	2014/04/10
file base:	sl05
file ext:	h
author:		����

purpose:	�������ݶ�дͷ�ļ�
*********************************************************************/
#ifndef INCLUDED_SL05_H
#define INCLUDED_SL05_H

// �趨���볤��
#define SET_MIMA_NUM			6
#define SL05_SIZE				128
//SL05����ID
enum
{
	SL05_U8_TEST_U8,
	SL05_U16_TEST_U16,
	SL05_U32_TEST_U32,
	SL05_U32_SL05_FLG,

	SL05_U32_PRODUCT_ID,
	SL05_U32_DEVICE_ID,		// Device Unique Identifier
	SL05_U32_DEALER_ID,		// Dealer ID
	SL05_U32_MACHINE_ID,	    // Machine ID
	SL05_U8_GVERSION,			// Version
	SL05_U8_LANGUAGE,

	SL05_U32_FIX_COINS,         //����Ͷ����
	SL05_U32_FIX_RETURNS,       //������Ť����
	SL05_U32_FIX_TIMES,         //������Ϸ����
/**************************************************************/
};

//sram������ʽ
enum
{
	SL05_ADD,
	SL05_SUB,
	SL05_COPY,
};


extern unsigned int Sl05SafeRead(int id,int len);

extern unsigned int Sl05SafeWrite(unsigned int id,unsigned char op,int len,unsigned int data);

extern void SL05SafeClear(void);

extern void Sl05SafeChange();


#endif

