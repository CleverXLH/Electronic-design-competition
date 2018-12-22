

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
  P1OUT = 0xF0;                        //初始值0xF0
}

//*************************************************************************
//	74HC573控制数码管动态扫描键值显示函数
//*************************************************************************

void Display_Key(uchar num)
{
  uchar i,j;
  j=0x01;                             //此数据用来控制位选
  for(i=0;i<8;i++)
  {
    DCTR1;                            //控制数码管段数据的74HC573的LE管脚置高
    WCTR1;                            //控制数码管位的74HC573的LE管脚置高
    DATAPORT=~j;                      //设置要显示的位，也就是哪一个数码管亮
    WCTR0;                            //锁存位数据，下面送上段数据以后，就显示出来了                          
    DATAPORT=table[num++];              //送要显示的数据，这里是键值
    DCTR0;                            //锁存段数据，数码管亮一个时间片刻
    j=j<<1;                           //移位，准备进行下一位的显示
    delay_us(500);                    //显示一个时间片刻，会影响亮度和闪烁性
  }
  Close_LED();                        //显示完8个数码管后关闭数码管显示，否则可能导致各个数码管亮度不一致
}

//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************

uchar Key_Scan(void) 
{
  uchar key_check;
  uchar key_checkin;
  key_checkin=KEYIN;          	        //读取IO口状态，判断是否有键按下
  if((key_checkin & 0xF0)!=0XF0)            	//IO口值发生变化则表示有键按下
    {
      delay_ms(20);                  	//键盘消抖，延时20MS
      key_checkin=KEYIN;                //再次读取IO口状态
      if((key_checkin & 0xF0)!=0XF0)             //确定是否真正的有键按下
        {  
          key_check=KEYIN;              //有键按下，读取端口值
          switch (key_check & 0xF0)     //判断是哪个键按下
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
     key=0xFF;                          //无键按下，返回FF
   }
  return key;
}

//*************************************************************************
//		主程序
//*************************************************************************

void main(void)
{
  uint key_store=0x00;                  //没有按键按下时，默认显示1  
  WDT_Init();                           //看门狗初始化
  Clock_Init();                         //时钟初始化
  Port_Init();                          //端口初始化，用于控制IO口输入或输出
  Close_LED();
  delay_ms(100);                        //延时100ms
  while(1)
    { 
      Key_Scan();                       //键盘扫描，看是否有按键按下
      if(key!=0xff)                     //如果有按键按下，则显示该按键键值1～4
        {
	  {
            switch(key)
               {
                case 1: Display_Key(0x01);key_store=0x01;break;   //根据键值显示到数码管上
                case 2: Display_Key(0x02);key_store=0x02;break;
	        case 3: Display_Key(0x03);key_store=0x03;break;
                case 4: Display_Key(0x04);key_store=0x04;break;
              }
          } 
	
       }
     else
      {
        Display_Key(key_store);          //没有按键的时候显示上次的键值
      }
    }

}