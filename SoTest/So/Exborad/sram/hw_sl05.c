/********************************************************************
	created:	2014/04/10
	file base:	hw_sl05
	file ext:	c
	author:		�1�7�1�7�1�7�1�7
	
	purpose:	�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�4�1�7�Մ1�7�1�8�1�7
*********************************************************************/

#include "hw_sl05.h"
#include "sl05.h"
#include "malloc.h"
#include <string.h>
#include "stdio.h"
#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#endif

#if defined(_SECURITY)|| defined(_SECURITY_MONITOR)

#define MAX_WRITE_NUM	16
#define MAX_READ_NUM	1

/**************************************************************************************************************************
Function:		LittleBigConvert
Description:	�1�7�1�7���1�7�1�7�0�8�1�7�1�7
Input:			*data�0�8�0�8�1�7�1�7�1�7�1�7�1�7�1�7
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
Function:		write_hw_sl05
Description:	�Մ1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
Input:			data:�0�8�Մ1�7�1�7HY04�1�7�ք1�7�1�7�1�7�1�7�1�5�1�7addr�1�7�1�7�0�7
return			�1�7�1�7�1�7�0�4�1�7�1�7�3�7�1�70
***************************************************************************************************************************/
unsigned char write_hw_sl05(unsigned char data,unsigned int addr)
{
//#ifdef WIN32
//	fseek(sl05_fp,addr,SEEK_SET);
//	fwrite(&data,1,1,sl05_fp);
//#else
//	lseek(sl05_fp,addr,SEEK_SET);
//	write(sl05_fp,&data,1);
//#endif
//	return 1;
	unsigned char send_buf[6] = {0};
	unsigned char rec_buf[4] = {0};
	U32ToU8(addr,send_buf);
	send_buf[4] = 1;
	send_buf[5] = data;
	senddata_to_security(DUANHAO_SL05_WRITE,send_buf,6,rec_buf,1);
	return rec_buf[0];
}

/**************************************************************************************************************************
Function:		read_SL05
Description:	�1�7�1�7HY04�1�7�1�7�1�7�1�7
Input:			addr�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�3�0�7�1�3�1�7�0�7
***************************************************************************************************************************/
unsigned char read_hw_sl05(unsigned int addr)
{
//	unsigned char read_data = 0;
//#ifdef WIN32
//	unsigned int temp;
//	fseek(sl05_fp,addr,SEEK_SET);
//	temp = ftell(sl05_fp);
//	fread(&read_data,1,1,sl05_fp);
//#else
//	lseek(sl05_fp,addr,SEEK_SET);
//	read(sl05_fp,&read_data,1);
//#endif
//	return read_data;
	unsigned char send_buf[4] = {0};
	unsigned char rec_buf[4] = {0};
	U32ToU8(addr,send_buf);
	senddata_to_security(DUANHAO_SL05_READ,send_buf,4,rec_buf,1);
	return rec_buf[0];
}

/**************************************************************************************************************************
Function:		write_hw_sl05
Description:	�Մ1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
Input:			data:�0�8�Մ1�7�1�7HY04�1�7�ք1�7�1�7�1�7�1�7�1�5�1�7addr�1�7�1�7�0�7
return			�1�7�1�7�1�7�0�4�1�7�1�7�3�7�1�70
***************************************************************************************************************************/
unsigned char write_hw_sl05_group(unsigned char *data,unsigned char number,unsigned int addr)
{
//#ifdef WIN32
//	fseek(sl05_fp,addr,SEEK_SET);
//	fwrite(data,number,1,sl05_fp);
//#else
//	lseek(sl05_fp,addr,SEEK_SET);
//	write(sl05_fp,data,number);
//#endif
//	return 1;
	unsigned char send_buf[30] = {0};
	unsigned char rec_buf[5] = {0};
	unsigned int tcnt = 0;
	logging("write start!\n");
	while(tcnt < number)
	{
		memset(send_buf,0,30);
		memset(rec_buf,0,5);
		U32ToU8(addr+tcnt,send_buf); 
		if(number - tcnt >= MAX_WRITE_NUM)
		{
			send_buf[4] = MAX_WRITE_NUM;
			memcpy(send_buf+5,data+tcnt,MAX_WRITE_NUM);
			senddata_to_security(DUANHAO_SL05_WRITE_GROUP,send_buf,MAX_WRITE_NUM+5,rec_buf,5);
			if(rec_buf[0] != MAX_WRITE_NUM)
			{
				logging("Write Sl05 Err At %d\n",addr+tcnt);
				while(1);
				return 0;
			}
			//logging("%d",U8ToU32(rec_buf+1));
			tcnt += 16;
		}
		else
		{
			memset(rec_buf,0,5);
			send_buf[4] = number-tcnt;
			memcpy(send_buf+5,data+tcnt,number-tcnt);
			senddata_to_security(DUANHAO_SL05_WRITE_GROUP,send_buf,number-tcnt+5,rec_buf,5);
			if(rec_buf[0] != number-tcnt)
			{
				logging("Write Sl05 Err At %d\n",addr+tcnt);
				while(1);
				return 0;
			}
			//logging("%d",U8ToU32(rec_buf+1));
			tcnt = number;
		}
	}
	logging("write end!\n");
	return 1;
}

/**************************************************************************************************************************
Function:		read_hw_sl05_group
Description:	�1�7�1�7HY04�1�7�1�7�1�7�1�7,�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7�1�7
Input:			addr�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�3�1�7�1�7�1�7�0�3�1�7�1�7�0�7
***************************************************************************************************************************/
unsigned char read_hw_sl05_group(unsigned char *data,unsigned char number,unsigned int addr)
{
//#ifdef WIN32
//	unsigned int temp;
//	fseek(sl05_fp,addr,SEEK_SET);
//	temp = ftell(sl05_fp);
//	fread(data,number,1,sl05_fp);
//#else
//	lseek(sl05_fp,addr,SEEK_SET);
//	read(sl05_fp,data,number);
//#endif
//	return 1;
	unsigned char send_buf[5] = {0};
	unsigned char rec_buf[30] = {0};
	unsigned int tcnt = 0;
	while(tcnt < number)
	{
		memset(rec_buf,0,30);
		U32ToU8(addr+tcnt,send_buf); 
		if(number - tcnt >= MAX_READ_NUM)
		{
			send_buf[4] = MAX_READ_NUM;
			senddata_to_security(DUANHAO_SL05_READ_GROUP,send_buf,5,rec_buf,MAX_READ_NUM+5);
			assert0(rec_buf[0] == MAX_READ_NUM);
			if(rec_buf[0] != MAX_READ_NUM)
			{
				//logging("read hw sl05 group err!\n");
				logging("read hw sl05 group err,%d!\n",rec_buf[0]);
				while(1);
				return 0;
			}
			logging("%d,",U8ToU32(rec_buf+2));
			memcpy(data+tcnt,rec_buf+1,1);
			tcnt += 1;
		}
		else
		{	
			send_buf[4] = number-tcnt;
			senddata_to_security(DUANHAO_SL05_READ_GROUP,send_buf,5,rec_buf,number-tcnt+5);
			assert0(rec_buf[0] == number-tcnt);
			if(rec_buf[0] != number-tcnt)
			{
				logging("read hw sl05 group err!\n");
				while(1);
				return 0;
			}
			logging("%d,",U8ToU32(rec_buf+2));
			memcpy(data+tcnt,rec_buf+1,number-tcnt);
			tcnt = number;
		}
	}
	logging("\n");
	return 1;
}

/**************************************************************************************************************************
Function:		hw_sl05_init
Description:	sl05�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
Retrun:			0�1�7�1�7�0�3�1�7�1�7�0�2�1�7�1�1�1�71�1�7�0�6�1�7
***************************************************************************************************************************/
int hw_sl05_init(void)
{
	unsigned char send_buf[4] = {0};
	unsigned char rec_buf[4] = {0};
	senddata_to_security(DUANHAO_SL05_INIT,send_buf,1,rec_buf,1);
	return rec_buf[0];
}

/**************************************************************************************************************************
Function:		hw_sl05_exit
Description:	sl05�1�7�1�9�1�7
***************************************************************************************************************************/
void hw_sl05_exit(void)
{
	unsigned char send_buf[4] = {0};
	unsigned char rec_buf[4] = {0};
	senddata_to_security(DUANHAO_SL05_EXIT,send_buf,1,rec_buf,1);
}

#elif WIN32
FILE *sl05_fp;
#else
int	sl05_fp;
#endif


/**************************************************************************************************************************
Function:		LittleBigConvert
Description:	�1�7�1�7���1�7�1�7�0�8�1�7�1�7
Input:			*data�0�8�0�8�1�7�1�7�1�7�1�7�1�7�1�7
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
Function:		write_hw_sl05
Description:	�Մ1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
Input:			data:�0�8�Մ1�7�1�7HY04�1�7�ք1�7�1�7�1�7�1�7�1�5�1�7addr�1�7�1�7�0�7
return			�1�7�1�7�1�7�0�4�1�7�1�7�3�7�1�70
***************************************************************************************************************************/
unsigned char write_hw_sl05(unsigned char data,unsigned int addr)
{
#ifdef WIN32
	fseek(sl05_fp,addr,SEEK_SET);
	fwrite(&data,1,1,sl05_fp);
#else
	lseek(sl05_fp,addr,SEEK_SET);
	write(sl05_fp,&data,1);
#endif
	return 1;
}

/**************************************************************************************************************************
Function:		read_SL05
Description:	�1�7�1�7HY04�1�7�1�7�1�7�1�7
Input:			addr�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�3�0�7�1�3�1�7�0�7
***************************************************************************************************************************/
unsigned char read_hw_sl05(unsigned int addr)
{
	unsigned char read_data = 0;
#ifdef WIN32
	unsigned int temp;
	fseek(sl05_fp,addr,SEEK_SET);
	temp = ftell(sl05_fp);
	fread(&read_data,1,1,sl05_fp);
#else
	lseek(sl05_fp,addr,SEEK_SET);
	read(sl05_fp,&read_data,1);
#endif
	return read_data;
}

/**************************************************************************************************************************
Function:		write_hw_sl05
Description:	�Մ1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
Input:			data:�0�8�Մ1�7�1�7HY04�1�7�ք1�7�1�7�1�7�1�7�1�5�1�7addr�1�7�1�7�0�7
return			�1�7�1�7�1�7�0�4�1�7�1�7�3�7�1�70
***************************************************************************************************************************/
unsigned char write_hw_sl05_group(unsigned char *data,unsigned char number,unsigned int addr)
{
#ifdef WIN32
	fseek(sl05_fp,addr,SEEK_SET);
	fwrite(data,number,1,sl05_fp);
#else
	lseek(sl05_fp,addr,SEEK_SET);
	write(sl05_fp,data,number);
#endif
	return 1;
}

/**************************************************************************************************************************
Function:		read_hw_sl05_group
Description:	�1�7�1�7HY04�1�7�1�7�1�7�1�7,�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7�1�7
Input:			addr�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�3�1�7�1�7�1�7�0�3�1�7�1�7�0�7
***************************************************************************************************************************/
unsigned char read_hw_sl05_group(unsigned char *data,unsigned char number,unsigned int addr)
{
#ifdef WIN32
	fseek(sl05_fp,addr,SEEK_SET);
	fread(data,number,1,sl05_fp);
#else
	lseek(sl05_fp,addr,SEEK_SET);
	read(sl05_fp,data,number);
#endif
	return 1;
}


#define EPROM_TYPE_EEPROM      0
#define EPROM_TYPE_SPIFLASH    1
/**************************************************************************************************************************
Function:		hw_sl05_init
Description:	sl05�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
Retrun:			0�1�7�1�7�0�3�1�7�1�7�0�2�1�7�1�1�1�71�1�7�0�6�1�7
***************************************************************************************************************************/
int hw_sl05_init(void)
{
#ifdef WIN32
	if ((sl05_fp = fopen("mySL05","rb+")) == NULL)            //�0�2�Մ1�7�1�7�1�7�1�7�1�7�0�4�1�7
	{
		sl05_fp = fopen("mySL05","w");
		if(sl05_fp == NULL)
		{
			logging("Create mySl05 File Err!\n");
			return 0;
		}
		else
			logging("Create mySl05 File Success!\n");
	}
	if(sl05_fp != NULL)
		fclose(sl05_fp);
	if ((sl05_fp = fopen("mySL05","rb+")) == NULL)            //�0�2�Մ1�7�1�7�1�7�1�7�1�7�0�4�1�7
	{
		sl05_fp = fopen("mySL05","w");
		if(sl05_fp == NULL)
		{
			logging("Create mySl05 File Err!\n");
			return 0;
		}
		else
			logging("Create mySl05 File Success!\n");
	}
#else
	if ((sl05_fp = open("/dev/eprom",O_RDWR)) == -1)            //�0�2�Մ1�7�1�7�1�7�1�7�1�7�0�4�1�7
	{
		// logging("Open mySl05 File Err!\n");
		return 0;
	}
	ioctl(sl05_fp, EPROM_TYPE_SPIFLASH, NULL);
#endif
	return 1;
}

/**************************************************************************************************************************
Function:		hw_sl05_exit
Description:	sl05�1�7�1�9�1�7
***************************************************************************************************************************/
void hw_sl05_exit(void)
{
#ifdef WIN32
	fclose(sl05_fp);
#else
	close(sl05_fp);
#endif
}

