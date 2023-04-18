#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "NRF24L01.h"
#include "main.h"


/** 开启调试模式的落料。
  * @brief  开启调试模式的落料。
  * @param  
  * @retval None
  */
void Feeding_DebugFeedSet(machine* feeder1, machine* feeder2, machine* feeder3)
{
	//显示正在喂料的喂料器(仅手动)	
	if (feeder1->Time_ms != 0)
		OLED_ShowChar(4, 4, '1');
	else
		OLED_ShowChar(4, 4, ' ');
	if (feeder2->Time_ms != 0)
		OLED_ShowChar(4, 6, '2');
	else
		OLED_ShowChar(4, 6, ' ');
	if (feeder2->Time_ms != 0)
		OLED_ShowChar(4, 8, '3');
	else
		OLED_ShowChar(4, 8, ' ');
}

/** 开启自动落料。
  * @brief  开启自动落料。
  * @param  *Flag：传入的标志位,大于1表示开启落料。
			BlinkTimes：闪烁次数。
  * @retval None
  */
void Feeding_AutoFeedSet(uint8_t *Flag, uint8_t BlinkTimes)
{
	//若开启，则关闭
	if (*Flag != 0)
	{
		*Flag = 0;
		NRF24L01_SetBuf(NORMAL_TRANSMIT, AUTO_FEED_OFF);
	}
	else
	{
		*Flag = BlinkTimes;
		NRF24L01_SetBuf(NORMAL_TRANSMIT, AUTO_FEED_ON);
		OLED_ShowNum(1,1,1,1);
		OLED_ShowNum(4,3,NRF24L01_GetData(NORMAL_TRANSMIT),1);
	}
}

void Feeding_AutoFeedShow(uint8_t Flag)
{
	//显示自动落料是否开启, 奇数显示，偶数不显示
	if (Flag%2 == SET)
		OLED_ShowString(2, 1, "AutoFeed: ON");
	else if (Flag%2 == RESET)
	OLED_ShowString(2, 1, "AutoFeed:   ");
	
}
