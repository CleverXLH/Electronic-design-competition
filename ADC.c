/********************************************************************
//DM430-A型开发板数码管显示ADC转换结果程序，显示输入到ADC的电压转换后的数字量
//MSP430单片机内部12位ADC，采用内部2.5V基准，单通道单次转换，中断模式
//通过调节ADC_IN0电位器，可以更改输入电压，最大显示量4095
//数码管控制采用74HC573锁存器控制，采用动态显示机制，采用后四位数码管显示
//调试环境：EW430 V5.30
//作者：阿迪 www.avrgcc.com
//时间：2011.09.19
********************************************************************/

#include <msp430x14x.h>
#include "Config.h"                     //开发板配置头文件，主要配置IO端口信息

static uchar Flag=0;                    //标志变量
uint TEMP=0;                            //ADC值暂存变量 

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

//*************************************************************************
//	74HC573控制数码管动态扫描显示函数，显示采集到的温度
//*************************************************************************

void Display(uint data_q,uint data_b,uint data_s,uint data_g)
{
  uchar i,j;

  j=0x01;                         //此数据用来控制位选
  for(i=0;i<4;i++)                //用后4位数码管来显示
  {
     DCTR1;                     
     WCTR1;                     
     DATAPORT=~j;                
     WCTR0;                      
     j=(j<<1);
     DATAPORT=0x00;               //前4位都不显示，送数据00即可
     DCTR0;                      
     delay_ms(2);                
  }
  
  DCTR1;                          //开始显示千位
  WCTR1;                    
  DATAPORT=~j;               
  WCTR0;                    
  j=(j<<1);
  DATAPORT=table[A1];         
  DCTR0;                    
  delay_ms(1); 
  
  DCTR1;                          //开始显示百位
  WCTR1;                    
  DATAPORT=~j;               
  WCTR0;                    
  j=(j<<1);
  DATAPORT=table[A2];         
  DCTR0;                    
  delay_ms(1);              

  DCTR1;                           //开始显示十位
  WCTR1;                     
  DATAPORT=~j;                
  WCTR0;                      
  j=(j<<1);
  DATAPORT=table[A3];           
  DCTR0;                      
  delay_ms(1);                

  DCTR1;                           //开始显示个位
  WCTR1;                     
  DATAPORT=~j;                
  WCTR0;                      
  j=(j<<1);
  DATAPORT=table[A4];           
  DCTR0;                      
  delay_ms(1);                
     
  DCTR1;                     
  WCTR1;                     
  DATAPORT=0xff;                
  WCTR0;                      
}

//*************************************************************************
//		电压数据处理函数
//*************************************************************************
void Data_do(uint temp_d)
{
  uint temp_1,temp_2;
  A1=temp_d/1000;                       //分出千，百，十，和个位
  temp_1=temp_d%1000;
  A2=temp_1/100;
  temp_2=temp_1%100;
  A3=temp_2/10;
  A4=temp_2%10;
}

//*************************************************************************
//	ADC初始化程序，用于配置ADC相关寄存器
//*************************************************************************
void ADC_Init()
{
  P6SEL|=0x01;                                    //选择ADC通道
  ADC12CTL0|= ADC12ON + SHT0_2 + REF2_5V + REFON; //ADC电源控制开，16个CLK，内部基准2.5V
  ADC12CTL1|= ADC12SSEL1 + ADC12SSEL0;            //SMCLK做时钟源
  ADC12MCTL0= SREF0 + INCH_0;                     //参考控制位及通道选择，这里选择通道0
  ADC12IE|= 0x01;                                 //中断允许
  ADC12CTL0|= ENC;                                //使能转换器
}

//*************************************************************************
//	ADC中断服务程序
//*************************************************************************
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
  uchar j;
  while((ADC12CTL1&0x01)==1);           //如果ADC忙，则等待，否则读取ADC转换数值
  Flag = 1 ;
  TEMP = ADC12MEM0;                     //读取ADC转换值
  if ((TEMP>2800)&&(TEMP<3000))
  LED8PORT = ~BIT3;
  delay_ms(20);  
  LED8PORT |= BIT3;
  Data_do(TEMP);                        //处理ADC值，用于显示
  for(j=0;j<50;j++)
  {
    Display(A1,A2,A3,A4);               //显示ADC的数据
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
  Close_LED();                        //LED数码管先关闭
  ADC_Init();                         //初始化ADC配置
  _EINT();                            //使能中断
  Flag=1;                             //标志位先置1
  
  while(1)
  {
    while(Flag==1)
    {
      ADC12CTL0 |= ADC12SC;           //开启转换
      ADC12CTL0 &= ~ADC12SC;          //清零
      Flag=0;                         //清零标志位
    }
  }
}