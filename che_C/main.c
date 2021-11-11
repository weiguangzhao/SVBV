#include <msp430x14x.h>              //3.1   6.6    6.7   4.4
#include "24l01.h"
#include "uart0.h"
#include "Timer_do.h"
#include "filter_z.h"
//===========================��ʱ����===================================
#define CPU_F ((double)8000000)
#define delay_us(x)  __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))
//========================LED����ָʾ��=================================
#define LED_0 P3OUT&=~BIT3             //ָʾ��Ϩ��
#define LED_1 P3OUT|=BIT3             //ָʾ������
void LED_IO_SET()
{
  P3SEL&=~BIT3;                  //����Ϊ��ͨIO��ģʽ
  P3DIR|=BIT3;                  //����Ϊ���ģʽ
}

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
//===========================������============================
void main()
{
   WDTCTL = WDTPW + WDTHOLD; //��ֹ���Ź�
   InitSys();               //���þ���Ϊ8Mz
   RF24L01_IO_set();         //24l01IO����
   init_NRF24L01() ;         //24l01��ʼ��
   LED_IO_SET();             //����ָʾ��IO������
   TimerB_IO_SET();          //��ʱ���˿�����
   init_TimerB();            //��ʱ����ʼ��
   TimerA_IO_SET();           //��ʱ��A
   zhua_SET();                //ģ��pwm�˿ڳ�ʼ��
   init_uart0();
   int num=0;
   while(1)
   {
     SetRX_Mode();            //24l01����Ϊ����ģʽ
     LED_0;                   //��ʼ���գ�ָʾ��Ϩ��
     if(nRF24L01_RxPacket(RxBuf))   //�ж��Ƿ��յ�����
      {
       temp[0]=RxBuf[0]*100;        //ǰ��
       temp[1]=RxBuf[1]*100;        //����
       temp[2]=RxBuf[2]*100;        //360���
       temp[3]=RxBuf[3]*100;        //��ǿ
       temp[4]=RxBuf[4];           //180�����־λ
       temp[5]=RxBuf[5]*100;        //
       temp[6]=RxBuf[6];
       temp[7]=RxBuf[7]*100;
       temp[8]=RxBuf[8]*100;
       LED_1;                   //������ϣ�ָʾ������
       delay_ms(5);
      }
     temp[2]=filter_LV(temp[2],num);
     num++;
     if(num==40)
       num=0;
     TimerB_count();           //��ʱ������ֵ����
     TimerB_do();              //��ʱ������
    //printf(temp[1]);
   }  
}