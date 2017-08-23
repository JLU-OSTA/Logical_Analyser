#include "STC89C5xRC.h"
#define FOSC 22118400L
#define BAUD 9600

bit busy;
struct
{
	int ch[8];
	int lst[8];
}Sample_Data;

int Analyser_Scan();
int Sender();
void SendData(unsigned char dat);
void SendStr(char *s);
void delay();

void delay()
{
	unsigned char i, j;

	i = 36;
	j = 217;
	do
	{
		while (--j);
	} while (--i);
}

int main()
{
	P1=0x00;
	P2=0x00;
	SCON=0x50;
	TMOD=0x20;
    TH1=TL1=-(FOSC/12/32/BAUD);
    TR1=1;
	ES=1;
	EA=1;
	SendStr("ok!\n");
	while(1)
	{
		Analyser_Scan();
		Sender();
		delay();
	}
}

void Uart_Isr() interrupt 4 using 1
{
    if (TI)
    {
        TI=0;
        busy=0;
    }
}

void SendData(unsigned char dat)
{
    while (busy);
	ACC=dat;
    busy=1;
    SBUF=ACC;
}

void SendStr(char *s)
{
    while (*s)
    {
        SendData(*s++);
    }
}


int Analyser_Scan()
{
	int i,s[2];
	for(i=0;i<8;i++)
		Sample_Data.lst[i]=Sample_Data.ch[i];
	s[0]=P1;
	for(i=0;i<8;i++)
	{
		Sample_Data.ch[i]=s[0]&0x01;
		s[0]=(s[0]>>1);
	}
	return 1;
}

int Sender()
{
	if(Sample_Data.ch[0])SendStr("1,");else SendStr("0,");
	if(Sample_Data.ch[1])SendStr("3,");else SendStr("2,");
	if(Sample_Data.ch[2])SendStr("5,");else SendStr("4,");
	if(Sample_Data.ch[3])SendStr("7,");else SendStr("6,");
	if(Sample_Data.ch[4])SendStr("9,");else SendStr("8,");
	if(Sample_Data.ch[5])SendStr("11,");else SendStr("10,");
	if(Sample_Data.ch[6])SendStr("13,");else SendStr("12,");
	if(Sample_Data.ch[7])SendStr("15,");else SendStr("14,");
	SendStr("\n");
	return 0;
}