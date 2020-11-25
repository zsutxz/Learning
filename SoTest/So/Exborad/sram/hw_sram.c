/********************************************************************
	created:	2014/04/09
	file base:	hw_sram
	file ext:	c
	author:		����
	
	purpose:	Ӳ��sram�����߼�
*********************************************************************/

#include <assert.h>
#include <string.h>

#if WIN32
#include "stdio.h"
FILE *save_fp;

/**************************************************************************************************************************
 Function:		init_hw_sram
 Description:	sram��ʼ��
 Return:		0��ʼ������1��ʼ���ɹ�
***************************************************************************************************************************/
int init_hw_sram(void)
{
	if ((save_fp = fopen("save","rb+")) == NULL)            //Ϊд�����ļ�
	{
		save_fp = fopen("save","w");
		if(save_fp == NULL)
		{
			logging("Create save File Err!\n");
			return 0;
		}
		else
			logging("Create save File Success!\n");
	}
	if(save_fp != NULL)
		fclose(save_fp);
	if ((save_fp = fopen("save","rb+")) == NULL)            //Ϊд�����ļ�
	{
		save_fp = fopen("save","w");
		if(save_fp == NULL)
		{
			logging("Create save File Err!\n");
			return 0;
		}
		else
			logging("Create save File Success!\n");
	}
	return 1;
}
/**************************************************************************************************************************
Function:		LittleBigConvert
Description:	��С��ת��
Input:			data��Ҫת�����ݵ�ָ��
***************************************************************************************************************************/
static void LittleBigConvert(unsigned char *data)
{
	unsigned char temp = 0;
	if(data == NULL)
		return;
	else
	{

		//*data = ((*data << 24)&0xff000000)|((*data << 8)&0x00ff0000) |((*data >> 8)&0x0000ff00) | ((*data >> 24)&0x000000ff);
		temp = data[0];
		data[0] = data[3];
		data[3] = temp;
		temp = data[1];
		data[1] = data[2];
		data[2] = temp;
	}
}

/**************************************************************************************************************************
 Function:		write_hw_sram
 Description:	дsram��д��һ���ֽ�
 Input:			data��Ҫд��sram�е����ݣ�addr:sram��������ݵĵ��?
 return:		���ִ��󷵻�0
 Others:		ֻд�ֽ�������
***************************************************************************************************************************/
unsigned char write_hw_sram(unsigned char data,unsigned int addr)
{
	if(save_fp == NULL)
	{
		assert(0&&"Sram file do not create!");
		return 0;
	}
	fseek(save_fp,addr,0);
	fwrite(&data,1,1,save_fp);
	return 1;
}

/**************************************************************************************************************************
 Function:		read_hw_sram
 Description:	��sram������һ���ֽ�
 Input:			addr����ַ
 Return:		���������?
***************************************************************************************************************************/
unsigned char read_hw_sram(unsigned int addr)
{
	unsigned int data = {0};
	fseek(save_fp,addr,0);
	fread(&data,1,1,save_fp);
	return data;
}

/**************************************************************************************************************************
Function:		write_hw_sram_group
Description:	�������ֽ�������д��sram
Input:			dataд������ݵ�ַ��numerд�������addr��ַ
***************************************************************************************************************************/
unsigned char write_hw_sram_group(unsigned char *data,unsigned int number,unsigned int addr)
{
	if(save_fp == NULL)
	{
		assert(0&&"Sram file do not create!");
		return 0;
	}
	fseek(save_fp,addr,0);
	fwrite(data,number,1,save_fp);
	return 1;
}

/**************************************************************************************************************************
Function:		read_hw_sram_group
Description:	��������sram�ֽ�������
Input:			data:���������ݵ�ַ��number:�������ݸ�����addr����ַ
***************************************************************************************************************************/
unsigned char read_hw_sram_group(unsigned char *data,unsigned int number,unsigned int addr)
{
	fseek(save_fp,addr,0);
	fread(data,number,1,save_fp);
	return 1;
}

/**************************************************************************************************************************
 Function:		clear_hw_sram
 Description:	��sram
 Retrun:		1�����ɣ�0�������?
***************************************************************************************************************************/
unsigned char sram_data = 0;
unsigned char clear_hw_sram(void)
{
	unsigned int count;
	unsigned int temp_data = 0;
	for(count = 0;count<16*1024;count++)
		write_hw_sram(temp_data,count);
	for(count = 0;count<16*1024;count++)
	{
		sram_data = read_hw_sram(count);
		if(sram_data!= 0)
		{
			assert0("clear sram fail!\n");
			return 0;
		}
	}

	return 1;
}

/**************************************************************************************************************************
 Function:		hw_sram_exit
 Description:	sram�ر�
***************************************************************************************************************************/
void hw_sram_exit(void)
{
	if(save_fp != NULL)
		fclose(save_fp);
}

#else

int init_hw_sram(void)
{

	return 1;
}

unsigned char write_hw_sram(unsigned char data,unsigned int addr)
{

	return 1;
}

unsigned char read_hw_sram(unsigned int addr)
{

	return 1;
}

unsigned char write_hw_sram_group(unsigned char *data,unsigned int number,unsigned int addr)
{
	return 1;
}

unsigned char read_hw_sram_group(unsigned char *data,unsigned int number,unsigned int addr)
{
	return 1;
}

unsigned char clear_hw_sram(int _a, int _b)
{
	return _a+_b;
}
void hw_sram_exit(void)
{

}
#endif