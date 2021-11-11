
//================================AD转换初始化================================
void init_ADC12()
{
  P6SEL|=BIT0;                  //设置为外部引脚即AD0采样输入端
  P6SEL|=BIT1;                  //设置为外部引脚即AD1采样输入端
  P6SEL|=BIT2;                  //设置为外部引脚即AD2采样输入端
  P6SEL|=BIT3;                  //设置为外部引脚即AD2采样输入端
  ADC12CTL0=ADC12ON+SHT0_2+MSC;  //打开内部AD功能、256个采样保持时间、连续采样
  ADC12CTL1=SHP+CONSEQ_1;       //由采样信号上升沿触发、多通道单次模式
  ADC12MCTL0=INCH_0;           //设定AD0为信号输入端
  ADC12MCTL1=INCH_1;           //设定AD1为信号输入端
  ADC12MCTL2=INCH_2;           //设定AD2为信号输入端
  ADC12MCTL3=INCH_3+EOS;           //设定AD3为信号输入端
  ADC12CTL0|=ENC;             //允许AD转换
}
void AD_start_one()
{
  ADC12CTL0|=ADC12SC;        //开始一次转换
  while((ADC12IFG&BIT2)==0);//判断是否转换完成
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