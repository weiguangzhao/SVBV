
//================================ADת����ʼ��================================
void init_ADC12()
{
  P6SEL|=BIT0;                  //����Ϊ�ⲿ���ż�AD0���������
  P6SEL|=BIT1;                  //����Ϊ�ⲿ���ż�AD1���������
  P6SEL|=BIT2;                  //����Ϊ�ⲿ���ż�AD2���������
  P6SEL|=BIT3;                  //����Ϊ�ⲿ���ż�AD2���������
  ADC12CTL0=ADC12ON+SHT0_2+MSC;  //���ڲ�AD���ܡ�256����������ʱ�䡢��������
  ADC12CTL1=SHP+CONSEQ_1;       //�ɲ����ź������ش�������ͨ������ģʽ
  ADC12MCTL0=INCH_0;           //�趨AD0Ϊ�ź������
  ADC12MCTL1=INCH_1;           //�趨AD1Ϊ�ź������
  ADC12MCTL2=INCH_2;           //�趨AD2Ϊ�ź������
  ADC12MCTL3=INCH_3+EOS;           //�趨AD3Ϊ�ź������
  ADC12CTL0|=ENC;             //����ADת��
}
void AD_start_one()
{
  ADC12CTL0|=ADC12SC;        //��ʼһ��ת��
  while((ADC12IFG&BIT2)==0);//�ж��Ƿ�ת�����
  LR_AD=ADC12MEM0;
  FB_AD=ADC12MEM1;
  S995_AD=ADC12MEM2;
  zhua1_AD=S995_AD;
  Light_AD=ADC12MEM3;
}
void count_sent()
{

  inf[0]=LR_AD/100;
  inf[1]=FB_AD/100;
  inf[2]=S995_AD/100;
  inf[3]=Light_AD/100;
  inf[4]=flag;
  inf[5]=GY61_pm/100;
  inf[6]=mode;
  inf[7]=zhua1_AD/100;
  inf[8]=zhua2_AD/100;
}  