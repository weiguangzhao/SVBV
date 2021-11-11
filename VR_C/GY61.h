#define uchar unsigned char
#define uint  unsigned int
//===========================变量设置==========================================
char Re_buf[11],sign=0;
float a[3]={0},w[3]={0},angle[3]={0},T=0;  
int flag_1=0,flag_2=0,angle0=0,angle2=0,angle2_b=0,cha=0;                     
int Data1=0,Data2=0;
//===========================6050初始化==========================================
void init_usart0()
{
        U0CTL|=SWRST;       // 复位串口
        U0CTL|=CHAR;        //8位数据
        U0TCTL|=SSEL1;      //选择SMCLK作为 CLK
        U0BR0=0x45;
        U0BR1=0x00;
        UMCTL0=0x4A;       //115200 波特率
        ME1|=UTXE0+URXE0;   //使能接受和发送
        U0CTL&=~SWRST;      //  清除串口复位信号
        IE1|=URXIE0 ;       // 使能接受中断
        P3SEL|=BIT4;        //设置第二功能
        P3SEL|=BIT5;
        P3DIR|=BIT4;        //    P3.4 位TX口
        _EINT();
}
//===========================串口接收中断==========================================
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX(void)
      {
        static uchar counter=0;
        Re_buf[counter]=U0RXBUF;;//不同单片机略有差异
	if(counter==0&&Re_buf[0]!=0x55)
          return; //第 0 号数据不是帧头，跳过
	counter++;
	if(counter==11) //接收到 11 个数据
	{
		counter=0; //重新赋值，准备下一帧数据的接收
		sign=1;                 
	} 
       
      }
//===========================6050Data==========================================
void mpu6050_data()
{  
	if(sign)
  {  
    sign=0; 
     if(Re_buf[0]==0x55)      //检查帧头
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
      mpu6050_data();                 //计算角度值
      angle0=(int)angle[0];           //俯仰角
      angle2=(int)angle[2];           //旋转角     
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