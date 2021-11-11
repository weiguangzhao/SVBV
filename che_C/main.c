#include <msp430x14x.h>              //3.1   6.6    6.7   4.4
#include "24l01.h"
#include "uart0.h"
#include "Timer_do.h"
#include "filter_z.h"
//===========================延时函数===================================
#define CPU_F ((double)8000000)
#define delay_us(x)  __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))
//========================LED接收指示灯=================================
#define LED_0 P3OUT&=~BIT3             //指示灯熄灭
#define LED_1 P3OUT|=BIT3             //指示灯亮起
void LED_IO_SET()
{
  P3SEL&=~BIT3;                  //设置为普通IO口模式
  P3DIR|=BIT3;                  //设置为输出模式
}

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
//===========================主函数============================
void main()
{
   WDTCTL = WDTPW + WDTHOLD; //禁止看门狗
   InitSys();               //设置晶振为8Mz
   RF24L01_IO_set();         //24l01IO设置
   init_NRF24L01() ;         //24l01初始化
   LED_IO_SET();             //接收指示灯IO口设置
   TimerB_IO_SET();          //定时器端口设置
   init_TimerB();            //定时器初始化
   TimerA_IO_SET();           //定时器A
   zhua_SET();                //模拟pwm端口初始化
   init_uart0();
   int num=0;
   while(1)
   {
     SetRX_Mode();            //24l01设置为接收模式
     LED_0;                   //开始接收，指示灯熄灭
     if(nRF24L01_RxPacket(RxBuf))   //判断是否收到数据
      {
       temp[0]=RxBuf[0]*100;        //前后
       temp[1]=RxBuf[1]*100;        //左右
       temp[2]=RxBuf[2]*100;        //360舵机
       temp[3]=RxBuf[3]*100;        //光强
       temp[4]=RxBuf[4];           //180舵机标志位
       temp[5]=RxBuf[5]*100;        //
       temp[6]=RxBuf[6];
       temp[7]=RxBuf[7]*100;
       temp[8]=RxBuf[8]*100;
       LED_1;                   //接收完毕，指示灯亮起
       delay_ms(5);
      }
     temp[2]=filter_LV(temp[2],num);
     num++;
     if(num==40)
       num=0;
     TimerB_count();           //定时器数据值计算
     TimerB_do();              //定时器工作
    //printf(temp[1]);
   }  
}