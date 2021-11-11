#include <msp430x14x.h>
#include "24l01.h"
#include "hc_05.h"
#include "GY61.h"
#include "AD.h"
//===========================延时函数===================================
#define CPU_F ((double)8000000)
#define delay_us(x)  __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))
//===========================系统初始化============================
void InitSys()
{
   unsigned int iq0;
   _DINT();
   BCSCTL1 &=~XT2OFF;
   do
   {
      IFG1 &= ~OFIFG ;                   // 清除振荡器失效标志
      for (iq0 = 0xFF; iq0 > 0; iq0--);	// 延时，等待XT2起振
   }
   while ((IFG1 & OFIFG) != 0);		// 判断XT2是否起振		
   BCSCTL2 =SELM1+SELS;                // MCLK,SMCLK时钟为XT2		
} 
//==========================主函数==============================================
void main()
{
  WDTCTL = WDTPW + WDTHOLD;  //禁止看门狗 
  InitSys();                //晶振设置为8Mz
  RF24L01_IO_set();        //24l01端口设置
  init_NRF24L01() ;        //24l01初始化
  init_ADC12();              //AD初始化
  init_usart0();           //GY61串口初始化
  init_uart1();              //蓝牙串口初始化
  TYPE_IO_SET();           //按键初始化
  LED_IO_SET();            //led初始化
  while(1)
  { 
    sel_mode();                                 //模式判断
    AD_start_one();                            //开启一次AD转换
    Gy61_do();                                //180度舵机启动  
    count_sent();                            //生成发送数组
    nRF24L01_TxPacket(inf);                 //把数据发送出去	
    SPI_RW_Reg(WRITE_REG+STATUS,0XFF);     //清状态寄存器
    delay_ms(5);
  }
}  