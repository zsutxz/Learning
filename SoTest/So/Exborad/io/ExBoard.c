// xb.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "ExBoard.h"

#ifdef ANDROID

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <android/log.h>
#include "ExBoard.h"
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

static int fd=0;
static int fd2=0;
struct termios options;

#define ERR_EXIT(m) \
    do \
    { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)
unsigned int RunTime = 0;
void handler(int signo)
{
    RunTime += 1;
	//__android_log_print(ANDROID_LOG_INFO, DTAG, "timer_signal function! %d\n", signo);  
}
int init_timer(void)
{
    if (signal(SIGALRM, handler) == SIG_ERR)
        ERR_EXIT("signal error");

    struct timeval tv_interval = {0, 1000};
    struct timeval tv_value = {0, 1000};
    struct itimerval it;
    it.it_interval = tv_interval;
    it.it_value = tv_value;
    setitimer(ITIMER_REAL, &it, NULL);
    return 0;
}
int get_ticks(void)
{
    return RunTime;
}

#endif


#ifdef USE_PC

#include <windows.h>
#include "ExBoard.h"
#include "time.h"
#include "stdio.h"
#define get_ticks clock
static HANDLE hCom;
static OVERLAPPED m_osRead;
static OVERLAPPED m_osWrite;
static DCB dcb;

#endif

unsigned char ComInitFlag = 0;
unsigned char ExboardVison[5] = {'V','-','-','-'};
unsigned char GetBoardVersionFlag = 0;
unsigned int  LastTalkTodownMachineTime = 0; //��ʱ3s����λ�������������ݰ�
unsigned int  lasttimeconnected  = 0;
unsigned char isconsoleconnected = 0; //��̨����,���ڼ���̨�Ƿ�������,0-δ���ӣ�1 - ����

unsigned char pcrgbtype[16] = {1,1,1,1, 1,6,6,6, 6,6,6,3, 3,3,3,3};
unsigned short pcrgbdata[16] = {0x0f,0x0f,0x0f,0x0f,0x0f,0,0,0, 0,0,0,0, 0,0,0,0};
unsigned short pcrgbspeed[16] = {50,50,50,50, 50,50,50,50, 50,50,50,50, 50,50,50,50};

unsigned char bdrgbtype[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
unsigned short bdrgbdata[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
unsigned short bdrgbspeed[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

unsigned short CardOutNum = 0;
unsigned short CardOutLast = 0;
unsigned short EggOutNum = 0;
unsigned short EggOutLast = 0;
unsigned char Coininlast = 0;
unsigned char CoinInNum = 0;
unsigned char LastCoinIn[3] = {0,0};

unsigned char skey_com[4] = {0,0,0,0};
unsigned char stkey_com[4] = {0,0,0,0};

unsigned char SendBuff[15];

unsigned int TotalPacketCnt = 0;
unsigned int ErrorPacketCnt = 0;

unsigned int TestComSend = 0;
unsigned int TestComRecv = 0;

static void DelayMs(int ms)
{
	int lasttime = 0;

	lasttime = get_ticks();
	while((get_ticks()-lasttime)<ms);

}

#ifdef ANDROID
static int ReceiveData( unsigned char *buff_Receive )
{
	int nread = 0;
	int lentoread = 2048;

	nread = read(fd,buff_Receive,lentoread);
	if(nread < 0){
		return 0;
	}
	return nread;
}

static int SendData(unsigned char *buff_send, int length )
{
	 int nwrite = 0;
	 nwrite = write(fd,buff_send,length);

	 return nwrite;
}

#endif

#ifdef USE_PC
int SendData(unsigned char *buff_send, int length)
{
	 DWORD dwBytesWrite;

     DWORD dwBytesRead=2048;//��ȡ���ֽ���

     BOOL bReadStat=FALSE;
	 BOOL bWriteStat;
	 static unsigned long LastTime = 0;

	 ////���η���ʱ�䲻��С�����������Ƭ���˲�����ȷ��������
	 LastTime = get_ticks();

	 bWriteStat = WriteFile(hCom,buff_send,length,&dwBytesWrite,&m_osWrite);
	 if(!bWriteStat)
     {
        if(GetLastError()==ERROR_IO_PENDING)
        {

          //   WaitForSingleObject(m_osWrite.hEvent,5000);
			//�ȴ�ֱ�������꣬�ŷ���
			 GetOverlappedResult(hCom,&m_osWrite,&dwBytesWrite,TRUE);
        }
      }

	 return dwBytesWrite;
}

static int ReceiveData( unsigned char *buff_Receive )
{
	   COMSTAT ComStat;
       DWORD dwErrorFlags;
       DWORD dwBytesRead=1024;//��ȡ���ֽ���
       BOOL bReadStat=FALSE;

	   ClearCommError(hCom,&dwErrorFlags,&ComStat);

	   if(!ComStat.cbInQue)
		   return 0;
	   dwBytesRead=min(dwBytesRead,(DWORD)ComStat.cbInQue);
       bReadStat=ReadFile(hCom,buff_Receive,dwBytesRead,&dwBytesRead,&m_osRead);
	   if(!bReadStat)
       {
		   if(GetLastError()==ERROR_IO_PENDING)
           //GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
		   {
			   WaitForSingleObject(m_osRead.hEvent,10);
			   //ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2����
			   //�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
		   }
       }
      // PurgeComm(hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	   return dwBytesRead;
}

static VOID DebugOutputText(LPCTSTR Format, ...)
{
    TCHAR       str[500];
    va_list     vaList;

    va_start(vaList, Format);

    vsprintf_s(str, 500, Format, vaList);

    OutputDebugString(str);

    va_end(vaList);

    return;
}
#endif


//-------------------------------------------------------------------�����ݷ�װ���ܺ���
unsigned char SendData_Package(unsigned char cmd,unsigned char *data)//9���ֽ�
{
	unsigned char index;
	unsigned char checkvalue;

	SendBuff[0] = 0xaa;       			  		   		             //��̬������ʼ�ֽ�
	SendBuff[1] = cmd;                      					     //�����ֽ�
	for(index=0;index<5;index++)         							 //����12��Ч����
	{
		SendBuff[2+index] = data[index];
	}
	checkvalue = 0;
	for(index = 0; index<7; index++)	      		 		           //�������ݰ����
	{
		checkvalue ^= SendBuff[index];
	}
	SendBuff[7] = checkvalue;                              //���һλ����У���룬������֤

	SendData(SendBuff,8);

	return 1;
}

/*********************************************************************
�������ܣ�����λ�������ֺ����������ư��Ƿ������ϴ���
���������void
���������void
**********************************************************************/
void ExBoardCtltalk(void)
{
	unsigned char databuff[12] ={0};
	SendData_Package(0xaa,databuff);
	TestComSend++;
}

#ifdef ANDROID
int ExBoardInit(void )
{
	int ret;
	unsigned char index;
	unsigned char databuff[12];
    unsigned int  start_time = 0;
	//ttyS1 ttyS3//
    char *ComTable[4] = {"/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4"};
	
	init_timer();
    for(index=0;index<4;index++)
    {
		fd = open(&ComTable[index][0], O_RDWR | O_NOCTTY | O_NDELAY);//����Ϊ�Ƕ���//
		if (fd == -1)
		{
			continue;
		}
		else
		{
			tcgetattr(fd, &options);
			memset( &options, 0, sizeof(options) );
			cfsetispeed(&options, B115200);
			cfsetospeed(&options, B115200);
			options.c_cflag |= CS8;//set 8bit data//
			options.c_cflag |= (CLOCAL | CREAD);
			tcsetattr(fd, TCSANOW, &options);
			tcflush(fd,TCIOFLUSH);

			GetBoardVersionFlag = 0;
			start_time = get_ticks();
			while((get_ticks()-start_time)<250)   //�ȴ�500ms������ͬ��
			{
				databuff[0] = 0xaa;
				databuff[1] = 0x55;
				SendData_Package(0x00,databuff);   //���ͻ�ȡ���ư�ָ��
				DelayMs(50);
				ExBoardProcess();
			}
			if(GetBoardVersionFlag==1)      //�յ��򿪴��ڵĿ��ư巢������Ӧ���źţ�Ȼ���ʼ������������
			{
				Coininlast = CoinInNum ;
				// PiaoOutLast = PiaoOutNum;
				EggOutLast = EggOutNum;
				break;
			}
		}
		if(fd > 0)
		{
			close(fd);
			fd = 0;
		}
	}
    return GetBoardVersionFlag;
}

void ExBoardExit(void)
{
	if(fd >= 0)
	{
		close(fd);
	}
	if(fd2 >= 0)
	{
		close(fd2);
	}
}
#endif

#ifdef USE_PC
int ExBoardInit(void )
{

	unsigned char index;
    unsigned int  clocktime = 0;
	unsigned char databuff[12];
    unsigned char ComTable[4][5] = {{"COM1"},{"COM2"},{"COM3"},{"COM4"}};

    COMMTIMEOUTS TimeOuts;
	for(index=0;index<4; index++)
	{
		hCom = CreateFile(&ComTable[index][0], GENERIC_READ|GENERIC_WRITE,  //������д
						  0,						   //ֵ����Ϊ������ʾ��������������Ӧ�ó�����
						  NULL,					   // no security attrs
						  OPEN_EXISTING,			   // ָ����δ��ļ����ڴ��豸��������һ���豸��ʱ���˲�������ָ��Ϊ�ϣУţΣߣţأɣӣԣɣΣ�
						  FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,	//ָ���ļ����Լ���ر�־���ص���ʽ�����Ƕ��ڴ��пڣ�Ψһ������������ǣƣɣ̣ţߣƣ̣��ǣߣϣ֣ţң̣��УУţĻ�
						  NULL);

		if( hCom ==  (HANDLE)-1 )  //�򿪴���ʧ�ܣ���������һ������
		{
			//break;
		}
		else   //���ڴ򿪳ɹ� ��������ʼ�����ͻ�ȡ���ư��ָ��
		{
			GetCommState(hCom, &dcb );	//������ԭ���Ĳ�������
			dcb.BaudRate = 115200;
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
			dcb.fBinary = TRUE;
			dcb.fParity = FALSE;
			SetCommState( hCom, &dcb ); //���ڲ�������
			SetCommMask(hCom, EV_RXCHAR|EV_TXEMPTY );//�����¼�����������
			SetupComm( hCom, 10240,10240) ; //�������롢����������Ĵ�С
			PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR|PURGE_RXCLEAR);//��ɾ����롢���������

			//���ö���ʱ
			TimeOuts.ReadIntervalTimeout=1;
			TimeOuts.ReadTotalTimeoutMultiplier=1;
			TimeOuts.ReadTotalTimeoutConstant=1;
			//����д��ʱ
			TimeOuts.WriteTotalTimeoutMultiplier=1;
			TimeOuts.WriteTotalTimeoutConstant=1;
			SetCommTimeouts(hCom,&TimeOuts); //���ó�ʱ

			memset(&m_osRead,0,sizeof(OVERLAPPED));
			m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

			memset(&m_osWrite,0,sizeof(OVERLAPPED));
			m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
			
			ComInitFlag = 1;

			clocktime = get_ticks();
			while((get_ticks()-clocktime)<250)   //�ȴ�500ms������ͬ��
			{
				databuff[0] = 0xaa;
				databuff[1] = 0x55;
				SendData_Package(0x00,databuff);   //���ͻ�ȡ���ư�ָ��
				DelayMs(50);
				ExBoardProcess();
			}
			if(GetBoardVersionFlag==1)      //�յ��򿪴��ڵĿ��ư巢������Ӧ���źţ�Ȼ���ʼ������������
			{
				Coininlast = CoinInNum;
				CardOutLast = CardOutNum;
				EggOutLast = EggOutNum;
				break;
			}
			
			ComInitFlag = 0;
			CloseHandle(hCom);    //�رյ�ǰ���ڣ�����һ������ ,//�򿪵Ĵ��ڲ��������Ͽ��ư壬�������������ڲ�ѯ
			hCom = (HANDLE)-1;
		}
	}
    return GetBoardVersionFlag;
}


void ExBoardExit(void)
{
	CloseHandle(hCom);
	CloseHandle(m_osRead.hEvent);
	CloseHandle(m_osWrite.hEvent);

	return ;
}
#endif


void syncrgbdata(void);
int keyandctlprocess(void);

//�����崦��������ÿ�궼Ҫ��������������Խ��մ�������
int ExBoardProcess( void )
{
	static unsigned int last_process_time = 0;
	static unsigned int last_test_time = 0;

	if((get_ticks()-last_process_time)>=15)
	{
		last_process_time = get_ticks();
	}
	else
	{
		return 1;
	}
	keyandctlprocess();
	syncrgbdata();

	return 1;
}

//�����崦��������ÿ�궼Ҫ��������������Խ��մ�������
int keyandctlprocess( void )
{
	unsigned char index;
	int length_read = 0;
	int datalen = 0;
	
	static int length_receive=0;
	static unsigned char ReceiveBuff[81920];
	
    unsigned char CheckValue;
	unsigned char Buffer_char[20480];

	int temp,i,j;
	static unsigned char stkeylast[4] = {0x00,0x00,0x00,0x00};
	
	while( (length_read=ReceiveData( Buffer_char ))>0)
	{
		for( i=0; i<length_read; i++ )
			ReceiveBuff[length_receive+i] = Buffer_char[i];
		length_receive += length_read;
	}

  	for( i=0; i<length_receive; )
	{
	  if(ReceiveBuff[i] ==0xaa)
	  {
           if( (length_receive-i)>=7 )
           {
			   if(ReceiveBuff[i+1]==0)datalen = 6;
			   else if(ReceiveBuff[i+1]==1)datalen = 6;
			   else if(ReceiveBuff[i+1]==2)datalen = 7;
			   else if(ReceiveBuff[i+1]==4)datalen = 8;
			   else if(ReceiveBuff[i+1]==5)datalen = 6;
			   else if(ReceiveBuff[i+1]==7)datalen = 7;
			   else if(ReceiveBuff[i+1]==0x0a)datalen = 6;

			   if( (length_receive-i)<(datalen+1) )
			   {
				   break;
			   }

			   CheckValue = 0;
			   for(index=0;index<datalen;index++)
			   {
					CheckValue ^= ReceiveBuff[i+index];
			   }

				if( CheckValue==ReceiveBuff[i+datalen] )   //У�����ݰ�
				{
					switch(ReceiveBuff[i+1])
					{
					   	case 0x00: //��λ����λ�󣬱���Ͷ����ͬ��
								   Coininlast = 0;
								   CoinInNum = 0;
								   CardOutNum = 0;
								   CardOutLast = 0;
								   EggOutNum = 0;
								   EggOutLast = 0;
								   LastCoinIn[0] = 0;
								   LastCoinIn[1] = 0;
								   LastCoinIn[2] = 0;
								   TestComSend = 0;
								   TestComRecv = 0;
								   break;

						case 0x01://��ȡ���ư�汾��
								   ExboardVison[0] = ReceiveBuff[i+2];
								   ExboardVison[1] = ReceiveBuff[i+3];
								   ExboardVison[2] = ReceiveBuff[i+4];
								   ExboardVison[3] = ReceiveBuff[i+5];
								   GetBoardVersionFlag = 1;
								   break;

						case 0x02://������Ͷ����Ʊ�źż��
								  stkey_com[0] = ReceiveBuff[i+2]^0xff;
								  skey_com[0] |= (stkeylast[0]^0xff) & stkey_com[0];//�½��ؼ��
								  stkeylast[0] = stkey_com[0];

                   				  stkey_com[1] = ReceiveBuff[i+3]^0xff;
								  skey_com[1] |= (stkeylast[1]^0xff) & stkey_com[1];//�½��ؼ��
								  stkeylast[1] = stkey_com[1];

								  stkey_com[2] = ReceiveBuff[i+4]^0xff;
								  skey_com[2] |= (stkeylast[2]^0xff) & stkey_com[2];//�½��ؼ��
								  stkeylast[2] = stkey_com[2];

								  stkey_com[3] = ReceiveBuff[i+5]^0xff;
								  skey_com[3] |= (stkeylast[3]^0xff) & stkey_com[3];//�½��ؼ��
								  stkeylast[3] = stkey_com[3];
								  
								  LastCoinIn[2] = LastCoinIn[1];
								  LastCoinIn[1] = LastCoinIn[0];
								  LastCoinIn[0] = ReceiveBuff[i+6];
								  if( (LastCoinIn[2]==LastCoinIn[1])&&(LastCoinIn[1]==LastCoinIn[0]) )
								  {
									CoinInNum = ReceiveBuff[i+6];
								  }
								  break;
						case 0x04://��̨����/��Ʊ��//
								  if( ReceiveBuff[i+7]==(ReceiveBuff[i+2]^ReceiveBuff[i+3]^ReceiveBuff[i+4]^ReceiveBuff[i+5]^ReceiveBuff[i+6]) )
								  {
									  CardOutNum = (ReceiveBuff[i+3]<<8)+ReceiveBuff[i+4];
									  EggOutNum = (ReceiveBuff[i+5]<<8)+ReceiveBuff[i+6];
								  }
								  break;
					    case 0x07://��Ч����-��������//
								  temp = ReceiveBuff[i+2]>>4;
								  bdrgbtype[temp] = ReceiveBuff[i+2]&0x0f;
								  bdrgbdata[temp] = (ReceiveBuff[i+3]<<8)|ReceiveBuff[i+4];
								  bdrgbspeed[temp] = (ReceiveBuff[i+5]<<8)|ReceiveBuff[i+6];
								  //DebugOutputText("RGB:%d from IO board\r\n",temp);
								  break;	
					    case 0x0a://��������
								  if((ReceiveBuff[i+2]==0x55)&&(ReceiveBuff[i+3]==0x66)&&(ReceiveBuff[i+4]==0xbb)&&(ReceiveBuff[i+5]==0xcc))
								  {
									   TestComRecv++;
								  }
								  break;
						default:break;
					}
					TotalPacketCnt++;
					i += datalen+1;  //��ѯ��һ�����ݰ�
					isconsoleconnected = 1;
					lasttimeconnected = get_ticks();
				}
				else
				{
					ErrorPacketCnt++;
					TotalPacketCnt++;
					i++;
					//DebugOutputText("check err,data:0x%x\r\n",ReceiveBuff[i]);
				}
		   }
		   else
			   break;
		}
	  else
	  {
			ErrorPacketCnt++;
			//DebugOutputText("not head:0x%02x\n",ReceiveBuff[i]);
			i++;
	  }
	}

	for(j=0; j<length_receive-i;j++)
	{
		ReceiveBuff[j] = ReceiveBuff[i+j];
	}
	length_receive -= i;

   if((get_ticks() - lasttimeconnected)>2000)
   {
		lasttimeconnected = get_ticks();
		isconsoleconnected = 0;

		skey_com[0] = 0;          //���ڳ�ʼ��ʱ�����������
		skey_com[1] = 0;
		skey_com[2] = 0;
		stkey_com[0] = 0;
		stkey_com[1] = 0;
		stkey_com[2] = 0;
	}
    if((get_ticks() - LastTalkTodownMachineTime)>100)//3s�ӷ���һ���������ݰ�
	{
		LastTalkTodownMachineTime = get_ticks();
		ExBoardCtltalk();
	}

	return 1;
}

//����ͨ�Ŷ�����������QCʹ��//
int ExBoardComLost(void)
{
	int ret = 0;

	ret = TestComSend-TestComRecv;
	if(ret>1)
		return ret;
	else
		return 0;
}

/************************************************************************
*����˵������ȡͶ����
*�����������
*������������ε����ڼ��Ͷ����
************************************************************************/
unsigned char ExBoardGetCoinIn(void)
{
	unsigned char temp = 0;

    if(CoinInNum >= Coininlast)
	{
		temp = CoinInNum - Coininlast;
	}
	else
	{
	  	temp = 256 - Coininlast + CoinInNum;
	}

    Coininlast = CoinInNum;
	return temp;
}

unsigned char ExBoardSetCardOut(unsigned short cardnum)
{
	unsigned long start_clock;
	unsigned char TryTime = 0;
	unsigned char databuff[12] ={0};

	if(isconsoleconnected==0)return 0;

	databuff[0] = 0;
	databuff[1] = 0;
    databuff[2] = cardnum>>8;
	databuff[3] = cardnum&0xff;

	SendData_Package(0x03,databuff);
	start_clock = get_ticks();

	while(1)
	{
        ExBoardProcess();
		if( cardnum==CardOutNum )
        {
			CardOutLast = CardOutNum;
 			return 1;
		}
		if( get_ticks()-start_clock>50 )
		{
			if(TryTime>=5)
				return 0;
			
			// DebugOutputText("ExBoardSetCardOut retry\r\n");
			SendData_Package(0x03,databuff);
			start_clock = get_ticks();
			TryTime++;
		}
	}
}

unsigned char ExBoardSetEggOut(unsigned short eggnum)
{
	unsigned long start_clock;
	unsigned char TryTime = 0;
	unsigned char databuff[12] ={0};

	if(isconsoleconnected==0)return 0;

	databuff[0] = 1;//��Ť��//
	databuff[1] = 0;
    databuff[2] = eggnum>>8;
	databuff[3] = eggnum&0xff;

	SendData_Package(0x03,databuff);
	start_clock = get_ticks();

	while(1)
	{
        ExBoardProcess();
		if( eggnum==EggOutNum )
        {
			EggOutLast = EggOutNum;
 			return 1;
		}
		if( get_ticks()-start_clock>50 )
		{
			if(TryTime>=5)
				return 0;
			
			// DebugOutputText("ExBoardSetEggOut retry\r\n");
			SendData_Package(0x03,databuff);
			start_clock = get_ticks();
			TryTime++;
		}
	}
}
unsigned int ExBoardGetCardOut(void)
{
	unsigned int temp = 0;

	if(CardOutLast>CardOutNum)
	{
		temp = CardOutLast-CardOutNum;
		CardOutLast = CardOutNum;
	}
	else
	{
		temp = 0;
	}
	return temp;
}


unsigned int ExBoardGetEggOut(void)
{
	unsigned int temp = 0;

	if(EggOutLast>EggOutNum)
	{
		temp = EggOutLast-EggOutNum;
		EggOutLast = EggOutNum;
	}
	else
	{
		temp = 0;
	}
	return temp;
}


unsigned char ExBoardClearCardOut(void)
{
	unsigned long start_clock;
	unsigned char TryTime = 0;
	unsigned char databuff[12] ={0};

	if(isconsoleconnected==0)return 0;

	databuff[0] = 0;//�������//  
	databuff[1] = 0;
    databuff[2] = 0xaa;
    databuff[3] = 0x55;
	SendData_Package(0x05,databuff);
	start_clock = get_ticks();

	while(1)
	{
        ExBoardProcess();
		if( 0==CardOutNum )
        {
			CardOutLast = CardOutNum;
 			return 1;
		}
		if( get_ticks()-start_clock>50 )
		{
			if(TryTime>=5)
				return 0;
			
			// DebugOutputText("ExBoardSetCardOut retry\r\n");
			SendData_Package(0x05,databuff);
			start_clock = get_ticks();
			TryTime++;
		}
	}
}

unsigned char ExBoardClearEggOut(void)
{
	unsigned long start_clock;
	unsigned char TryTime = 0;
	unsigned char databuff[12] ={0};

	if(isconsoleconnected==0)return 0;

	databuff[0] = 1;//�������//  
	databuff[1] = 0;
    databuff[2] = 0xaa;
    databuff[3] = 0x55;
	SendData_Package(0x05,databuff);
	start_clock = get_ticks();

	while(1)
	{
        ExBoardProcess();
		if( 0==EggOutNum )
        {
			EggOutLast = EggOutNum;
 			return 1;
		}
		if( get_ticks()-start_clock>50 )
		{
			if(TryTime>=5)
				return 0;
			
			// DebugOutputText("ExBoardSetPiaoOut retry\r\n");
			SendData_Package(0x05,databuff);
			start_clock = get_ticks();
			TryTime++;
		}
	}
}

/*
* ������  ��ExBoardExecute
* ��    �ܣ����Ƹ�����Ч��
* ������idnum/���ƶ���			type/��Ч����                    data/���ݼ�����(16λ)                     speed/�ٶȲ���(16λ)
*
*       0-�忨��(�׵�)	        0-�ر�                            ��Ч                                    ��Ч
*               				1-��ɫ����                        RGB��ɫֵ(B-5bit,G-5bit,R-5bit(��))	  ��Ч
*								2-��ɫ��˸                        RGB��ɫֵ(B-5bit,G-5bit,R-5bit(��))	  ����ms(����500-1000ms)
*								3-�Զ�����                        ��Ч                                    �ٶ�ms(����25,50,75,100ms)
*                               4-��ɫ��˸                        ��Ч                                    ����ms(����100-1000ms)
*								5-��ɫ                            ��Ч                                    ����ms(����100-1000ms)
*								6-����                            ����Ч��                                �ٶ�ms(����25,50,75,100,200ms)
*								7-Ծ��                            RGB��ɫֵ(B-5bit,G-5bit,R-5bit(��))     ����ms(����100-1000ms)
*                               8-����2
*
*       1-������(�׵�)	    ͬ��                              ͬ��                                      ͬ��
*
*       2-�����(�׵�)	    ͬ��                              ͬ��                                      ͬ��
*
*       3-�м���(�׵�)	    ͬ��                              ͬ��                                      ͬ��
*
*       4-�Ҽ���(�׵�)	    ͬ��                              ͬ��                                      ͬ��
*
*       ע���׵�ֻ֧��Ч����0,1,2,7����ɫֵ��ӦRGB��R��
*/

void ExBoardExecute(unsigned char idnum,unsigned char type,unsigned short data,unsigned short speed)
{
	pcrgbtype[idnum] = type;
	pcrgbdata[idnum] = data;
	pcrgbspeed[idnum] = speed;
}

void syncrgbdata(void)
{
	unsigned char TryTime = 0;
	unsigned char index = 0;
	unsigned char idnum = 0;
	unsigned char databuff[16];

	for(index=0;index<14;index++)
	{
		idnum = index;
		if( (pcrgbtype[idnum]!=bdrgbtype[idnum])||(bdrgbdata[idnum]!=pcrgbdata[idnum])||(bdrgbspeed[idnum]!=pcrgbspeed[idnum]) )
		{
			//DebugOutputText("Sync rgb:%d\r\n",idnum);

			databuff[0] = (idnum<<4)|pcrgbtype[idnum];
			databuff[1] = (unsigned char)(pcrgbdata[idnum]>>8);
			databuff[2] = (unsigned char)pcrgbdata[idnum];
			databuff[3] = (unsigned char)(pcrgbspeed[idnum]>>8);
			databuff[4] = (unsigned char)pcrgbspeed[idnum];
			SendData_Package(0x04,databuff);   //���ͻ�ȡ���ư�ָ��//
		}
	}
}


