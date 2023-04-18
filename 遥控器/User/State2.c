#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "NRF24L01.h"
#include "main.h"


/** ��������ģʽ�����ϡ�
  * @brief  ��������ģʽ�����ϡ�
  * @param  
  * @retval None
  */
void Feeding_DebugFeedSet(machine* feeder1, machine* feeder2, machine* feeder3)
{
	//��ʾ����ι�ϵ�ι����(���ֶ�)	
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

/** �����Զ����ϡ�
  * @brief  �����Զ����ϡ�
  * @param  *Flag������ı�־λ,����1��ʾ�������ϡ�
			BlinkTimes����˸������
  * @retval None
  */
void Feeding_AutoFeedSet(uint8_t *Flag, uint8_t BlinkTimes)
{
	//����������ر�
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
	//��ʾ�Զ������Ƿ���, ������ʾ��ż������ʾ
	if (Flag%2 == SET)
		OLED_ShowString(2, 1, "AutoFeed: ON");
	else if (Flag%2 == RESET)
	OLED_ShowString(2, 1, "AutoFeed:   ");
	
}
