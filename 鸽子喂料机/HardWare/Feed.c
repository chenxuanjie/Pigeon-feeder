#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "OLED.h"
#include "Feed.h"
#include "Relay.h"

uint8_t Bird1, Bird2, Bird3;
uint32_t Timeout;

void Feed_Init(void)
{
	USART1_Config();
}

//每隔0.5s获取现在的鸽子数量
void GetBirdNum(void)
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
