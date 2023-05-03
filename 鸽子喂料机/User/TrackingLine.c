#include "stm32f10x.h"    // Device header
#include "TrackingLine.h"
#include "NRF24L01.h"
#include "main.h"
#include "OLED.h"
#include "Robot.h"
#include "Feed.h"

/**
  * @brief  灰度传感器初始化函数
  * @param  无
  * @retval 无
  */
void TrackingLine_Init(void)
{
	RCC_APB2PeriphClockCmd(DIGITAL_APBPEIRPH, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//初始化GPIOC
	GPIO_InitStructure.GPIO_Pin = DIGITAL_1_PIN | DIGITAL_2_PIN | DIGITAL_3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DIGITAL_GPIO1, &GPIO_InitStructure);
	//初始化GPIOA
	GPIO_InitStructure.GPIO_Pin = DIGITAL_4_PIN | DIGITAL_5_PIN;
	GPIO_Init(DIGITAL_GPIO2, &GPIO_InitStructure);
}

/**
  * @brief  读取灰度传感器的引脚数据
  * @param  DigitalPin：要读取的引脚
  * @retval 引脚的数值。若输入的不是指定的引脚，则返回2，表示错误。
  */
uint8_t TrackingLine_ReadDigital(uint16_t DigitalPin)
{
	if (DigitalPin==DIGITAL_1_PIN || DigitalPin==DIGITAL_2_PIN || DigitalPin==DIGITAL_3_PIN)
		return GPIO_ReadInputDataBit(DIGITAL_GPIO1, DigitalPin);
	else if (DigitalPin==DIGITAL_4_PIN || DigitalPin==DIGITAL_5_PIN)
		return GPIO_ReadInputDataBit(DIGITAL_GPIO2, DigitalPin);
	else
		return 2;
}

uint8_t TrackingLine_Test(void)
{
	if (TrackingLine_IfExit())
	{
		OLED_ShowNum(1,12,1,1);
		return 1;
	}
	else
		OLED_ShowNum(1,12,0,1);
		
	return 0;
//		OLED_ShowNum(1,1,TrackingLine_ReadDigital(DIGITAL_1_PIN),2);
//		OLED_ShowNum(1,4,TrackingLine_ReadDigital(DIGITAL_2_PIN),2);
//		OLED_ShowNum(1,7,TrackingLine_ReadDigital(DIGITAL_3_PIN),2);
//		OLED_ShowNum(1,10,TrackingLine_ReadDigital(DIGITAL_4_PIN),2);
//		OLED_ShowNum(1,13,TrackingLine_ReadDigital(DIGITAL_5_PIN),2);
}

/**     判断是否退出循迹模式
  * @brief  Judge if exit the Trackingline mode.
  * @param  None
  * @retval return 0 continue Trackingline mode;
			return 1 exit Trackingline mode. 
  */
uint8_t TrackingLine_IfExit(void)
{
	NRF24L01_ReceiveData();
//	OLED_ShowHexNum(4, 13, NRF24L01_ReadByte(FIFO_STATUS), 2);
	if (NRF24L01_ReadByte(FIFO_STATUS) == 0x10)
		OLED_ShowNum(4, 3, 0, 1);
	else if (NRF24L01_ReadByte(FIFO_STATUS) == 0x11)
		OLED_ShowNum(4, 3, 1, 1);
	else
		OLED_ShowNum(4, 3, 2, 1);
	if (NRF24L01_GetData(TRACKINGLINE_TRANSMIT) == TRACKINGLINE_OFF)
	{
		Robot_Stop();
		return 1;
	}
	else
		return 0;
}

/**			循迹模式开启
  * @brief  Start Tracking mode.
  * @param  None
* @retval Flag: 0: still in the Tracking mode.
				1: exit the Tracking mode. 
  */
uint8_t TrackingLine(machine* machine1, machine* machine2, machine* machine3)
{
	uint8_t Digital_Mode;
	static uint8_t Feeding_TrackingLineAutoTimes;
	//调试
	OLED_ShowNum(4,7,1,1);
	OLED_ShowNum(4,10,Digital_L2,1);
	OLED_ShowNum(4,11,Digital_L1,1);
	OLED_ShowNum(4,12,Digital_M,1);
	OLED_ShowNum(4,13,Digital_R1,1);
	OLED_ShowNum(4,14,Digital_R2,1);
	//显示
	OLED_ShowNum(1,10,Feeding_TrackingLineAutoTimes,1);
	//是否切换手动
	if (TrackingLine_IfExit())
		Digital_Mode = DIGITAL_STOP;
	else
		Digital_Mode = Digital_ModeJudge(Feeding_TrackingLineAutoTimes);
	
	//调试 
		OLED_ShowNum(4,16,Digital_Mode,1);

	switch (Digital_Mode)
	{
		case DIGITAL_STRAIGHT:		//正常直行
			return Digital_Straight();
		case DIGITAL_TURNLEFT:		//左转
			return Digital_Turn(LEFT);
		case DIGITAL_TURNRIGHT:	//右转
			return Digital_Turn(RIGHT);
		case DIGITAL_FEEDING:	//喂料模式
			return Digital_Feeding(machine1, machine2, machine3, &Feeding_TrackingLineAutoTimes);
		case DIGITAL_STOP:		//循迹结束，自动切换为遥控器控制模式
			Robot_Stop();
			return 1;
		default: Robot_Stop();return 1;	//意外情况
	}
}

/**     模式判断
  * @brief  mode will be judged and determinte how to run by a correct way.
  * @param  None.
* @retval return 0: Unexpected return.
		  return DIGITAL_STRAIGHT:  Go straight.
		  return DIGITAL_TURNLEFT:  Turn left.
		  return DIGITAL_TURNRIGHT: Turn right.
		  return DIGITAL_FEEDING:   Feeding mode.
		  return DIGITAL_STOP:		Stop robot and return the remote control mode.
  */
uint8_t Digital_ModeJudge(uint8_t TrackingLine_FeedFlag)
{
	if (Digital_L2==RESET && Digital_R2==RESET)		//直走
		return DIGITAL_STRAIGHT;
	else if (Digital_L2==SET && Digital_R2!=SET)	//左转
		return DIGITAL_TURNLEFT;
	else if (Digital_L2!=SET && Digital_R2==SET)	//右转
		return DIGITAL_TURNRIGHT;
	//喂料(之前踩线次数。若踩线2次，实际只喂1次，因此仍然需要再喂一次）
	else if (Digital_L2==SET && Digital_R2==SET && TrackingLine_FeedFlag<=DIGITAL_FEEDINGTIMES)	
		return DIGITAL_FEEDING;
	else if (Digital_L2==SET && Digital_R2==SET && TrackingLine_FeedFlag>DIGITAL_FEEDINGTIMES)	//达到喂料的次数，停止
		return DIGITAL_STOP;
	else		
		return 0;
	
}

/**     正常直走
  * @brief  run forward straightly by recognizing  black line.
  * @param  
  * @retval return 0 meeting crossroad; return 1 in a straight road. 
  */
uint8_t Digital_Straight(void)
{
	Robot_Start();
	if (Digital_L1==1 && Digital_M==0 && Digital_R1==0)	//偏右
	{
		Robot_TurnLeft(FRONT);
	}
	else if (Digital_L1==1 && Digital_M==1 && Digital_R1==0)	//微微偏右
	{
		Robot_TurnLeft(FRONT);
	}
	else if (Digital_L1==0 && Digital_M==0 && Digital_R1==1)	//偏左
	{
		Robot_TurnRight(FRONT);
	}
	else if (Digital_L1==0 && Digital_M==1 && Digital_R1==1)	//微微偏左
	{
		Robot_TurnRight(FRONT);
	}
	else if (Digital_L1==0 && Digital_M==1 && Digital_R1==0)	//正常
	{
		Robot_Front();
	}
	else if (Digital_L1==0 && Digital_M==0 && Digital_R1==0)	//都检测不到
	{
		Robot_Front();
	}
	else 
		Robot_Front();
	return 0;
}

/**     循迹模式下的自旋式直走
  * @brief  run forward straightly by recognizing  black line.
  * @param  
  * @retval return 0 meeting crossroad; return 1 in a straight road. 
  */
uint8_t Digital_Straight2(void)
{
	Robot_Start();
	if (Digital_L1==1 && Digital_M==0 && Digital_R1==0)	//偏右
	{
		Digital_Turn2(LEFT);
	}
	else if (Digital_L1==1 && Digital_M==1 && Digital_R1==0)	//微微偏右
	{
		Digital_Turn2(LEFT);
	}
	else if (Digital_L1==0 && Digital_M==0 && Digital_R1==1)	//偏左
	{
		Digital_Turn2(RIGHT);
	}
	else if (Digital_L1==0 && Digital_M==1 && Digital_R1==1)	//微微偏左
	{
		Digital_Turn2(RIGHT);
	}
	else if (Digital_L1==0 && Digital_M==1 && Digital_R1==0)	//正常
	{
		Robot_Front();
	}
	else if (Digital_L1==0 && Digital_M==0 && Digital_R1==0)	//都检测不到
	{
		Robot_Front();
	}
	else 
		Robot_Front();
	return 0;
}

/**
  * @brief  转弯。调用此函数，转弯后会停0.5秒。
* @param  Direction:方向。LEFT：左。RIGHT：右。
  * @retval 无
  */
uint8_t Digital_Turn(uint8_t Direction)
{
	Robot_Start();
//	if (Robot_RunTime(TRACKINGLINE_RUNTIME) != 0)
//		return 1;
//	if (Robot_StopTime(TRACKINGLINE_STOPTIME) != 0)
//		return 1;
	switch(Direction)
	{
		case LEFT:
		{
			while (!(Digital_R1==1))	//转到整车稍微偏左为止
			{
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(NI);
			}
		}break;
		case RIGHT:
		{
			while (!(Digital_L1==1))	//转到整车稍微偏右为止
			{
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(SHUN);
			}
		}break;
	}
//	if (Robot_StopTime(TRACKINGLINE_STOPTIME) != 0)
//		return 1;
	return 0;
}

/**
  * @brief  循迹模式下的差速转弯。
* @param  Direction:方向。LEFT：左。RIGHT：右。
  * @retval 无
  */
uint8_t Digital_Turn2(uint8_t Direction)
{
	Robot_Start();
	switch(Direction)
	{
		case LEFT:
		{
			while (!(Digital_L1==0 && Digital_M==1 && Digital_R1==0))	//中间黑线两边无
			{
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(NI);
			}
		}break;
		case RIGHT:
		{
			while (!(Digital_L1==0 && Digital_M==1 && Digital_R1==0))	//中间黑线两边无
			{
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(SHUN);
			}
		}break;
	}
	return 0;
}

uint8_t Digital_Feeding(machine* machine1, machine* machine2, machine* machine3, uint8_t* TrackingLine_FeedFlag)
{
	//停止1秒，准备落料
	if (Robot_StopTime(1000) != 0)
		return 1;
	Get_FeedTime(machine1, machine2, machine3, *TrackingLine_FeedFlag);
	//智能喂料，直到喂料完成
	while(machine1->SecondAutoTime_ms!=0 && machine2->SecondAutoTime_ms!=0 && machine3->SecondAutoTime_ms!=0)
	{
		if (TrackingLine_IfExit())
			return 1;
		else
			StartFeed(machine1->SecondAutoTime_ms, machine2->SecondAutoTime_ms, machine3->SecondAutoTime_ms);
	}
	Feeding_CloseFeeder();		//喂料完成后，确保喂料器全部关闭
	(*TrackingLine_FeedFlag) ++;	//每喂一次料加一
	//停止0.5秒，避免余料没有落完
	if (Robot_StopTime(TRACKINGLINE_STOPTIME) != 0)
		return 1;	
	//继续向前走一点，免得下次监测到继续喂料
	if (Robot_RunTime(TRACKINGLINE_RUNTIME) != 0)
		return 1;
	return 0;
}
