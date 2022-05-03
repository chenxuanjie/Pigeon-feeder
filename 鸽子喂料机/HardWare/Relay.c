#include "stm32f10x.h"    // Device header
#include "Relay.h"
#include "NRF24L01.h"
#include "main.h"

/**
  * @brief  继电器初始化函数
  * @param  无
  * @retval 无
  */
void Relay_Init(void)
{
	RCC_APB2PeriphClockCmd(RELAY_APBPEIRPH | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//重定义PB4引脚功能，免得无法使用
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RELAY1_PIN | RELAY2_PIN | RELAY3_PIN | RELAY4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RELAY_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(RELAY_GPIO, RELAY1_PIN);
	GPIO_ResetBits(RELAY_GPIO, RELAY2_PIN);
	GPIO_ResetBits(RELAY_GPIO, RELAY3_PIN);
	GPIO_ResetBits(RELAY_GPIO, RELAY4_PIN);
}

/**
  * @brief  喂料器的打开与关闭
  * @param  SET:打开 	RESET：关闭
  * @retval 无
  */
void Relay_Set(uint8_t Value, uint8_t State)
{
	if (Value == ALL)
	{
		GPIO_WriteBit(RELAY_GPIO, RELAY1_PIN, (BitAction)State);
		GPIO_WriteBit(RELAY_GPIO, RELAY2_PIN, (BitAction)State);
		GPIO_WriteBit(RELAY_GPIO, RELAY3_PIN, (BitAction)State);
	}
	else if (Value==1 && NRF24L01_GetData(FEEDTIME1_TRANSMIT)!=0)
		GPIO_WriteBit(RELAY_GPIO, RELAY1_PIN, (BitAction)State);
	else if (Value == 2 && NRF24L01_GetData(FEEDTIME2_TRANSMIT)!=0)
		GPIO_WriteBit(RELAY_GPIO, RELAY2_PIN, (BitAction)State);
	else if (Value == 3 && NRF24L01_GetData(FEEDTIME3_TRANSMIT)!=0)
		GPIO_WriteBit(RELAY_GPIO, RELAY3_PIN, (BitAction)State);	
}
