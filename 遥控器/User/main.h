#ifndef __MAIN_H__
#define __MAIN_H__

#define ORINGINAL_VOLTAGE		ADC_ConvertedValue[2]

//发送的第n个数组
#define NORMAL_TRANSMIT			0
#define KEY_TRANSMIT			1
#define SWITCH_TRANSMIT			2
#define ROCKER_TRANSMIT			3
#define ENCODER_TRANSMIT		4
#define FEEDTIME1_TRANSMIT		5
#define FEEDTIME2_TRANSMIT		6
#define FEEDTIME3_TRANSMIT		7

//Nrmal_Transmit
#define FEED_OFF				(0x0 + 1)
#define FEED_ON					(0x1 + 1)
#define AUTO_FEED				(0x0 + 3)
#define CHECK					(0xFE)
//按键 KEY_TRANSMIT
#define KEY_ROCKER_TRANSMIT			0x1
#define KEY_UP					KEY_PIN4_NUM
#define KEY_DOWN				KEY_PIN5_NUM
#define KEY_LEFT				KEY_PIN6_NUM
#define KEY_RIGHT				KEY_PIN7_NUM

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

#define MAX_MODE				4
#define STORENUM				6

typedef enum{
	Feed_ON = 1,
	Feed_OFF = 2,
	Feed_Every = 3,
	Feed_All = 4
}Feed;

typedef enum{
	Remote_UP = 1,
	Remote_DOWN = 2,
	Remote_LEFT = 3,
	Remote_RIGHT = 4
}RemoteDirection;

typedef enum{
	SETTINGSTATE = 1,
	CONTROLSTATE = 2,
	DEBUGSTATE = 3
}RemoteState;

typedef enum{
	BuzzerChoseStore = 0,
	BeepNumStore = 1,
	FeedChoseStore = 2,
	FeedTime1Store = 3,
	FeedTime2Store = 4,
	FeedTime3Store = 5
}RemoteStore;

typedef enum{
	FeedLine = 2,
	BuzzerLine = 3
}RemoteLine;
#endif
