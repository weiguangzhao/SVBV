#include <MSP430x16x.h>
#include "GY61.h"
//===========================��ʱ����===================================
#define CPU_F ((double)8000000)
#define delay_us(x)  __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))
//===========================��������==========================================
char Re_buf[11],sign;
//===========================ϵͳ��ʼ��============================
void InitSys()
{
   unsigned int iq0;
   _DINT();
   BCSCTL1 &=~XT2OFF;
   do
   {
      IFG1 &= ~OFIFG ;                   // �������ʧЧ��־
      for (iq0 = 0xFF; iq0 > 0; iq0--);	// ��ʱ���ȴ�XT2����
   }
   while ((IFG1 & OFIFG) != 0);		// �ж�XT2�Ƿ�����		
   BCSCTL2 =SELM1+SELS;                // MCLK,SMCLKʱ��ΪXT2		
}
//===============================���ڳ�ʼ��=====================================
void init_uart1(void)
  {

   U1CTL|=SWRST;       // ��λ����
   P3SEL|=BIT6;                   //p3.6���ݷ���
   P3DIR|=BIT6;        //    P3.6 λTX��
   U1BR0=0x45;
   U1BR1=0x00;
   UMCTL1=0x4A;                //115200 ������
   ME2|=UTXE1;                  //ʹ�ܷ���
   UTCTL1|=SSEL1+SSEL0;             //ѡ��SMCLKΪʱ��Դ
   UCTL1|=CHAR;                //8λ����λ
   UCTL1&=~(SWRST);            //��λ
   IE1|=URXIE1 ;             // ʹ�ܽ����ж�  
  }
//===========================���ڶ�д==========================================
void usart_write_char(char data)
{
	while((IFG2&UTXIFG1)==0); //�жϷ��ͻ������Ƿ����
	U1TXBUF=data;   
}
//================================������==========================================
int main()
{
     WDTCTL = WDTPW + WDTHOLD;     //�رտ��Ź���ʱ��
     InitSys();                   //ʱ�ӳ�ʼ��
     init_uart1();                //�������ڳ�ʼ�� 
     init_usart0();                //6050���ڳ�ʼ�� 
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