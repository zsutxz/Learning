/********************************************************************
created:	2014/04/10
file base:	sl05
file ext:	h
author:		钟征

purpose:	永久数据读写头文件
*********************************************************************/
#ifndef INCLUDED_SL05_H
#define INCLUDED_SL05_H

// 设定密码长度
#define SET_MIMA_NUM			6
#define SL05_SIZE				128
//SL05数据ID
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

	SL05_U32_FIX_COINS,         //永久投币数
	SL05_U32_FIX_RETURNS,       //永久退扭蛋数
	SL05_U32_FIX_TIMES,         //永久游戏次数
/**************************************************************/
};

//sram操作方式
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

