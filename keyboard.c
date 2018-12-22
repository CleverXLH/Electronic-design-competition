/********************************************************************
//DM430-A型开发板矩阵键盘控制程序，通过数码管显示按键值，采用逐行扫描
//4X4键盘接在P1口，通过控制IO口状态变化，逐行扫描按键是否按下
//调试环境：EW430 V5.30
//作者：阿迪 www.avrgcc.com
//时间：2011.09.19
********************************************************************/

#include <msp430x14x.h>
#include "Config.h"                     //开发板配置头文件，主要配置IO端口信息

//***********************************************************************
//               MSP430IO口初始化
//***********************************************************************
void Port_Init()
{
  P2SEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  P2DIR  = 0xFF;                      //设置IO口方向为输出
  P2OUT = 0xFF;                      //P2口初始设置为FF
  
  P4SEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  P4DIR  = 0xFF;                      //设置IO口方向为输出
  P4OUT = 0xFF;                      //P4口初始设置为FF
  
  P6SEL  =  0x00;                      //设置IO口为普通I/O模式，此句可省
  P6DIR |=  BIT3 + BIT4;               //设置IO口方向为输出,控制口在P63,P64
  P6OUT =  0xFF;                      //P6口初始设置为FF 
  
  P1SEL = 0x00;                        //设置IO口为普通I/O模式，此句可省      
  P1DIR = 0x0F;                        //高四位输入模式，低四位输出模式，外部上拉电阻
  P1OUT= 0x58;                        //初始值0xF0
}

//*************************************************************************
//	74HC573控制数码管动态扫描键值显示函数
//*************************************************************************

void Display_Key(uchar num)
{
  uchar i,j;
  j=0x08;                             //此数据用来控制位选
  for(i=0;i<8;i++)
  {
    DCTR1;                            //控制数码管段数据的74HC573的LE管脚置高
    WCTR1;                            //控制数码管位的74HC573的LE管脚置高
    DATAPORT=~j;                      //设置要显示的位，也就是哪一个数码管亮
    WCTR0;                            //锁存位数据，下面送上段数据以后，就显示出来了                          
    DATAPORT=table[num];              //送要显示的数据，这里是键值
    DCTR0;                            //锁存段数据，数码管亮一个时间片刻
    j=j;                           //移位，准备进行下一位的显示
    delay_us(500);                    //显示一个时间片刻，会影响亮度和闪烁性
  }
  Close_LED();                        //显示完8个数码管后关闭数码管显示，否则可能导致各个数码管亮度不一致
}

//**********************************************************************
//	键盘扫描子程序，采用逐行(列)扫描的方式
//**********************************************************************
uchar Key_Scan(void) 
{
  uchar statevalue,key_checkin,key_check;

  P1OUT=0xf0;                       //在IO口由输出方式变为输入方式时要延迟一个周期
  P1OUT=0xf0;                       //采取写两次的方法延时
  key_checkin=P1IN;                  //读取IO口状态，判断是否有键按下
  if(key_checkin!=0xf0)               //IO口值发生变化则表示有键按下
  {
    delay_ms(20);                     //键盘消抖，延时20MS
    key_checkin=P1IN;
    if((key_checkin &=0xf0)!=0xf0)
    {
      P1OUT=0x0E;                    //逐行扫描开始
      P1OUT=0x0E;                    //写两次延时，下同
      key_check=P1IN;
      switch (key_check)               //根据读到的IO口值判断按键的按下情况
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
    statevalue=0xff;                      //无按键时返回值
    key=statevalue;
    return(key);
  }
  return(key);
}

//*************************************************************************
//			主程序
//*************************************************************************

void main(void)
{ 
  uint key_store=0x01;              //没有按键按下时，默认显示1
  WDT_Init();                       //看门狗初始化
  Clock_Init();                     //时钟初始化
  Port_Init();                      //端口初始化，用于控制IO口输入或输出
  Close_LED();
  while(1)
  {
    Key_Scan();                     //键盘扫描，看是否有按键按下
    if(key!=0xff)                   //如果有按键按下，则显示该按键键值0~15
    {
      {
        switch(key)
        {
          case 1: Display_Key(0x01);key_store=0x01;break;   //根据键值显示到数码管上
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
    Display_Key(key_store);              //没有按键的时候显示上次的键值
  }
 }

}