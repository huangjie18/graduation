#include "pwm.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ��PWM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//TIM84 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;

/********************����ʹ��********************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
 
/**********************����ѡ��ʹ��**************/	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);

	
/******************IO�ڻ�����ʼ��******************/	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	
	
/************��ʱ����Ԫ����****************************/
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);
	
/******************��ʱ������ģʽѡ��*****************/
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Set;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	
	TIM_OC1Init(TIM8,&TIM_OCInitStruct);
	TIM_OC2Init(TIM8,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);

/**********************������ʼ��**********************/
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Enable;
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRInitStruct.TIM_DeadTime=0x2;
	TIM_BDTRInitStruct.TIM_LOCKLevel=TIM_LOCKLevel_OFF;
	TIM_BDTRInitStruct.TIM_OSSIState=TIM_OSSIState_Enable;
	TIM_BDTRInitStruct.TIM_OSSRState=TIM_OSSRState_Enable;
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);

/*************TIM8ʱ��ʹ�ܺ�PWMʹ��(TIM1��TIM8��Ҫ)******/
	
	TIM_Cmd(TIM8,ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);

										  
}  


