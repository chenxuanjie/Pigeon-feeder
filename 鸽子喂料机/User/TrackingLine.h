#ifndef __TRACKINGLINE_H__
#define __TRACKINGLINE_H__

#include "main.h"

//С���Ӽ�������Ҫ��ʱ��.��λ��100ms(��ʱʧЧ)
#define ADDSPEEDTIME			20
#define TRACKINGLINE_STOPTIME	500	//С��ÿ��ת��ǰֹͣ��ʱ��
#define TRACKINGLINE_RUNTIME	1500	//С��ÿ��ת��ǰ��ֹͣ������ǰֱ�е�ʱ��
//���ӻ���һ��ѭ�������е�ι�ϴ���
#define DIGITAL_FEEDINGTIMES	2

//ѭ��ģʽ�µ��ٶ�����
#define TRACKINGLINE_BASESPEED		20/10 * (Robot_GetSpeedLevel()+1)	//һ���Set_SpeedΪ20
#define TRACKINGLINE_FRONTSPEED		TRACKINGLINE_BASESPEED *4	//��ǰ���ߵ��ٶ�
#define TRACKINGLINE_TURNSPEED		TRACKINGLINE_BASESPEED *4
#define TRACKINGLINE_SELFTURNSPEED	TRACKINGLINE_BASESPEED *2	//�����ٶ�

/*************Digital_Mode*************/
#define DIGITAL_STRAIGHT	1	
#define DIGITAL_TURNLEFT	2	
#define DIGITAL_TURNRIGHT	3	
#define DIGITAL_FEEDING		4	
#define DIGITAL_STOP		5	

/*************·���滮*************/
#define LEFT		0
#define RIGHT		1
#define NI	 	0
#define SHUN 	1

#define Digital_L2		TrackingLine_ReadDigital(DIGITAL_1_PIN)	
#define Digital_L1		TrackingLine_ReadDigital(DIGITAL_2_PIN)	
#define Digital_M		TrackingLine_ReadDigital(DIGITAL_3_PIN)	
#define Digital_R1		TrackingLine_ReadDigital(DIGITAL_4_PIN)	
#define Digital_R2		TrackingLine_ReadDigital(DIGITAL_5_PIN)	

#define DIGITAL_APBPEIRPH		RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
//ѭ��ģ�������5������
#define DIGITAL_GPIO1	GPIOC
#define DIGITAL_1_PIN	GPIO_Pin_13
#define DIGITAL_2_PIN	GPIO_Pin_14
#define DIGITAL_3_PIN	GPIO_Pin_15
#define DIGITAL_GPIO2	GPIOA
#define DIGITAL_4_PIN	GPIO_Pin_0
#define DIGITAL_5_PIN	GPIO_Pin_8

void TrackingLine_Init(void);
uint8_t TrackingLine(machine* machine1, machine* machine2, machine* machine3);
uint8_t TrackingLine_ReadDigital(uint16_t DigitalPin);
uint8_t Digital_ModeJudge(uint8_t TrackingLine_FeedFlag);
uint8_t Digital_Straight(void);
uint8_t Digital_Straight2(void);
uint8_t Digital_Turn(uint8_t Direction);
uint8_t Digital_Turn2(uint8_t Direction);
uint8_t Digital_Feeding(machine* machine1, machine* machine2, machine* machine3, uint8_t* TrackingLine_FeedFlag);
uint8_t TrackingLine_IfExit(void);
uint8_t TrackingLine_Test(void);

#endif
