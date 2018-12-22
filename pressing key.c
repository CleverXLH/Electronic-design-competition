

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
  P1OUT = 0xF0;                        //��ʼֵ0xF0
}

//*************************************************************************
//	74HC573��������ܶ�̬ɨ���ֵ��ʾ����
//*************************************************************************

void Display_Key(uchar num)
{
  uchar i,j;
  j=0x01;                             //��������������λѡ
  for(i=0;i<8;i++)
  {
    DCTR1;                            //��������ܶ����ݵ�74HC573��LE�ܽ��ø�
    WCTR1;                            //���������λ��74HC573��LE�ܽ��ø�
    DATAPORT=~j;                      //����Ҫ��ʾ��λ��Ҳ������һ���������
    WCTR0;                            //����λ���ݣ��������϶������Ժ󣬾���ʾ������                          
    DATAPORT=table[num++];              //��Ҫ��ʾ�����ݣ������Ǽ�ֵ
    DCTR0;                            //��������ݣ��������һ��ʱ��Ƭ��
    j=j<<1;                           //��λ��׼��������һλ����ʾ
    delay_us(500);                    //��ʾһ��ʱ��Ƭ�̣���Ӱ�����Ⱥ���˸��
  }
  Close_LED();                        //��ʾ��8������ܺ�ر��������ʾ��������ܵ��¸�����������Ȳ�һ��
}

//**********************************************************************
//	����ɨ���ӳ��򣬲������ɨ��ķ�ʽ
//**********************************************************************

uchar Key_Scan(void) 
{
  uchar key_check;
  uchar key_checkin;
  key_checkin=KEYIN;          	        //��ȡIO��״̬���ж��Ƿ��м�����
  if((key_checkin & 0xF0)!=0XF0)            	//IO��ֵ�����仯���ʾ�м�����
    {
      delay_ms(20);                  	//������������ʱ20MS
      key_checkin=KEYIN;                //�ٴζ�ȡIO��״̬
      if((key_checkin & 0xF0)!=0XF0)             //ȷ���Ƿ��������м�����
        {  
          key_check=KEYIN;              //�м����£���ȡ�˿�ֵ
          switch (key_check & 0xF0)     //�ж����ĸ�������
            {
              case 0xE0:key=1;break;
              case 0xD0:key=2;break;
              case 0xB0:key=3;break;
              case 0x70:key=4;break;
            }
          
        }
      
   }
  else
   {
     key=0xFF;                          //�޼����£�����FF
   }
  return key;
}

//*************************************************************************
//		������
//*************************************************************************

void main(void)
{
  uint key_store=0x00;                  //û�а�������ʱ��Ĭ����ʾ1  
  WDT_Init();                           //���Ź���ʼ��
  Clock_Init();                         //ʱ�ӳ�ʼ��
  Port_Init();                          //�˿ڳ�ʼ�������ڿ���IO����������
  Close_LED();
  delay_ms(100);                        //��ʱ100ms
  while(1)
    { 
      Key_Scan();                       //����ɨ�裬���Ƿ��а�������
      if(key!=0xff)                     //����а������£�����ʾ�ð�����ֵ1��4
        {
	  {
            switch(key)
               {
                case 1: Display_Key(0x01);key_store=0x01;break;   //���ݼ�ֵ��ʾ���������
                case 2: Display_Key(0x02);key_store=0x02;break;
	        case 3: Display_Key(0x03);key_store=0x03;break;
                case 4: Display_Key(0x04);key_store=0x04;break;
              }
          } 
	
       }
     else
      {
        Display_Key(key_store);          //û�а�����ʱ����ʾ�ϴεļ�ֵ
      }
    }

}