//========================模拟PWM=================================
#define zuo_0 P3OUT&=~BIT0             // 
#define zuo_1 P3OUT|=BIT0             //
#define you_0 P3OUT&=~BIT1             //
#define you_1 P3OUT|=BIT1             //
void zhua_SET()
{
  P3SEL&=~BIT0;                  //设置为普通IO口模式
  P3DIR|=BIT0;                  //设置为输出模式
  P3SEL&=~BIT1;                  //设置为普通IO口模式
  P3DIR|=BIT1;                  //设置为输出模式
  P3SEL&=~BIT2;                  //设置为普通IO口模式
  P3DIR|=BIT2;                  //设置为输出模式
}
//===========================变量设置===================================
int LR_AD=2000,FB_AD=2000,TB6560_AD=3000;          //左右、前后、步进电机AD值
int flag=0,light=4000;                  //舵机标志位
int k=0;
//===========================TimerB变量设置===================================
int R_timer=0,L_timer=0;    //前进左轮右轮计数值、后退左轮右轮计数值FL_timer=0,FR_timer=0,BL_timer=0,BR_timer=0,
int MG995_timer=3000;                                  //180舵机计数值
int MG995_timer2=2000,zhua_AD=4000;                                 //MG995舵机计数值(360)
void TimerA_IO_SET()
{
    P1SEL|=BIT0;           //P1.0设置为第二功能
    P1SEL|=BIT1;           //P1.1设置为第二功能
    P1SEL|=BIT2;           //P1.2设置为第二功能
    P1SEL|=BIT3;           //P1.3设置为第二功能
    P1DIR|=BIT0;           //P1.0设置为输出
    P1DIR|=BIT1;           //P1.1设置为输出 
    P1DIR|=BIT2;           //P1.2设置为输出 
    P1DIR|=BIT3;           //P1.3设置为输出
    TACTL=TASSEL_2+TACLR+MC_1+ID_2;
    CCTL1=OUTMOD_7;
    CCTL2=OUTMOD_7;
}
//===========================TimerB端口设置===================================
void TimerB_IO_SET()
{
  P4SEL|=BIT0;           //P4.0设置为第二功能
  P4SEL|=BIT1;           //P4.1设置为第二功能
  P4SEL|=BIT2;           //P4.2设置为第二功能
  P4SEL|=BIT3;           //P4.3设置为第二功能
  P4SEL|=BIT4;           //P4.4设置为第二功能
  P4SEL|=BIT5;           //P4.5设置为第二功能
  P4SEL|=BIT6;           //P4.6设置为第二功能
  P4DIR|=BIT0;           //P4.0设置为输出
  P4DIR|=BIT1;           //P4.1设置为输出
  P4DIR|=BIT2;           //P4.2设置为输出
  P4DIR|=BIT3;           //P4.3设置为输出
  P4DIR|=BIT4;           //P4.4设置为输出
  P4DIR|=BIT5;           //P4.5设置为输出
  P4DIR|=BIT6;           //P4.6设置为输出
} 
//===========================TimerB初始化===================================
void init_TimerB()
{
  TBCTL=TBSSEL_2+MC_1+ID_2;   //SMCLK子系统时钟、增计数模式、1/2分频
  TBCCTL1=OUTMOD_7;
  TBCCTL2=OUTMOD_7;
  TBCCTL3=OUTMOD_7;
  TBCCTL4=OUTMOD_7;
  TBCCTL5=OUTMOD_7;           
  TBCCTL6=OUTMOD_7;           //PWM复位置位模式
  TBCCR3=2000;
  TBCCR4=2000;
  CCR2=3000;
}

void TimerB_count()
{
  zhua_AD=2500+(temp[7]*3/8);
  MG995_timer=temp[5];        //舵机计数器值固定  1000~5000
  MG995_timer2=5000-temp[2];   // 1000~5000
  light=(4000-temp[3])*10;
  FB_AD=temp[0];
  LR_AD=temp[1];
  if(!temp[4])              //让机械臂行动
  {
   if(FB_AD<1800)        //2100  前进
  {
    FB_AD=2000-FB_AD;
    zuo_0;               //前进高电平使能
    you_0;
    L_timer=0;   
    R_timer=0;            //后退计数值为0 
    if(LR_AD<1800)        //右转
    {
      L_timer=FB_AD*10;    //左轮为右轮一半
      R_timer=FB_AD*20;
      
    }
    if(LR_AD>2200)
    {
      R_timer=FB_AD*10;   //右轮为轮左一半
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
    you_1;                  //后退低电平使能
              //前进计数值为0 
    if(LR_AD<1800)          //左转
    {
      R_timer=FB_AD*10;  //右轮为左轮一半
      L_timer=FB_AD*20;
    }
     if(LR_AD>2200)
    {
      R_timer=FB_AD*20;  //左轮为右轮一半
      L_timer=FB_AD*10;
    }
    if((LR_AD<=2200)&&(LR_AD>=1800))
    {
      L_timer=FB_AD*20;  //右轮、左轮同值
      R_timer=FB_AD*20;
    }
  }
   else
    {
      L_timer=0;  
      R_timer=0;          //前进退计数值为0 
      if(LR_AD<1800)
      {
        LR_AD=2000-LR_AD;
        R_timer=LR_AD*20;
        if((2000-LR_AD)>0)
          L_timer=(2000-LR_AD)*20;
        else L_timer=0;
        zuo_1;
        you_0;          //自旋
      }
      else if(LR_AD>2200)
      {
        LR_AD=LR_AD-2000;
        if((2000-LR_AD)>0)
          R_timer=(2000-LR_AD)*20;
        else R_timer=0;
        L_timer=LR_AD*20;
        zuo_0;
        you_1;          //自旋
      }
      else
      {
        L_timer=0;
        R_timer=0;
        zuo_0;
        you_0;              //静止
      }
    }
  } 
}
//===========================TimerB调速===================================
void TimerB_do()
{
  TBCCR0=40000;              //最大值40000复位
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
        
      
