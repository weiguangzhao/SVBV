#include <msp430x14x.h>
#include "24l01.h"
#include "hc_05.h"
#include "GY61.h"
#include "AD.h"
//===========================��ʱ����===================================
#define CPU_F ((double)8000000)
#define delay_us(x)  __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))
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
//==========================������==============================================
void main()
{
  WDTCTL = WDTPW + WDTHOLD;  //��ֹ���Ź� 
  InitSys();                //��������Ϊ8Mz
  RF24L01_IO_set();        //24l01�˿�����
  init_NRF24L01() ;        //24l01��ʼ��
  init_ADC12();              //AD��ʼ��
  init_usart0();           //GY61���ڳ�ʼ��
  init_uart1();              //�������ڳ�ʼ��
  TYPE_IO_SET();           //������ʼ��
  LED_IO_SET();            //led��ʼ��
  while(1)
  { 
    sel_mode();                                 //ģʽ�ж�
    AD_start_one();                            //����һ��ADת��
    Gy61_do();                                //180�ȶ������  
    count_sent();                            //���ɷ�������
    nRF24L01_TxPacket(inf);                 //�����ݷ��ͳ�ȥ	
    SPI_RW_Reg(WRITE_REG+STATUS,0XFF);     //��״̬�Ĵ���
    delay_ms(5);
  }
}  