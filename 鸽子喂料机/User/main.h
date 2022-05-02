#ifndef __MAIN_H__
#define __MAIN_H__


//发送的第n个数组
#define NORMAL_TRANSMIT			0
#define KEY_TRANSMIT			1
#define SWITCH_TRANSMIT			2
#define ROCKER_TRANSMIT			3
#define ENCODER_TRANSMIT		4	//接收值为遥控器上的值
#define FEEDTIME1_TRANSMIT		5
#define FEEDTIME2_TRANSMIT		6
#define FEEDTIME3_TRANSMIT		7
//Nrmal_Transmit
#define FEED_OFF				(0x0 + 1)
#define FEED_ON					(0x1 + 1)
#define CHECK					(0xFE)

//按键 KEY_TRANSMIT
#define KEY_ROCKER_TRANSMIT		0x1
#define KEY_PIN1_NUM			0x1
#define KEY_PIN2_NUM			(0x1 << 1)
#define KEY_PIN3_NUM			(0x1 << 2)
#define KEY_PIN4_NUM			(0x1 << 3)
#define KEY_PIN5_NUM			(0x1 << 4)
#define KEY_PIN6_NUM			(0x1 << 5)
#define KEY_PIN7_NUM			(0x1 << 6)
#define EmergencyFault		0X00
//钮子开关 SWITCH_TRANSMIT
#define SWITCH2_PIN1_NUM		0x1			//二档钮子开关，
#define SWITCH3_PIN1_NUM		(0x1 << 1)	//三档钮子开关，
#define SWITCH3_PIN2_NUM		(0x1 << 2)	//三档钮子开关，
#define SWITCH3_NONE			(0x1 << 3)	//三档钮子开关，中间

//摇杆 ROCKER_TRANSMIT
#define UP_1					0x1		//1,2,3,4表示程度。1为最小，5最大。
#define UP_2					0x2
#define UP_3					0x3
#define UP_4					0x4
#define DOWN_1					0x5
#define DOWN_2					0x6
#define DOWN_3					0x7
#define DOWN_4					0x8
#define LEFT_1					0x1
#define LEFT_2					0x2
#define LEFT_3					0x3
#define LEFT_4					0x4
#define RIGHT_1					0x5
#define RIGHT_2					0x6
#define RIGHT_3					0x7
#define RIGHT_4					0x8
#define MEDIWM					0x99

typedef enum{
	SETTINGSTATE = 1,
	CONTROLSTATE = 2,
	DEBUGSTATE = 3
}RemoteState;

#endif
