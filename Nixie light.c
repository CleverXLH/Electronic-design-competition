/********************************************************************
//DM430-A�Ϳ������������ʾ����������ʾ0��7
//����ܿ��Ʋ���74HC573���������ƣ����ö�̬��ʾ����
//�����Ϊ4λ�����ͣ���Ӧ��λ�͵�ƽ��Ч�����Կ���λ�ź�ѡ����һλ��ʾ
//���Ի�����EW430 V5.30
//���ߣ����� www.avrgcc.com
//ʱ�䣺2011.09.19
********************************************************************/

#include <msp430x14x.h>
#include "Config.h"                  //����������ͷ�ļ�����Ҫ����IO�˿���Ϣ

//***********************************************************************
//               MSP430IO�ڳ�ʼ��
//***********************************************************************
void Port_Init()
{
  LED8SEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  LED8DIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  LED8PORT = 0xFF;                      //P2�ڳ�ʼ����ΪFF
  
  DATASEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  DATADIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  DATAPORT = 0xFF;                      //P4�ڳ�ʼ����ΪFF
  
  CTRSEL  =  0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  CTRDIR |=  BIT3 + BIT4;               //����IO�ڷ���Ϊ���,���ƿ���P63,P64
  CTRPORT =  0xFF;                      //P6�ڳ�ʼ����ΪFF  
}

//***********************************************************************
//		74HC573��������ܶ�̬ɨ�貢��ʾ����
//***********************************************************************

void Display()
{
   uchar i,j;
   j=0x01;                            //��������������λѡ
  for(i=0;i<100;i++)
  {
      for(j=0;j<100;j++)
      {
      P6OUT|=BIT3;                            //��������ܶ����ݵ�74HC573��LE�ܽ��ø�
      P6OUT|=BIT4;                            //���������λ��74HC573��LE�ܽ��ø�
      P4OUT=~0x02;                      //����Ҫ��ʾ��λ��Ҳ������һ���������
      P6OUT&=~BIT3;                            //����λ���ݣ��������϶������Ժ󣬾���ʾ������                          
      P4OUT=table[i%10];                //��Ҫ��ʾ�����ݣ����Ƕ����ݣ�����ʾ0�͵���0x3f
      P6OUT&=BIT4;                            //��������ݣ��������һ��ʱ��Ƭ��
      delay_us(500);                      //��ʾһ��ʱ��Ƭ�̣���Ӱ�����Ⱥ���˸��
      P6OUT|=BIT3;
      P6OUT|=BIT4;
      P4OUT=~0x01;
      P6OUT&=~BIT3;
      P4OUT=table[i/10];
      P6OUT&=BIT4;
      delay_us(500);             
    }
  }
}


//***********************************************************************
//            ������
//***********************************************************************
void main(void)
{ 
  WDT_Init();                         //���Ź���ʼ��
  Clock_Init();                       //ʱ�ӳ�ʼ��
  Port_Init();                        //�˿ڳ�ʼ�������ڿ���IO����������
  while(1)
  {
    Display();                        //������ʾ������������ʾ1��7
  }
}