#define uchar unsigned char
#define uint  unsigned int
//===========================��������==========================================
char Re_buf[11],sign=0;
float a[3]={0},w[3]={0},angle[3]={0},T=0;  
int flag_1=0,flag_2=0,angle0=0,angle2=0,angle2_b=0,cha=0;                     
int Data1=0,Data2=0;
//===========================6050��ʼ��==========================================
void init_usart0()
{
        U0CTL|=SWRST;       // ��λ����
        U0CTL|=CHAR;        //8λ����
        U0TCTL|=SSEL1;      //ѡ��SMCLK��Ϊ CLK
        U0BR0=0x45;
        U0BR1=0x00;
        UMCTL0=0x4A;       //115200 ������
        ME1|=UTXE0+URXE0;   //ʹ�ܽ��ܺͷ���
        U0CTL&=~SWRST;      //  ������ڸ�λ�ź�
        IE1|=URXIE0 ;       // ʹ�ܽ����ж�
        P3SEL|=BIT4;        //���õڶ�����
        P3SEL|=BIT5;
        P3DIR|=BIT4;        //    P3.4 λTX��
        _EINT();
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
void Gy61_do()
{
      angle2_b=angle2;
      mpu6050_data();                 //����Ƕ�ֵ
      angle0=(int)angle[0];           //������
      angle2=(int)angle[2];           //��ת��     
}
void angle_count()
{
  if(angle0>0)
  {
  flag_1=0;
  Data1=angle0;
  }
  else if(angle0<0)
  {
    flag_1=1;
    Data1=(0-angle0);
  }
  cha=angle2;
  if(cha>180||cha<-180||cha==0)
    Data2=0;
  else if(cha>0)
  {
    flag_2=0;
    Data2=cha;
  }
  else if(cha<0)
  {
    flag_2=1;
    Data2=0-cha;
  } 
}