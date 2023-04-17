#ifndef __MAIN_H__
#define __MAIN_H__

#define ORINGINAL_VOLTAGE		ADC_ConvertedValue[2]

//抽象层
#define KEY_ENTER				KEY_RIGHT
#define KEY_ROCKER				KEY_PIN1_NUM
#define KEY_BACK				KEY_LEFT			

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
#define AUTO_FEED_OFF			(0x0 + 3)
#define AUTO_FEED_ON			(0x1 + 3)
#define CHECK					(0xFE)
//按键 KEY_TRANSMIT
#define KEY_ROCKER_TRANSMIT			0x1
#define KEY_UP					KEY_PIN4_NUM
#define KEY_DOWN				KEY_PIN5_NUM
#define KEY_LEFT				KEY_PIN6_NUM
#define KEY_RIGHT				KEY_PIN7_NUM

//摇杆应用层(F为负，Z为正) 		ROCKER_TRANSMIT
#define UP_1					Y_F1		//1,2,3,4表示程度。1为最小，5最大。
#define UP_2					Y_F2
#define UP_3					Y_F3
#define UP_4					Y_F4
#define DOWN_1					Y_Z1
#define DOWN_2					Y_Z2
#define DOWN_3					Y_Z3
#define DOWN_4					Y_Z4
#define LEFT_1					X_F1
#define LEFT_2					X_F2
#define LEFT_3					X_F3
#define LEFT_4					X_F4
#define RIGHT_1					X_Z1
#define RIGHT_2					X_Z2
#define RIGHT_3					X_Z3
#define RIGHT_4					X_Z4
#define STOP_X					X_MIDDLE	//应用层上的X
#define STOP_Y					Y_MIDDLE	//应用层上的Y
//物理层(F为负，Z为正)
#define X_F1					0x1		//1,2,3,4表示程度。1为最小，5最大。
#define X_F2					0x2
#define X_F3					0x3
#define X_F4					0x4
#define X_Z1					0x5
#define X_Z2					0x6
#define X_Z3					0x7
#define X_Z4					0x8
#define Y_F1					0x1
#define Y_F2					0x2
#define Y_F3					0x3
#define Y_F4					0x4
#define Y_Z1					0x5
#define Y_Z2					0x6
#define Y_Z3					0x7
#define Y_Z4					0x8
#define X_MIDDLE				0x9
#define Y_MIDDLE				0x9
#define MEDIWM					0x99

#define MAX_MODE				4
#define STORENUM				6

typedef struct machine{
	uint32_t Time_ms;
	uint8_t Time_s;
}machine;

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
	STATE_SETTING = 10,
	STATE_SETTING_ORIGIN = 10,
	STATE_SETTING_FEED = 11,
	STATE_SETTING_BUZZER = 12,
	STATE_SETTING_OTHER = 13,
	STATE_CONTROL = 20,
	STATE_DEBUG = 30
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
