#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

/*当BRCLK=CPU_F时用下面的公式可以计算，否则要根据设置加入分频系数*/
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位

#define LED8PORT        P2OUT                   //P2接8个LED灯
#define LED8SEL         P2SEL                   //P2接8个LED灯
#define LED8DIR         P2DIR                   //P2接8个LED灯

#define DATAPORT	P4OUT                   //数据口所在端口P4
#define DATASEL         P4SEL                   //数据口功能寄存器，控制功能模式
#define DATADIR         P4DIR                   //数据口方向寄存器

#define CTRPORT	        P6OUT                   //控制线所在的端口P6
#define CTRSEL          P6SEL                   //控制口功能寄存器，控制功能模式
#define CTRDIR          P6DIR                   //控制口方向寄存器

#define DCTR0	        P6OUT &= ~BIT4          //数码管段控制位信号置低
#define DCTR1           P6OUT |=  BIT4          //数码管段控制位信号置高
#define WCTR0	        P6OUT &= ~BIT3          //数码管位控制位信号置低
#define WCTR1           P6OUT |=  BIT3          //数码管位控制位信号置高

#define KEYPORT	        P1OUT                   //按键所在的端口P1
#define KEYSEL          P1SEL                   //控制口功能寄存器，控制功能模式
#define KEYDIR          P1DIR                   //控制口方向寄存器
#define KEYIN           P1IN                    //键盘扫描判断需要读取IO口状态值

uchar key;					//键值变量

//***********************************************************************
//			共阴数码管显示的断码表
//***********************************************************************

uchar table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
               0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

//***********************************************************************
//                   系统时钟初始化
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
}
//***********************************************************************
//               MSP430内部看门狗初始化
//***********************************************************************
void WDT_Init()
{
   WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
}

//***********************************************************************
//		关闭数码管
//***********************************************************************
void Close_LED()
{
  DATASEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  DATADIR  = 0xFF;                      //设置IO口方向为输出
  DATAPORT = 0xFF;                      //P4口初始设置为FF
  
  CTRSEL  =  0x00;                      //设置IO口为普通I/O模式，此句可省
  CTRDIR |=  BIT3 + BIT4;               //设置IO口方向为输出,控制口在P63,P64
  CTRPORT =  0xFF;                      //P6口初始设置为FF  
  
  DCTR1;                                //关掉数码管，以免显示乱码                   
  WCTR1;                     
  DATAPORT=0xFF;                        //关数码管            
  WCTR0; 
}