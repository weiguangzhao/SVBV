#define uchar unsigned char
#define uint  unsigned int
#define LED_1 P1OUT|=BIT5
#define LED_0 P1OUT&=~BIT5
//===========================��������==========================================
char Re_buf[11],sign=0;
float a[3]={0},w[3]={0},angle[3]={0},T=0;  
int flag=0,angle1=0,GY61_pm=0;                     //�����־λ��������ֵ,���������ֵ
int mode=1;                        
//========================����IO������=================================
void TYPE_IO_SET()
{
  P1SEL&=~BIT4;   //����Ϊ��ͨIO��ģʽ
  P1DIR&=~BIT4;    //����Ϊ����ģʽ
  P1SEL&=~BIT3;   //����Ϊ��ͨIO��ģʽ
  P1DIR&=~BIT3;    //����Ϊ����ģʽ
}
void LED_IO_SET()
{
  P1SEL&=~BIT5;   //����Ϊ��ͨIO��ģʽ
  P1DIR|=BIT5;    //����Ϊ���ģʽ
  LED_0;
}
//===========================hand_6050��ʼ��==========================================
void init_usart0()
{
        U0CTL|=SWRST;       // ��λ����
        U0CTL|=CHAR;        //8λ����
        U0TCTL|=SSEL1;      //ѡ��SMCLK��Ϊ CLK
        U0BR0=0x45;
        U0BR1=0x00;
        UMCTL0=0x4A;          //115200 ������
        ME1|=URXE0;     //ʹ�ܽ��ܺͷ���
        U0CTL&=~SWRST;      //  ������ڸ�λ�ź�
        IE1|=URXIE0 ;       // ʹ�ܽ����ж�
        P3SEL|=BIT4;        //���õڶ�����
        P3SEL|=BIT5;
        P3DIR|=BIT4;        //    P3.4 λTX��
        _EINT();
}
//======================ģʽ�ж�=============================
void sel_mode()
{
  if(!(P1IN&BIT3))
  {
    while(!(P1IN&BIT3));
    if(mode==1)
    {
      init_uart1();              //�������ڳ�ʼ��
      IE1&=~URXIE0 ;       // ʹ�ܽ����ж�
      mode=0; 
      LED_1;
    }
    else 
    {
      init_usart0();           //GY61���ڳ�ʼ��
      IE2&=~URXIE1 ;       // ʹ�ܽ����ж�
      mode=1;
      LED_0;
    }
  }
  
}

//===========================���ڽ����ж�==========================================
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX(void)
      {
        static uchar counter=0;
        Re_buf[counter]=U0RXBUF;;//��ͬ��Ƭ�����в���
	if(counter==0&&Re_buf[0]!=0x55)
          return; //�� 0 �����ݲ���֡ͷ������
	counter++;
	if(counter==11) //���յ� 11 ������
	{
		counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
		sign=1;                 
	} 
       
      }
////===========================���ڽ������ݴ���==========================================
//void GY61_count()
//      {
//        while((IFG1&URXIFG0)==0); //�ж��Ƿ���ܵ�����
//        static uchar counter=0;
//        Re_buf[counter]=U0RXBUF;//��ͬ��Ƭ�����в���
//	if(counter==0&&Re_buf[0]!=0x55)
//          return; //�� 0 �����ݲ���֡ͷ������
//	counter++;
//	if(counter==11) //���յ� 11 ������
//	{
//		counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
//		sign=1;                 
//	} 
//       
//      }
//===========================6050Data==========================================
void mpu6050_data()
{  
	if(sign)
  {  
    sign=0; 
     if(Re_buf[0]==0x55)      //���֡ͷ
     {  
	 	switch(Re_buf [1])
		{
		case 0x51:
			 a[0] = ((float)(Re_buf [3]<<8| Re_buf[2]))/32768.0*16;
			 a[1] = ((float)(Re_buf [5]<<8| Re_buf[4]))/32768.0*16;
			 a[2] = ((float)(Re_buf [7]<<8| Re_buf[6]))/32768.0*16;
			 T = ((float)(Re_buf [9]<<8| Re_buf[8]))/340.0+36.25;
			 break;
		case 0x52:
			 w[0] = ((float)(Re_buf [3]<<8| Re_buf[2]))/32768.0*2000;
			 w[1] = ((float)(Re_buf [5]<<8| Re_buf[4]))/32768.0*2000;
			 w[2] = ((float)(Re_buf [7]<<8| Re_buf[6]))/32768.0*2000;
			 T = ((float)(Re_buf [9]<<8| Re_buf[8]))/340.0+36.25;
			 break;
		case 0x53:
        	        angle[0] = ((float)(Re_buf [3]<<8| Re_buf [2]))/32768.0*180;
			angle[1] = ((float)(Re_buf [5]<<8| Re_buf [4]))/32768.0*180;
			angle[2] = ((float)(Re_buf [7]<<8| Re_buf [6]))/32768.0*180;
			T = ((float)(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25;
        	break; 
		} 
    }    
 }
}
//===============================GY61_do===============
void Gy61_do()
{
  flag=0;
  if(!(P1IN&BIT4))     
    {
      flag=1;
      mpu6050_data();                 //����Ƕ�ֵ
      angle1=(int)angle[1];
      if(mode==1)                            //ģʽ�ж�
      {
      GY61_pm=3000+22*angle1;          //������������ֵ
      if(GY61_pm>5000) GY61_pm=5000;   //����
      if(GY61_pm<1000) GY61_pm=1000;   //����
      }
       if(mode==0)
       {
       zhua2_AD=3000+22*angle1;          //������������ֵ
       if(zhua2_AD>5000) zhua2_AD=5000;   //����
       if(zhua2_AD<1000) zhua2_AD=1000;   //����
       }
    }
  if(mode==0)
    {
      if(flag_1==0)
      GY61_pm=3000+22*Data1;  //������������ֵ
      else if(flag_1==1)
      GY61_pm=3000-22*Data1;  //������������ֵ 
      if(GY61_pm>5000) GY61_pm=5000;   //����
      if(GY61_pm<1000) GY61_pm=1000;   //����
      if(flag_2==0)
        S995_AD=2000+11*Data2;
      else if(flag_2==1)
        S995_AD=2000-11*Data2;
      if(S995_AD>4000) S995_AD=4000;   //����
      if(S995_AD<0) S995_AD=0;   //����
    }
    
}