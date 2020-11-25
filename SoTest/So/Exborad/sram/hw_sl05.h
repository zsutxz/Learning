#ifndef INCLUDED_HW_SL05_H
#define INCLUDED_HW_SL05_H



extern unsigned char write_hw_sl05(unsigned char data,unsigned int addr);


extern unsigned char read_hw_sl05(unsigned int addr);


extern unsigned char write_hw_sl05_group(unsigned char *data,unsigned char number,unsigned int addr);


extern unsigned char read_hw_sl05_group(unsigned char *data,unsigned char number,unsigned int addr);


extern int hw_sl05_init(void);


extern void hw_sl05_exit(void);


#endif

