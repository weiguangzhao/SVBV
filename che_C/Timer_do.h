//========================ģ��PWM=================================
#define zuo_0 P3OUT&=~BIT0             // 
#define zuo_1 P3OUT|=BIT0             //
#define you_0 P3OUT&=~BIT1             //
#define you_1 P3OUT|=BIT1             //
void zhua_SET()
{
  P3SEL&=~BIT0;                  //����Ϊ��ͨIO��ģʽ
  P3DIR|=BIT0;                  //����Ϊ���ģʽ
  P3SEL&=~BIT1;                  //����Ϊ��ͨIO��ģʽ
  P3DIR|=BIT1;                  //����Ϊ���ģʽ
  P3SEL&=~BIT2;                  //����Ϊ��ͨIO��ģʽ
  P3DIR|=BIT2;                  //����Ϊ���ģʽ
}
//===========================��������===================================
int LR_AD=2000,FB_AD=2000,TB6560_AD=3000;          //���ҡ�ǰ�󡢲������ADֵ
int flag=0,light=4000;                  //�����־λ
int k=0;
//===========================TimerB��������===================================
int R_timer=0,L_timer=0;    //ǰ���������ּ���ֵ�������������ּ���ֵFL_timer=0,FR_timer=0,BL_timer=0,BR_timer=0,
int MG995_timer=3000;                                  //180�������ֵ
int MG995_timer2=2000,zhua_AD=4000;                                 //MG995�������ֵ(360)
void TimerA_IO_SET()
{
    P1SEL|=BIT0;           //P1.0����Ϊ�ڶ�����
    P1SEL|=BIT1;           //P1.1����Ϊ�ڶ�����
    P1SEL|=BIT2;           //P1.2����Ϊ�ڶ�����
    P1SEL|=BIT3;           //P1.3����Ϊ�ڶ�����
    P1DIR|=BIT0;           //P1.0����Ϊ���
    P1DIR|=BIT1;           //P1.1����Ϊ��� 
    P1DIR|=BIT2;           //P1.2����Ϊ��� 
    P1DIR|=BIT3;           //P1.3����Ϊ���
    TACTL=TASSEL_2+TACLR+MC_1+ID_2;
    CCTL1=OUTMOD_7;
    CCTL2=OUTMOD_7;
}
//===========================TimerB�˿�����===================================
void TimerB_IO_SET()
{
  P4SEL|=BIT0;           //P4.0����Ϊ�ڶ�����
  P4SEL|=BIT1;           //P4.1����Ϊ�ڶ�����
  P4SEL|=BIT2;           //P4.2����Ϊ�ڶ�����
  P4SEL|=BIT3;           //P4.3����Ϊ�ڶ�����
  P4SEL|=BIT4;           //P4.4����Ϊ�ڶ�����
  P4SEL|=BIT5;           //P4.5����Ϊ�ڶ�����
  P4SEL|=BIT6;           //P4.6����Ϊ�ڶ�����
  P4DIR|=BIT0;           //P4.0����Ϊ���
  P4DIR|=BIT1;           //P4.1����Ϊ���
  P4DIR|=BIT2;           //P4.2����Ϊ���
  P4DIR|=BIT3;           //P4.3����Ϊ���
  P4DIR|=BIT4;           //P4.4����Ϊ���
  P4DIR|=BIT5;           //P4.5����Ϊ���
  P4DIR|=BIT6;           //P4.6����Ϊ���
} 
//===========================TimerB��ʼ��===================================
void init_TimerB()
{
  TBCTL=TBSSEL_2+MC_1+ID_2;   //SMCLK��ϵͳʱ�ӡ�������ģʽ��1/2��Ƶ
  TBCCTL1=OUTMOD_7;
  TBCCTL2=OUTMOD_7;
  TBCCTL3=OUTMOD_7;
  TBCCTL4=OUTMOD_7;
  TBCCTL5=OUTMOD_7;           
  TBCCTL6=OUTMOD_7;           //PWM��λ��λģʽ
  TBCCR3=2000;
  TBCCR4=2000;
  CCR2=3000;
}

void TimerB_count()
{
  zhua_AD=2500+(temp[7]*3/8);
  MG995_timer=temp[5];        //���������ֵ�̶�  1000~5000
  MG995_timer2=5000-temp[2];   // 1000~5000
  light=(4000-temp[3])*10;
  FB_AD=temp[0];
  LR_AD=temp[1];
  if(!temp[4])              //�û�е���ж�
  {
   if(FB_AD<1800)        //2100  ǰ��
  {
    FB_AD=2000-FB_AD;
    zuo_0;               //ǰ���ߵ�ƽʹ��
    you_0;
    L_timer=0;   
    R_timer=0;            //���˼���ֵΪ0 
    if(LR_AD<1800)        //��ת
    {
      L_timer=FB_AD*10;    //����Ϊ����һ��
      R_timer=FB_AD*20;
      
    }
    if(LR_AD>2200)
    {
      R_timer=FB_AD*10;   //����Ϊ����һ��
      L_timer=FB_AD*20;
    }
    if((LR_AD<=2200)&&(LR_AD>=1800))
    {
      L_timer=20*FB_AD;
      R_timer=20*FB_AD;     
    }
  }
  else if(FB_AD>2200)     
  {
    FB_AD=4000-FB_AD;
    zuo_1;
    you_1;                  //���˵͵�ƽʹ��
              //ǰ������ֵΪ0 
    if(LR_AD<1800)          //��ת
    {
      R_timer=FB_AD*10;  //����Ϊ����һ��
      L_timer=FB_AD*20;
    }
     if(LR_AD>2200)
    {
      R_timer=FB_AD*20;  //����Ϊ����һ��
      L_timer=FB_AD*10;
    }
    if((LR_AD<=2200)&&(LR_AD>=1800))
    {
      L_timer=FB_AD*20;  //���֡�����ֵͬ
      R_timer=FB_AD*20;
    }
  }
   else
    {
      L_timer=0;  
      R_timer=0;          //ǰ���˼���ֵΪ0 
      if(LR_AD<1800)
      {
        LR_AD=2000-LR_AD;
        R_timer=LR_AD*20;
        if((2000-LR_AD)>0)
          L_timer=(2000-LR_AD)*20;
        else L_timer=0;
        zuo_1;
        you_0;          //����
      }
      else if(LR_AD>2200)
      {
        LR_AD=LR_AD-2000;
        if((2000-LR_AD)>0)
          R_timer=(2000-LR_AD)*20;
        else R_timer=0;
        L_timer=LR_AD*20;
        zuo_0;
        you_1;          //����
      }
      else
      {
        L_timer=0;
        R_timer=0;
        zuo_0;
        you_0;              //��ֹ
      }
    }
  } 
}
//===========================TimerB����===================================
void TimerB_do()
{
  TBCCR0=40000;              //���ֵ40000��λ
  TBCCR1=L_timer;
  TBCCR2=R_timer;
  TBCCR5=MG995_timer;
  TBCCR6=MG995_timer2;
  CCR0=40000;
  CCR1=light;
 if(k==0)
 {
  TBCCR3=2500;
  TBCCR4=2500;
  CCR2=4000;
  }
 k=1;
 if((temp[6]==0))
    CCR2=zhua_AD;
      else  CCR2=CCR2;
 if((temp[6]==0))
  if(temp[4]==1)
  {
    TBCCR3=TBCCR3+(2000-temp[1])/400;
    TBCCR4=TBCCR4+(2000-temp[0])/400;
    if (TBCCR3<=1000)  TBCCR3=1000;   //protect
    if (TBCCR4<=2000)  TBCCR4=2000;
    if (TBCCR3>=5000)  TBCCR3=5000;
    if (TBCCR4>=4000)  TBCCR4=4000;
   }
   else 
   {
     TBCCR3=TBCCR3;
     TBCCR4=TBCCR4;
   }
 
 }
        
      
