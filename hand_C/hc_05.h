//====================��������============================
char inf[36];                       //����������
int LR_AD=0,FB_AD=0;   //����,ǰ��
int S995_AD=0,Light_AD=0,zhua1_AD=0,zhua2_AD=2000;
char Re_buf2[6]={0};
int flag_1=0,flag_2=0,Data1=0,Data2=0;
int R_a=0,sign2=0,counter2=0;
//===============================hc_05��ʼ��=====================================
void init_uart1(void)
  {

   U1CTL|=SWRST;       // ��λ����
   P3SEL|=BIT7;                   //p3.7���ݽ���
   U1BR0=0x45;
   U1BR1=0x00;
   UMCTL1=0x4A;                       //115200 ������
   ME2|=URXE1;                       //ʹ�ܽ���
   UTCTL1|=SSEL1+SSEL0;             //ѡ��SMCLKΪʱ��Դ
   UCTL1|=CHAR;                    //8λ����λ
   UCTL1&=~(SWRST);               //��λ
   IE2|=URXIE1 ;                 // ʹ�ܽ����ж�  
  }
//===========================���ڽ����ж�==========================================
#pragma vector=UART1RX_VECTOR
__interrupt void UART1_RX(void)
      {
         R_a=U1RXBUF;
        Re_buf2[counter2]=R_a;                 
	if(counter2==0&&Re_buf2[0]!=0xff)
          return; //�� 0 �����ݲ���֡ͷ������
	counter2++;
	if(counter2==5) //���յ� 11 ������
	{
	        counter2=0; //���¸�ֵ��׼����һ֡���ݵĽ���
		sign2=1;                 
	} 
        if(sign2)
        {
          sign2=0;
          if((Re_buf2[1]==1)||(Re_buf2[1]==0))
          {
            if((Re_buf2[3]==1)||(Re_buf2[3]==0))
            {
              flag_1=Re_buf2[1];      //��ͷΪ1
              if((Re_buf2[2]!=255)&&(Re_buf2[2]!=0))
              Data1=Re_buf2[2];
              flag_2=Re_buf2[3];     //��תΪ1
              if((Re_buf2[4]!=255)&&(Re_buf2[4]!=0))
              Data2=Re_buf2[4];
            }
          }
        }
      }