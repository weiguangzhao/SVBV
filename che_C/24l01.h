//==============================================================================
#define  RF24L01_CE_0        P2OUT &=~BIT1         
#define  RF24L01_CE_1        P2OUT |= BIT1        
//=============================RF24L01_CSN�˿�==================================
#define  RF24L01_CSN_0       P2OUT &=~BIT3         
#define  RF24L01_CSN_1       P2OUT |= BIT3     
//=============================RF24L01_SCK======================================
#define  RF24L01_SCK_0       P2OUT &=~BIT2      
#define  RF24L01_SCK_1       P2OUT |= BIT2   
//=============================MISO�˿�=========================================
#define  RF24L01_MISO_0      P2OUT &=~BIT0
#define  RF24L01_MISO_1      P2OUT |= BIT0
//============================= RF24L01_MOSI�˿�================================
#define  RF24L01_MOSI_0      P1OUT &=~BIT6
#define  RF24L01_MOSI_1      P1OUT |= BIT6
//==========================IRQ״̬============================================
#define  RF24L01_IRQ_0       P1OUT &=~BIT7       
#define  RF24L01_IRQ_1       P1OUT |= BIT7
//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
//=========================NRF24L01�Ĵ���ָ��===================================
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP1            0xFF  	// ����
//========================SPI(nRF24L01)�Ĵ�����ַ===============================
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//=============================RF24l01״̬=====================================
char  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
char  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ
char  sta;
char  RxBuf[32];
int   temp[32];
//===========================RF24L01�˿�����==========================================
void RF24L01_IO_set(void)
{
      P1DIR &= 0x7f;    P1DIR |= 0x40;  P1SEL&=0x3F;   P1IE=P1IE&0x7f;
      P2DIR &= 0xFE;    P2DIR |= 0x0E;  P2SEL&=0xF0;
}
//========================================����ʱ================================
void Delay(int s)
{
	unsigned int i,j;
	for(i=0; i<s; i++);
	for(j=0; j<s; j++);
}            
//============================NRF24L01��SPIдʱ��==================================================
char SPI_RW(char data)
{	
        char i,temp=0;
   	for(i=0;i<8;i++) // output 8-bit
   	{
	if((data & 0x80)==0x80)
	{
		RF24L01_MOSI_1;         // output 'uchar', MSB to MOSI
	}
	else
	{
	 	RF24L01_MOSI_0; 
	}	
//==============================================================================
		data = (data << 1);            // shift next bit into MSB..
		temp<<=1;
		RF24L01_SCK_1;                // Set SCK high..
		if((P2IN&0x01)==0x01)temp++;         // capture current MISO bit
		RF24L01_SCK_0;              // ..then set SCK low again
   	}
    return(temp);           		  // return read uchar  		
}
//==========================NRF24L01��SPIʱ��====================================================
char SPI_Read(char reg)
{
	char reg_val;
	RF24L01_CSN_0;           // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	RF24L01_CSN_1;         // CSN high, terminate SPI communication
	return(reg_val);       // return register value
}
//==========================NRF24L01��д�Ĵ�������====================================================
char SPI_RW_Reg(char reg, char value)
{
	char status1;
	RF24L01_CSN_0;                   // CSN low, init SPI transaction
	status1 = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	RF24L01_CSN_1;                   // CSN high again
	return(status1);            // return nRF24L01 status uchar
}
//****************************************************************************************************/
//������uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
//����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
//****************************************************************************************************/
char SPI_Read_Buf(char reg, char *pBuf, char chars)
{
	char status2,uchar_ctr;
	RF24L01_CSN_0;                    		// Set CSN low, init SPI tranaction
	status2 = SPI_RW(reg);       		// Select register to write to and read status uchar
	for(uchar_ctr=0;uchar_ctr<chars;uchar_ctr++)
        {
	pBuf[uchar_ctr] = SPI_RW(0);    // 
        }
	RF24L01_CSN_1;                           
	return(status2);   
}
//*********************************************************************************************************
//������uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
//����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
//*********************************************************************************************************/
char SPI_Write_Buf(char reg, char *pBuf, char chars)
{
	char status1,uchar_ctr;
	RF24L01_CSN_0;             //SPIʹ��       
	status1 = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<chars; uchar_ctr++) //
        {
	SPI_RW(*pBuf++);
        }
	RF24L01_CSN_1;           //�ر�SPI
	return(status1);    		  // 
}
//****************************************************************************************************/
//������void SetRX_Mode(void)
//���ܣ����ݽ������� 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
        Delay(1);
	RF24L01_CE_1; 
	Delay(1000);//ע�ⲻ��̫С
}
//******************************************************************************************************/
//������unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//******************************************************************************************************/
char nRF24L01_RxPacket(char* rx_buf)
{
    char revale=0;
	sta=SPI_Read(STATUS);	     // ��ȡ״̬�Ĵ������ж����ݽ���״��		
	if(sta&0x40)                 // �ж��Ƿ���յ�����
	{
	    RF24L01_CE_0 ; 			//SPIʹ��
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;			//��ȡ������ɱ�־
	}
        else 
        {
              revale =0;	
        }
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}
//****************************************************************************************
//NRF24L01��ʼ��
//***************************************************************************************/
void init_NRF24L01(void)
{

 	RF24L01_CE_0 ;    // chip enable
 	RF24L01_CSN_1;   // Spi disable 
 	RF24L01_SCK_0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������}
        Delay(1000);
}