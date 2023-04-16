#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "OLED.h"
#include "Feed.h"
#include "Relay.h"
#include "Hcsr04.h"

uint8_t Bird1, Bird2, Bird3;
uint32_t Timeout;
float Distance1, Distance2, Distance3, Distance4;

/**		得到四个超声波到料仓间的距离
  * @brief  Monitor the Distance from to sensor 3 repository .
  * @param  Hcsr04_StartFlag: 超声波使能位
  * @retval None
  */
void Get_Distance(uint8_t Hcsr04_StartFlag)
{
	if (Hcsr04_StartFlag&0x08)	//超声波是否使能
	{
		switch(Hcsr04_StartFlag&0x07)	//第几个超声波运行
		{
			case 1: Distance1 = Hcsr04_GetDistance1();break;
			case 2: Distance2 = Hcsr04_GetDistance2();break;
			case 3: Distance3 = Hcsr04_GetDistance3();break;
			case 4: Distance4 = Hcsr04_GetDistance4();break;						
		}
	Hcsr04_StartFlag &= 0x07;		//清除使能位
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
void MonitorFeed(void)
{
//监测剩余饲料
//1	
		if(Distance1 > 169.7)
		{
			OLED_ShowString(3, 3, "Lo");
		}
		else
		{	
			if(Distance1 > 62.35 && Distance1 <= 169.7)
			{
				OLED_ShowString(3, 3, "Mi");
			}
			
			else
			{
				OLED_ShowString(3, 3, "Fu");
			}
			
		}

//2
		if(Distance2 > 169.7)
		{
			OLED_ShowString(3, 8, "Lo");
		}
		else
		{	
			if(Distance2 > 62.35 && Distance2 <= 169.7)
			{
				OLED_ShowString(3, 8, "Mi");
			}
			
			else
			{
				OLED_ShowString(3, 8, "Fu");
			}
			
		}
		
//3
		if(Distance3 > 169.7)
		{
			OLED_ShowString(3, 14, "Lo");
		}
		else
		{	
			if(Distance3 > 62.35 && Distance3 <= 169.7)
			{
				OLED_ShowString(3, 14, "Mi");
			}
			
			else
			{
				OLED_ShowString(3, 14, "Fu");
			}
			
		}
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
void Get_BirdNum(void)
{
	if (Timeout == 0)
	{
		USART1_GetBirdNum(&Bird1, &Bird2, &Bird3);
		Timeout = 500;
	}
	OLED_ShowNum(1,4,Bird1,1);
	OLED_ShowNum(1,6,Bird2,1);
	OLED_ShowNum(1,8,Bird3,1);
}

void GetFeedTime(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3)
{
	*feedTime1 = Bird1 * FEEDTIME_PERBIRD;
	*feedTime2 = Bird2 * FEEDTIME_PERBIRD;
	*feedTime3 = Bird3 * FEEDTIME_PERBIRD;
}	

void StartFeed(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3)
{
	if (*feedTime1 > 0) Relay_Set(1, SET);
	else Relay_Set(1, RESET);
	
	if (*feedTime2 > 0) Relay_Set(2, SET);
	else Relay_Set(2, RESET);
		
	if (*feedTime3 > 0) Relay_Set(3, SET);
	else Relay_Set(3, RESET);		
}
