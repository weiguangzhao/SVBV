//===============================���ڳ�ʼ��=====================================
void init_uart0(void)
  {
   U0CTL|=SWRST;       // ��λ����
   P3SEL|=BIT4;                   //p3.4���ݷ���
   P3DIR|=BIT4;        //    P3.4 λTX��
   U0BR0=0x45;
   U0BR1=0x00;
   UMCTL0=0x4A;                //115200 ������
   ME1|=UTXE0;                  //ʹ�ܷ���
   UTCTL0|=SSEL1+SSEL0;             //ѡ��SMCLKΪʱ��Դ
   UCTL0|=CHAR;                //8λ����λ
   UCTL0&=~(SWRST);            //��λ
   IE1|=URXIE0 ;             // ʹ�ܽ����ж�  
  }
//===============================���ڶ�д=====================================
void usart_write_char(char data)
{
	while((IFG1&UTXIFG0)==0); //�жϷ��ͻ������Ƿ����
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