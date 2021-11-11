#include <MSP430x16x.h>
#include "GY61.h"
//===========================延时函数===================================
#define CPU_F ((double)8000000)
#define delay_us(x)  __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))
//===========================变量设置==========================================
char Re_buf[11],sign;
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
//===============================串口初始化=====================================
void init_uart1(void)
  {

   U1CTL|=SWRST;       // 复位串口
   P3SEL|=BIT6;                   //p3.6数据发送
   P3DIR|=BIT6;        //    P3.6 位TX口
   U1BR0=0x45;
   U1BR1=0x00;
   UMCTL1=0x4A;                //115200 波特率
   ME2|=UTXE1;                  //使能发送
   UTCTL1|=SSEL1+SSEL0;             //选用SMCLK为时钟源
   UCTL1|=CHAR;                //8位数据位
   UCTL1&=~(SWRST);            //软复位
   IE1|=URXIE1 ;             // 使能接受中断  
  }
//===========================串口读写==========================================
void usart_write_char(char data)
{
	while((IFG2&UTXIFG1)==0); //判断发送缓冲区是否结束
	U1TXBUF=data;   
}
//================================主函数==========================================
int main()
{
     WDTCTL = WDTPW + WDTHOLD;     //关闭看门狗定时器
     InitSys();                   //时钟初始化
     init_uart1();                //蓝牙串口初始化 
     init_usart0();                //6050串口初始化 
     while(1)
     {
         Gy61_do();        
         angle_count();
         usart_write_char(0xff);
         delay_ms(5);
         usart_write_char(flag_1);
         delay_ms(5);
         usart_write_char(Data1);
         delay_ms(5);
         usart_write_char(flag_2);
         delay_ms(5);
         usart_write_char(Data2);
         delay_ms(5);
     }
             
}