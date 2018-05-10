#include "time.h"
#include "led.h"
#include "adc.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"

 u8 statuspid=1;
extern u16 target;
struct PID spid; // PID Control Structure
int Moto1;
int Moto2;
int Moto3;
extern 	u8 sta1;
//struct _pid{
//float SetSpeed; // 定义设定值
//float ActualSpeed; // 定义实际值
//float err; // 定义偏差值
//float err_next; // 定义上一个偏差值
//float err_last; // 定义最上前的偏差值
//float Kp,Ki,Kd; // 定义比例、积分、微分系数
//}pid;
/**************初始化TIM3******************/
void Time_int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x03;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM3,ENABLE);
}
/****************TIM3中断对PWM进行处理**************/
void TIM3_IRQHandler(void)
{
	static u8 i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)     //并没有关闭计数器的计数和清零
	{											   //为的是增加进入中断的次数增加精度
		if(i>100)                                
		{
			sta1 = 1;
			i=0;                    //要赋值为0，要不然一直关闭中断
			TIM_CtrlPWMOutputs(TIM8,DISABLE);     //关闭高级定时器的PWM输出
			TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//关闭中断
			
			
		}
		else{
			Encode = Get_Adc_Average(ADC_Channel_5,5);    //adc采集给实际值
			Moto1 = PIDCalc(&spid,Encode);
			xianfu_Pwm();		//限幅
			jude(Encode);
			TIM_SetCompare1(TIM8,Moto2);//设置定时器8的pwm占空比输出
			TIM_SetCompare2(TIM8,Moto3);//设置定时器8的pwm占空比输出
			LED0 = !LED0 ;
		}
		
	}
	i++;
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


/*******************赋值给通道1,2的占空比************/
void jude(u16 Encode)
{
	if(Encode<target)
	{
		Moto2 = Moto1;
		Moto3 = 0;
	}
	else if(Encode>target)
	{
		Moto3 = Moto1;
		Moto2 = 0;
	}
}

/***************防止占空比数超范围**************/
void xianfu_Pwm(void)
{
	int Amplitude=499;	//占空比100%的值为500
	if(Moto1<-Amplitude) Moto1 = Amplitude;
	if(Moto1>Amplitude) Moto1=Amplitude;
}

int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=0;  
	  else temp=a;
	  return temp;
}


/******************PID初始化*****************/
void PIDInit (struct PID *pp) 
{ 
	spid.Derivative = 0;
	spid.Integral = 0;
	spid.LastError = 0;
	spid.PrevError = 0;
	spid.Proportion = 0;
	
} 


/****增量pid*******/
unsigned int PIDCalc( struct PID *pp,  int NextPoint ) 
{ 
  unsigned int dError,Error,pError; 
	static float propo,inter;
	u8 b;
  //增量法计算公式：
  //Pdt=Kp*[E(t)-E(t-1)]+Ki*E(t)+Kd*[E(t)-2*E(t-1)+E(t-2)]
	if(NextPoint>target)
	{
		Error = NextPoint - target;
	}
	else
	{
		Error = target - NextPoint;       // 偏差E(t)
	}
/**********************好吧，数据原来都没重新赋值*****************************/
/******************那么我之前调试的是什么。。。。*****************************/
	
		if(statuspid)               //对不同的初始距离赋予不同的pi初值，加快初始距离小的响应时间
		{							//增加一个状态位，为的是不连续赋值
			if(10<Error&&Error<=50) b = 1;
			else if(150<Error&&Error<=250) b = 2;
			else if(250<Error&&Error<=350) b = 3;
			else if(350<Error&&Error<=450) b = 4;
			else if(450<Error&&Error<=550) b = 5;
			else if(550<Error&&Error<=650) b = 6;
			else if(650<Error&&Error<=750) b = 7;
			else if(750<Error&&Error<=1150) b = 8;
			else if(1150<Error&&Error<=1450) b = 9;
			else if(1450<Error&&Error<=1550) b = 10;
			else if(1550<Error&&Error<=1750) b = 11;
			else if(1750<Error&&Error<=1950) b = 12;
			else if(1950<Error&&Error<=2450) b = 16;
			else if(2450<Error&&Error<=3050) b = 13;
			else if(3050<Error) b = 14;
			else if(50<Error&&Error<=100)  b = 15;
			else if(100<Error&&Error<=150) b = 17;
			switch(b)
			{
				case 1: inter = 3;propo = 3;break;
				case 2: inter = 2.7;propo = 3;break;
				case 3: inter = 2.55;propo = 3;break;
				case 4: inter = 2.2;propo = 2.5;break;
				case 5: inter = 2;propo = 2.5;break;
				case 6: inter = 1.73;propo = 2.5;break;  //原始数据可以看百度云盘
				case 7: inter = 1.6;propo = 2.5;break;
				case 8: inter = 1.4;propo = 2.5;break;
				case 9: inter = 1.2;propo = 2.5;break;
				case 10: inter = 1.19;propo = 2.5;break;
				case 11: inter = 1.18;propo = 2.5;break;
				case 12: inter = 1.18;propo = 2.5;break;
				case 13: inter = 1.18;propo = 2.5;break;
				case 14: inter = 1.1;propo = 2.8;break;
				case 15: inter = 2.9;propo = 3;break;
				case 16: inter = 1.18;propo = 2.5;break;
				case 17: inter = 2.8;propo = 2.5;break;
			}
			statuspid = 0;
		}
  pError=Error - pp->LastError;	 //E(t)-E(t-1)
  dError=Error-2*pp->LastError+pp->PrevError; //E(t)-2*E(t-1)+E(t-2) 
  pp->PrevError = pp->LastError; 
  pp->LastError = Error;
		if(Error>10)
		{
			return ( 
            propo*pp->Proportion * pError        //比例 
            + inter*pp->Integral *Error  //积分项 
            + pp->Derivative * dError	  // 微分项
			);
		}
		else
			return 0;
} 

/************定义PID的值,没有用到D微分*****************/
void PIDBEGIN1() 
{ 
  PIDInit(&spid); // Initialize Structure 
  spid.Proportion = 5; // Set PID Coefficients 
  spid.Integral = 0.8; 
  spid.Derivative =0; 
}



