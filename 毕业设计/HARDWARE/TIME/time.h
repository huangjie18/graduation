#ifndef __TIME_H
#define __TIME_H
#include "sys.h"

struct PID {
float Proportion; // �������� Proportional Const
float Integral; // ���ֳ��� Integral Const
float Derivative; // ΢�ֳ��� Derivative Const
float LastError; // Error[-1]
float PrevError; // Error[-2]
};


void Time_int_Init(u16 arr,u16 psc);
int Position_PID(int Encode,int Target);
void xianfu_Pwm(void);
void set_Pwm(int motol);
int myabs(int a);
extern int Encode;
int wucha(u16 adc);
void PID_init();
unsigned int PIDCalc( struct PID *pp, int NextPoint );
void PIDBEGIN1();
void jude(u16 Encode);
#endif
