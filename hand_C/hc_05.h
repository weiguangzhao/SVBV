//====================数组数据============================
char inf[36];                       //待发送数组
int LR_AD=0,FB_AD=0;   //左右,前后
int S995_AD=0,Light_AD=0,zhua1_AD=0,zhua2_AD=2000;
char Re_buf2[6]={0};
int flag_1=0,flag_2=0,Data1=0,Data2=0;
int R_a=0,sign2=0,counter2=0;
//===============================hc_05初始化=====================================
void init_uart1(void)
  {

   U1CTL|=SWRST;       // 复位串口
   P3SEL|=BIT7;                   //p3.7数据接收
   U1BR0=0x45;
   U1BR1=0x00;
   UMCTL1=0x4A;                       //115200 波特率
   ME2|=URXE1;                       //使能接受
   UTCTL1|=SSEL1+SSEL0;             //选用SMCLK为时钟源
   UCTL1|=CHAR;                    //8位数据位
   UCTL1&=~(SWRST);               //软复位
   IE2|=URXIE1 ;                 // 使能接受中断  
  }
//===========================串口接收中断==========================================
#pragma vector=UART1RX_VECTOR
__interrupt void UART1_RX(void)
      {
         R_a=U1RXBUF;
        Re_buf2[counter2]=R_a;                 
	if(counter2==0&&Re_buf2[0]!=0xff)
          return; //第 0 号数据不是帧头，跳过
	counter2++;
	if(counter2==5) //接收到 11 个数据
	{
	        counter2=0; //重新赋值，准备下一帧数据的接收
		sign2=1;                 
	} 
        if(sign2)
        {
          sign2=0;
          if((Re_buf2[1]==1)||(Re_buf2[1]==0))
          {
            if((Re_buf2[3]==1)||(Re_buf2[3]==0))
            {
              flag_1=Re_buf2[1];      //低头为1
              if((Re_buf2[2]!=255)&&(Re_buf2[2]!=0))
              Data1=Re_buf2[2];
              flag_2=Re_buf2[3];     //右转为1
              if((Re_buf2[4]!=255)&&(Re_buf2[4]!=0))
              Data2=Re_buf2[4];
            }
          }
        }
      }