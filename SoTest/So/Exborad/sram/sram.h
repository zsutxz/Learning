/********************************************************************
created:	2014/02/28
file base:	cmmn_sram
file ext:	h
author:		????

purpose:	?????????SRAM???????
*********************************************************************/

#ifndef INCLUDED_SRAM_H
#define INCLUDED_SRAM_H

#define SRAM_SIZE	2000
#define MAX_BACKUP 1   //ֻ��1����3������ֵ

#define _EXPORT   _declspec(dllexport)
#define EXPORT_DLL 
//sram????ID
enum
{
	SRAM_U8_NONE,
	//Rand 
	SRAM_U8_RANRAM,
	SRAM_U32_SEED,
	//Test
	SRAM_U8_TEST_U8,
	SRAM_U16_TEST_U16,
	SRAM_U32_TEST_U32,
	SRAM_U32_SRAM_FLG,

	//Fix record
	SRAM_U32_FIX_COINS,
	SRAM_U32_FIX_TIMES,

	//Ctrl
	SRAM_U32_CTRL_COIN,
	SRAM_U32_CTRL_GIFT,

	//???
	SRAM_U32_COINOUT,           // ?????
/********************************************************/
	SRAM_ID_CNT,
};

//sram???????
enum
{
	SRAM_ADD,
	SRAM_SUB,
	SRAM_COPY,
};

//??sram????
extern unsigned int SramSafeRead(unsigned int id,int len);


//??????��??sram
extern unsigned int SramSafeWrite(unsigned int id,unsigned char op,int len,unsigned int data);

//???sram
extern void SramCheck(void);

//??sram
extern void SramSafeClear(void);

//????sram???
extern void SramSafeFlg(void);


//sram???
extern void SramDetect(void);

extern void logging();
#define assert0(exp) (void)( (exp) || \
	(logging("Assert Failed! [%s] at [%s] [line %d]\r\n", #exp, __FILE__, __LINE__), 0) )
	
#endif

