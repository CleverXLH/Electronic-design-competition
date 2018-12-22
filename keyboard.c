/********************************************************************
//DM430-A�Ϳ����������̿��Ƴ���ͨ���������ʾ����ֵ����������ɨ��
//4X4���̽���P1�ڣ�ͨ������IO��״̬�仯������ɨ�谴���Ƿ���
//���Ի�����EW430 V5.30
//���ߣ����� www.avrgcc.com
//ʱ�䣺2011.09.19
********************************************************************/

#include <msp430x14x.h>
#include "Config.h"                     //����������ͷ�ļ�����Ҫ����IO�˿���Ϣ

//***********************************************************************
//               MSP430IO�ڳ�ʼ��
//***********************************************************************
void Port_Init()
{
  P2SEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  P2DIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  P2OUT = 0xFF;                      //P2�ڳ�ʼ����ΪFF
  
  P4SEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  P4DIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  P4OUT = 0xFF;                      //P4�ڳ�ʼ����ΪFF
  
  P6SEL  =  0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  P6DIR |=  BIT3 + BIT4;               //����IO�ڷ���Ϊ���,���ƿ���P63,P64
  P6OUT =  0xFF;                      //P6�ڳ�ʼ����ΪFF 
  
  P1SEL = 0x00;                        //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ      
  P1DIR = 0x0F;                        //����λ����ģʽ������λ���ģʽ���ⲿ��������
  P1OUT= 0x58;                        //��ʼֵ0xF0
}

//*************************************************************************
//	74HC573��������ܶ�̬ɨ���ֵ��ʾ����
//*************************************************************************

void Display_Key(uchar num)
{
  uchar i,j;
  j=0x08;                             //��������������λѡ
  for(i=0;i<8;i++)
  {
    DCTR1;                            //��������ܶ����ݵ�74HC573��LE�ܽ��ø�
    WCTR1;                            //���������λ��74HC573��LE�ܽ��ø�
    DATAPORT=~j;                      //����Ҫ��ʾ��λ��Ҳ������һ���������
    WCTR0;                            //����λ���ݣ��������϶������Ժ󣬾���ʾ������                          
    DATAPORT=table[num];              //��Ҫ��ʾ�����ݣ������Ǽ�ֵ
    DCTR0;                            //��������ݣ��������һ��ʱ��Ƭ��
    j=j;                           //��λ��׼��������һλ����ʾ
    delay_us(500);                    //��ʾһ��ʱ��Ƭ�̣���Ӱ�����Ⱥ���˸��
  }
  Close_LED();                        //��ʾ��8������ܺ�ر��������ʾ��������ܵ��¸�����������Ȳ�һ��
}

//**********************************************************************
//	����ɨ���ӳ��򣬲�������(��)ɨ��ķ�ʽ
//**********************************************************************
uchar Key_Scan(void) 
{
  uchar statevalue,key_checkin,key_check;

  P1OUT=0xf0;                       //��IO���������ʽ��Ϊ���뷽ʽʱҪ�ӳ�һ������
  P1OUT=0xf0;                       //��ȡд���εķ�����ʱ
  key_checkin=P1IN;                  //��ȡIO��״̬���ж��Ƿ��м�����
  if(key_checkin!=0xf0)               //IO��ֵ�����仯���ʾ�м�����
  {
    delay_ms(20);                     //������������ʱ20MS
    key_checkin=P1IN;
    if((key_checkin &=0xf0)!=0xf0)
    {
      P1OUT=0x0E;                    //����ɨ�迪ʼ
      P1OUT=0x0E;                    //д������ʱ����ͬ
      key_check=P1IN;
      switch (key_check)               //���ݶ�����IO��ֵ�жϰ����İ������
      {
        case 0xee:key=15;break;
        case 0xde:key=11;break;
        case 0xbe:key=8;break;
        case 0x7e:key=4;break;
     }
     
     P1OUT=0x0d;
     P1OUT=0x0d;
     key_check=P1IN;
     switch (key_check)
     {
        case 0xed:key=14;break;
        case 0xdd:key=10;break;
        case 0xbd:key=7;break;
        case 0x7d:key=3;break;
     }
     
     P1OUT=0xfb;
     P1OUT=0xfb;
     key_check=P1IN;
     switch (key_check)
     {
        case 0xeb:key=13;break;
        case 0xdb:key=0;break;
        case 0xbb:key=6;break;
        case 0x7b:key=2;break;
     }
     
     P1OUT=0xf7;
     P1OUT=0xf7;
     key_check=P1IN;
     switch (key_check)
     {
        case 0xe7:key=12;break;
        case 0xd7:key=9;break;
        case 0xb7:key=5;break;
        case 0x77:key=1;break;
     }
     
    }
  }
  else
  {
    statevalue=0xff;                      //�ް���ʱ����ֵ
    key=statevalue;
    return(key);
  }
  return(key);
}

//*************************************************************************
//			������
//*************************************************************************

void main(void)
{ 
  uint key_store=0x01;              //û�а�������ʱ��Ĭ����ʾ1
  WDT_Init();                       //���Ź���ʼ��
  Clock_Init();                     //ʱ�ӳ�ʼ��
  Port_Init();                      //�˿ڳ�ʼ�������ڿ���IO����������
  Close_LED();
  while(1)
  {
    Key_Scan();                     //����ɨ�裬���Ƿ��а�������
    if(key!=0xff)                   //����а������£�����ʾ�ð�����ֵ0~15
    {
      {
        switch(key)
        {
          case 1: Display_Key(0x01);key_store=0x01;break;   //���ݼ�ֵ��ʾ���������
          case 2: Display_Key(0x02);key_store=0x02;break;
	  case 3: Display_Key(0x03);key_store=0x03;break;
          case 4: Display_Key(0x04);key_store=0x04;break;
	  case 5: Display_Key(0x05);key_store=0x05;break;
          case 6: Display_Key(0x06);key_store=0x06;break;
          case 7: Display_Key(0x07);key_store=0x07;break;
          case 8: Display_Key(0x08);key_store=0x08;break;
          case 9: Display_Key(0x09);key_store=0x09;break;
	  case 0: Display_Key(0x00);key_store=0x00;break;
          case 10: Display_Key(0x0a);key_store=0x0a;break;
	  case 11: Display_Key(0x0b);key_store=0x0b;break;
	  case 12: Display_Key(0x0c);key_store=0x0c;break;
	  case 13: Display_Key(0x0d);key_store=0x0d;break;
	  case 14: Display_Key(0x0e);key_store=0x0e;break;
          case 15: Display_Key(0x0f);key_store=0x0f;break;
         }
      }
    }
  else
  {
    Display_Key(key_store);              //û�а�����ʱ����ʾ�ϴεļ�ֵ
  }
 }

}