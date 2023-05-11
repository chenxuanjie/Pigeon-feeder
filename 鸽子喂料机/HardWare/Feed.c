#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "OLED.h"
#include "Feed.h"
#include "Relay.h"
#include "Hcsr04.h"

uint8_t Bird1, Bird2, Bird3, Feeding_RemoteAutoTimes;
float Distance1, Distance2, Distance3, Distance4;

uint16_t Feeding_GetRemoteAutoTimes(void)
{
	return Feeding_RemoteAutoTimes;
}

void Feeding_ShowTime(machine* machine1, machine* machine2, machine* machine3)
{
	OLED_ShowNum(2,2,machine1->SecondAutoTime_ms,4);
	OLED_ShowNum(2,8,machine2->SecondAutoTime_ms,4);
	OLED_ShowNum(2,14,machine3->SecondAutoTime_ms,4);
	OLED_ShowNum(2,1,machine1->FirstAutoTime_ms/1000,1);
	OLED_ShowNum(2,7,machine2->FirstAutoTime_ms/1000,1);
	OLED_ShowNum(2,13,machine3->FirstAutoTime_ms/1000,1);
}

/**
  * @brief  设定延时时间，设置1则为1ms。
  * @param  Num：延时Value ms。
  * @retval 无
  */
void Feeding_SetRemoteAutoTimes(uint16_t Num)
{
		Feeding_RemoteAutoTimes = Num;
}

/**		得到四个超声波到料仓间的距离
  * @brief  Monitor the Distance from to sensor 3 repository .
  * @param  Hcsr04_StartFlag: 超声波使能位
  * @retval None
  */
void Get_Distance(uint8_t *Hcsr04_StartFlag)
{
	if ((*Hcsr04_StartFlag)&0x08)	//超声波是否使能
	{
		switch((*Hcsr04_StartFlag)&0x07)	//第几个超声波运行(这里用十进制的逻辑，不是二进制)
		{
			case 1: Distance1 = Hcsr04_GetDistance1();break;
			case 2: Distance2 = Hcsr04_GetDistance2();break;
			case 3: Distance3 = Hcsr04_GetDistance3();break;
			case 4: Distance4 = Hcsr04_GetDistance4();break;						
		}
	(*Hcsr04_StartFlag) &= 0x07;		//清除使能位
	}	
	//show
//	OLED_ShowNum(2 , 3, Distance1/10, 2);
//	OLED_ShowNum(2 , 11, Distance2/10, 2);
//	OLED_ShowNum(3 , 3, Distance3/10, 2);
//	OLED_ShowNum(3 , 11, Distance4/10, 2);
//	OLED_ShowNum(2 , 6, (uint32_t)Distance1%10 ,1);
//	OLED_ShowNum(2 , 14, (uint32_t)Distance2%10 ,1);
//	OLED_ShowNum(3 , 6, (uint32_t)Distance3%10 ,1);
//	OLED_ShowNum(3 , 14, (uint32_t)Distance4%10 ,1);
}

/**		监测三个料仓的余料
  * @brief  Monitor the remaining feed in 3 repository .
  * @param  
  * @retval None
  */
void MonitorFeed(uint8_t *Hcsr04_StartFlag)
{
	Get_Distance(Hcsr04_StartFlag);
//1	
		if(Distance1 > 169.7)
			OLED_ShowString(3, 3, "Lo");
		else
		{	
			if(Distance1 > 62.35 && Distance1 <= 169.7)
				OLED_ShowString(3, 3, "Mi");
			else
				OLED_ShowString(3, 3, "Fu");
		}

//2
		if(Distance2 > 169.7)
			OLED_ShowString(3, 8, "Lo");
		else
		{	
			if(Distance2 > 62.35 && Distance2 <= 169.7)
				OLED_ShowString(3, 8, "Mi");
			else
				OLED_ShowString(3, 8, "Fu");
		}
		
//3
		if(Distance3 > 169.7)
			OLED_ShowString(3, 13, "Lo");
		else
		{
			if(Distance3 > 62.35 && Distance3 <= 169.7)
				OLED_ShowString(3, 13, "Mi");
			else
				OLED_ShowString(3, 13, "Fu");
		}
//		OLED_ShowNum(3 , 1, Distance1/10, 2);
////		OLED_ShowString(3, 3,"%");
//		OLED_ShowNum(3 , 5, Distance2/10, 2);
////		OLED_ShowString(3, 7,"%");
//		OLED_ShowNum(3 , 9, Distance3/10, 2);
////		OLED_ShowString(3, 11,"%");
////		OLED_ShowNum(3 , 13, Distance4/10, 2);//避障用

}


/**		初始化喂料系统
  * @brief  Init feed system.
  * @param  None
  * @retval None
  */
void Feeding_Init(void)
{
	Relay_Init();
	USART1_Config();
}

//每隔0.5s获取现在的鸽子数量
void Get_BirdNum(uint32_t *Timeout)
{
	if (*Timeout == 0)
	{
		USART1_GetBirdNum(&Bird1, &Bird2, &Bird3);
		*Timeout = 500;
	}
	OLED_ShowNum(1,1,Bird1,1);
	OLED_ShowNum(1,3,Bird2,1);
	OLED_ShowNum(1,5,Bird3,1);
}

/**		将落料秒数转化为毫秒(一只鸽子定义为1s)
  * @brief  Convert unit 's' of feeding to 'ms'.
  * @param  
  * @retval None
  */
void Get_FeedTime(machine* machine1, machine* machine2, machine* machine3, uint8_t Flag)
{
	if (Flag == 0)
	{
		machine1->SecondAutoTime_ms = 0;
		machine2->SecondAutoTime_ms = 0;
		machine3->SecondAutoTime_ms = 0;	
		machine1->FirstAutoTime_ms = Bird1 * FEEDTIME_PERBIRD;
		machine2->FirstAutoTime_ms = Bird2 * FEEDTIME_PERBIRD;
		machine3->FirstAutoTime_ms = Bird3 * FEEDTIME_PERBIRD;
	}
	else
	{
		machine1->SecondAutoTime_ms = machine1->FirstAutoTime_ms;
		machine2->SecondAutoTime_ms = machine2->FirstAutoTime_ms;
		machine3->SecondAutoTime_ms = machine3->FirstAutoTime_ms;	
		machine1->FirstAutoTime_ms = Bird1 * FEEDTIME_PERBIRD;
		machine2->FirstAutoTime_ms = Bird2 * FEEDTIME_PERBIRD;
		machine3->FirstAutoTime_ms = Bird3 * FEEDTIME_PERBIRD;
	}
	Feeding_ShowTime(machine1, machine2, machine3);	
}	

/**		根据输入的秒数驱动落料器
  * @brief  Driving feeder according to the input seconds.
* @param  feedTime1: 落料器1的秒数
  		  feedTime2: 落料器2的秒数
		  feedTime3: 落料器3的秒数
  * @retval None
  */
void StartFeed(uint32_t feedTime1_ms, uint32_t feedTime2_ms, uint32_t feedTime3_ms)
{
	if (feedTime1_ms > 0) Relay_Set(1, SET);
	else Relay_Set(1, RESET);
	
	if (feedTime2_ms > 0) Relay_Set(2, SET);
	else Relay_Set(2, RESET);
		
	if (feedTime3_ms > 0) Relay_Set(3, SET);
	else Relay_Set(3, RESET);			
}

void Feeding_CloseFeeder(void)
{
	Relay_Set(ALL, RESET);
}

void Feeding_ResetFeeder(machine* machine1, machine* machine2, machine* machine3)
{
	machine1->FirstAutoTime_ms = 0;
	machine2->FirstAutoTime_ms = 0;
	machine3->FirstAutoTime_ms = 0;
	machine1->SecondAutoTime_ms = 0;
	machine2->SecondAutoTime_ms = 0;
	machine3->SecondAutoTime_ms = 0;	
	Relay_Set(ALL, RESET);
}
