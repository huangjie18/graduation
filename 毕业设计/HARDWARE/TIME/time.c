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
//float SetSpeed; // �����趨ֵ
//float ActualSpeed; // ����ʵ��ֵ
//float err; // ����ƫ��ֵ
//float err_next; // ������һ��ƫ��ֵ
//float err_last; // ��������ǰ��ƫ��ֵ
//float Kp,Ki,Kd; // ������������֡�΢��ϵ��
//}pid;
/**************��ʼ��TIM3******************/
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
/****************TIM3�ж϶�PWM���д���**************/
void TIM3_IRQHandler(void)
{
	static u8 i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)     //��û�йرռ������ļ���������
	{											   //Ϊ�������ӽ����жϵĴ������Ӿ���
		if(i>100)                                
		{
			sta1 = 1;
			i=0;                    //Ҫ��ֵΪ0��Ҫ��Ȼһֱ�ر��ж�
			TIM_CtrlPWMOutputs(TIM8,DISABLE);     //�رո߼���ʱ����PWM���
			TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//�ر��ж�
			
			
		}
		else{
			Encode = Get_Adc_Average(ADC_Channel_5,5);    //adc�ɼ���ʵ��ֵ
			Moto1 = PIDCalc(&spid,Encode);
			xianfu_Pwm();		//�޷�
			jude(Encode);
			TIM_SetCompare1(TIM8,Moto2);//���ö�ʱ��8��pwmռ�ձ����
			TIM_SetCompare2(TIM8,Moto3);//���ö�ʱ��8��pwmռ�ձ����
			LED0 = !LED0 ;
		}
		
	}
	i++;
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


/*******************��ֵ��ͨ��1,2��ռ�ձ�************/
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

/***************��ֹռ�ձ�������Χ**************/
void xianfu_Pwm(void)
{
	int Amplitude=499;	//ռ�ձ�100%��ֵΪ500
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


/******************PID��ʼ��*****************/
void PIDInit (struct PID *pp) 
{ 
	spid.Derivative = 0;
	spid.Integral = 0;
	spid.LastError = 0;
	spid.PrevError = 0;
	spid.Proportion = 0;
	
} 


/****����pid*******/
unsigned int PIDCalc( struct PID *pp,  int NextPoint ) 
{ 
  unsigned int dError,Error,pError; 
	static float propo,inter;
	u8 b;
  //���������㹫ʽ��
  //Pdt=Kp*[E(t)-E(t-1)]+Ki*E(t)+Kd*[E(t)-2*E(t-1)+E(t-2)]
	if(NextPoint>target)
	{
		Error = NextPoint - target;
	}
	else
	{
		Error = target - NextPoint;       // ƫ��E(t)
	}
/**********************�ðɣ�����ԭ����û���¸�ֵ*****************************/
/******************��ô��֮ǰ���Ե���ʲô��������*****************************/
	
		if(statuspid)               //�Բ�ͬ�ĳ�ʼ���븳�費ͬ��pi��ֵ���ӿ��ʼ����С����Ӧʱ��
		{							//����һ��״̬λ��Ϊ���ǲ�������ֵ
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
				case 6: inter = 1.73;propo = 2.5;break;  //ԭʼ���ݿ��Կ��ٶ�����
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
            propo*pp->Proportion * pError        //���� 
            + inter*pp->Integral *Error  //������ 
            + pp->Derivative * dError	  // ΢����
			);
		}
		else
			return 0;
} 

/************����PID��ֵ,û���õ�D΢��*****************/
void PIDBEGIN1() 
{ 
  PIDInit(&spid); // Initialize Structure 
  spid.Proportion = 5; // Set PID Coefficients 
  spid.Integral = 0.8; 
  spid.Derivative =0; 
}



