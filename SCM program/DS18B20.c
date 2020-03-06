#include <reg51.h>
#include <stdio.h>

#include "DHT11.h"
#include "Timer.h"


#define uchar unsigned char


sbit DQ=P3^6; 				//���ݴ����߽ӵ�Ƭ������Ӧ������ 
unsigned char tempL=0; 		//��ȫ�ֱ���
unsigned char tempH=0; 
unsigned char sdata;			//���������¶ȵ���������
unsigned char xiaoshu1;		//С����һλ
unsigned char xiaoshu2;		//С���ڶ�λ
unsigned char xiaoshu;		//��λС��
bit fg=1;        			//�¶�������־

void delay(unsigned char i)
{
	for(i;i>0;i--);
}


void Init_DS18B20(void) 
{
	unsigned char x=0;
	DQ=1; 					//DQ���ø� 
	delay(8); 				//����ʱ
	DQ=0; 					//���͸�λ���� 
	delay(80); 				//��ʱ��>480us) 
	DQ=1; 					//���������� 
	delay(5); 				//�ȴ���15~60us) 
	x=DQ; 					//��X��ֵ���жϳ�ʼ����û�гɹ���18B20���ڵĻ�X=0������X=1 
	delay(20); 
}

//��һ���ֽ�
ReadOneChar(void)  			//�����������ȴӸ������͵�ƽ1us���ϣ���ʹ��������Ϊ�ߵ�ƽ���Ӷ��������ź�
{
	unsigned char i=0; 		//ÿ����������̵ĳ���ʱ��Ϊ60us������������֮�������1us���ϵĸߵ�ƽ�ָ���
	unsigned char dat=0; 
	for (i=8;i>0;i--) 		//һ���ֽ���8λ 
	{
		DQ=1; 
		delay(1); 
		DQ=0;
		dat>>=1; 
		DQ=1; 
		if(DQ) 
		dat|=0x80; 
		delay(4);
	} 
	return(dat);
}

//дһ���ֽ�
void WriteOneChar(unsigned char dat) 
{ 
	unsigned char i=0; 		//�����ߴӸߵ�ƽ�����͵�ƽ������д��ʼ�źš�15us֮�ڽ�����д��λ�͵��������ϣ�
	for(i=8;i>0;i--) 		//��15~60us֮��������߽��в���������Ǹߵ�ƽ��д1����д0������ 
	{
		DQ=0; 				//�ڿ�ʼ��һ��д����ǰ������1us���ϵĸߵ�ƽ�ָ��ڡ� 
		DQ=dat&0x01; 
		delay(5); 
		DQ=1; 
		dat>>=1;
	} 
	delay(4);
}

//���¶�ֵ����λ��tempL;��λ��tempH;��
void ReadTemperature(void) 
{ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ���
	WriteOneChar(0x44); 				//�����¶�ת��
	delay(125); 						//ת����Ҫһ��ʱ�䣬��ʱ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ��� 
	WriteOneChar(0xbe); 				//���¶ȼĴ�����ͷ����ֵ�ֱ�Ϊ�¶ȵĵ�λ�͸�λ�� 
	tempL=ReadOneChar(); 				//�����¶ȵĵ�λLSB
	tempH=ReadOneChar(); 				//�����¶ȵĸ�λMSB	
	if(tempH>0x7f)      				//���λΪ1ʱ�¶��Ǹ�
	{
		tempL=~tempL;					//����ת����ȡ����һ
		tempH=~tempH+1;       
		fg=0;      						//��ȡ�¶�Ϊ��ʱfg=0
	}
	sdata = tempL/16+tempH*16;      	//��������
	xiaoshu1 = (tempL&0x0f)*10/16; 		//С����һλ
	xiaoshu2 = (tempL&0x0f)*100/16%10;	//С���ڶ�λ
	xiaoshu=xiaoshu1*10+xiaoshu2; 		//С����λ
}
