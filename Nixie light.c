/********************************************************************
//DM430-A型开发板数码管显示程序，依次显示0～7
//数码管控制采用74HC573锁存器控制，采用动态显示机制
//数码管为4位共阴型，对应的位低电平有效，可以控制位信号选择哪一位显示
//调试环境：EW430 V5.30
//作者：阿迪 www.avrgcc.com
//时间：2011.09.19
********************************************************************/

#include <msp430x14x.h>
#include "Config.h"                  //开发板配置头文件，主要配置IO端口信息

//***********************************************************************
//               MSP430IO口初始化
//***********************************************************************
void Port_Init()
{
  LED8SEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  LED8DIR  = 0xFF;                      //设置IO口方向为输出
  LED8PORT = 0xFF;                      //P2口初始设置为FF
  
  DATASEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  DATADIR  = 0xFF;                      //设置IO口方向为输出
  DATAPORT = 0xFF;                      //P4口初始设置为FF
  
  CTRSEL  =  0x00;                      //设置IO口为普通I/O模式，此句可省
  CTRDIR |=  BIT3 + BIT4;               //设置IO口方向为输出,控制口在P63,P64
  CTRPORT =  0xFF;                      //P6口初始设置为FF  
}

//***********************************************************************
//		74HC573控制数码管动态扫描并显示数据
//***********************************************************************

void Display()
{
   uchar i,j;
   j=0x01;                            //此数据用来控制位选
  for(i=0;i<100;i++)
  {
      for(j=0;j<100;j++)
      {
      P6OUT|=BIT3;                            //控制数码管段数据的74HC573的LE管脚置高
      P6OUT|=BIT4;                            //控制数码管位的74HC573的LE管脚置高
      P4OUT=~0x02;                      //设置要显示的位，也就是哪一个数码管亮
      P6OUT&=~BIT3;                            //锁存位数据，下面送上段数据以后，就显示出来了                          
      P4OUT=table[i%10];                //送要显示的数据，就是段数据，如显示0送的是0x3f
      P6OUT&=BIT4;                            //锁存段数据，数码管亮一个时间片刻
      delay_us(500);                      //显示一个时间片刻，会影响亮度和闪烁性
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
//            主程序
//***********************************************************************
void main(void)
{ 
  WDT_Init();                         //看门狗初始化
  Clock_Init();                       //时钟初始化
  Port_Init();                        //端口初始化，用于控制IO口输入或输出
  while(1)
  {
    Display();                        //调用显示函数，依次显示1～7
  }
}