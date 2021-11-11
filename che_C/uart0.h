//===============================串口初始化=====================================
void init_uart0(void)
  {
   U0CTL|=SWRST;       // 复位串口
   P3SEL|=BIT4;                   //p3.4数据发送
   P3DIR|=BIT4;        //    P3.4 位TX口
   U0BR0=0x45;
   U0BR1=0x00;
   UMCTL0=0x4A;                //115200 波特率
   ME1|=UTXE0;                  //使能发送
   UTCTL0|=SSEL1+SSEL0;             //选用SMCLK为时钟源
   UCTL0|=CHAR;                //8位数据位
   UCTL0&=~(SWRST);            //软复位
   IE1|=URXIE0 ;             // 使能接受中断  
  }
//===============================串口读写=====================================
void usart_write_char(char data)
{
	while((IFG1&UTXIFG0)==0); //判断发送缓冲区是否结束
	U0TXBUF=data;   
}
void usart_write_string(char *str)
{
	while(*str!='\0')
	{
		usart_write_char(*str);
		str++;
	}
}
void printf(int data)
{
	usart_write_char(data/10000%10+'0');
	usart_write_char(data/1000%10+'0');
	usart_write_char(data/100%10+'0');
	usart_write_char(data/10%10+'0');
	usart_write_char(data%10+'0');
 	usart_write_string("    ");
}