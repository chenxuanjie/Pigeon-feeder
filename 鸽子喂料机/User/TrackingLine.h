#ifndef __TRACKINGLINE_H__
#define __TRACKINGLINE_H__

//鸽子机在一次循迹过程中的喂料次数
#define DIGITAL_FEEDINGTIMES	2

//循迹模式下的速度设置
#define TRACKINGLINE_BASESPEED		20/10	//一般的Set_Speed为20
#define TRACKINGLINE_TURNSPEED		20/5
#define TRACKINGLINE_SELFTURNSPEED	20/5	//自旋速度

/*************Digital_Mode*************/
#define DIGITAL_STRAIGHT	1	
#define DIGITAL_TURNLEFT	2	
#define DIGITAL_TURNRIGHT	3	
#define DIGITAL_FEEDING		4	
#define DIGITAL_STOP		5	

/*************路径规划*************/
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
//循迹模块从左到右5个引脚
#define DIGITAL_GPIO1	GPIOC
#define DIGITAL_1_PIN	GPIO_Pin_13
#define DIGITAL_2_PIN	GPIO_Pin_14
#define DIGITAL_3_PIN	GPIO_Pin_15
#define DIGITAL_GPIO2	GPIOA
#define DIGITAL_4_PIN	GPIO_Pin_0
#define DIGITAL_5_PIN	GPIO_Pin_8

void TrackingLine_Init(void);
uint8_t TrackingLine(void);
uint8_t TrackingLine_ReadDigital(uint16_t DigitalPin);
uint8_t Digital_ModeJudge(void);
uint8_t Digital_Straight(void);
uint8_t Digital_Turn(uint8_t Direction);
uint8_t Digital_Feeding(void);
uint8_t TrackingLine_IfExit(void);

#endif
