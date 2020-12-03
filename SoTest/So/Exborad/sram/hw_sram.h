/********************************************************************
	created:	2014/04/09
	file base:	hw_sram
	file ext:	h
	author:		钟征
	
	purpose:	硬件sram操作头文件
*********************************************************************/
#ifndef INCLUDED_HW_SRAM_H
#define INCLUDED_HW_SRAM_H

#define SRAM_SIZE	2000

extern int init_hw_sram(void);
extern unsigned char write_hw_sram(unsigned char data,unsigned int addr);
extern unsigned char read_hw_sram(unsigned int addr);
extern unsigned char write_hw_sram_group(unsigned char *data,unsigned int number,unsigned int addr);
extern unsigned char read_hw_sram_group(unsigned char *data,unsigned int number,unsigned int addr);
extern unsigned char clear_hw_sram(void);
extern void hw_sram_exit(void);

#endif
