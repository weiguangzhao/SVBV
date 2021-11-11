#define uchar unsigned char
#define uint  unsigned int
#define LED_1 P1OUT|=BIT5
#define LED_0 P1OUT&=~BIT5
//===========================变量设置==========================================
char Re_buf[11],sign=0;
float a[3]={0},w[3]={0},angle[3]={0},T=0;  
int flag=0,angle1=0,GY61_pm=0;                     //舵机标志位，俯仰角值,舵机计数器值
int mode=1;                        
//========================按键IO口设置=================================
void TYPE_IO_SET()
{
  P1SEL&=~BIT4;   //设置为普通IO口模式
  P1DIR&=~BIT4;    //设置为输入模式
  P1SEL&=~BIT3;   //设置为普通IO口模式
  P1DIR&=~BIT3;    //设置为输入模式
}
void LED_IO_SET()
{
  P1SEL&=~BIT5;   //设置为普通IO口模式
  P1DIR|=BIT5;    //设置为输出模式
  LED_0;
}
//===========================hand_6050初始化==========================================
void init_usart0()
{
        U0CTL|=SWRST;       // 复位串口
        U0CTL|=CHAR;        //8位数据
        U0TCTL|=SSEL1;      //选择SMCLK作为 CLK
        U0BR0=0x45;
        U0BR1=0x00;
        UMCTL0=0x4A;          //115200 波特率
        ME1|=URXE0;     //使能接受和发送
        U0CTL&=~SWRST;      //  清除串口复位信号
        IE1|=URXIE0 ;       // 使能接受中断
        P3SEL|=BIT4;        //设置第二功能
        P3SEL|=BIT5;
        P3DIR|=BIT4;        //    P3.4 位TX口
        _EINT();
}
//======================模式判断=============================
void sel_mode()
{
  if(!(P1IN&BIT3))
  {
    while(!(P1IN&BIT3));
    if(mode==1)
    {
      init_uart1();              //蓝牙串口初始化
      IE1&=~URXIE0 ;       // 使能接受中断
      mode=0; 
      LED_1;
    }
    else 
    {
      init_usart0();           //GY61串口初始化
      IE2&=~URXIE1 ;       // 使能接受中断
      mode=1;
      LED_0;
    }
  }
  
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
////===========================串口接收数据处理==========================================
//void GY61_count()
//      {
//        while((IFG1&URXIFG0)==0); //判断是否接受到数据
//        static uchar counter=0;
//        Re_buf[counter]=U0RXBUF;//不同单片机略有差异
//	if(counter==0&&Re_buf[0]!=0x55)
//          return; //第 0 号数据不是帧头，跳过
//	counter++;
//	if(counter==11) //接收到 11 个数据
//	{
//		counter=0; //重新赋值，准备下一帧数据的接收
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
//===============================GY61_do===============
void Gy61_do()
{
  flag=0;
  if(!(P1IN&BIT4))     
    {
      flag=1;
      mpu6050_data();                 //计算角度值
      angle1=(int)angle[1];
      if(mode==1)                            //模式判断
      {
      GY61_pm=3000+22*angle1;          //计算舵机计数器值
      if(GY61_pm>5000) GY61_pm=5000;   //保护
      if(GY61_pm<1000) GY61_pm=1000;   //保护
      }
       if(mode==0)
       {
       zhua2_AD=3000+22*angle1;          //计算舵机计数器值
       if(zhua2_AD>5000) zhua2_AD=5000;   //保护
       if(zhua2_AD<1000) zhua2_AD=1000;   //保护
       }
    }
  if(mode==0)
    {
      if(flag_1==0)
      GY61_pm=3000+22*Data1;  //计算舵机计数器值
      else if(flag_1==1)
      GY61_pm=3000-22*Data1;  //计算舵机计数器值 
      if(GY61_pm>5000) GY61_pm=5000;   //保护
      if(GY61_pm<1000) GY61_pm=1000;   //保护
      if(flag_2==0)
        S995_AD=2000+11*Data2;
      else if(flag_2==1)
        S995_AD=2000-11*Data2;
      if(S995_AD>4000) S995_AD=4000;   //保护
      if(S995_AD<0) S995_AD=0;   //保护
    }
    
}