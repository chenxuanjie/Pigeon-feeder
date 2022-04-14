#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "main.h"
#include "Key.h"

uint8_t KeyNumer;

void Key_IRQHandler(void);

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY_PIN1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_PIN1_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = KEY_PIN2 | KEY_PIN3;
	GPIO_Init(KEY_PIN2_3_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = KEY_PIN4 | KEY_PIN5| KEY_PIN6 |
								  KEY_PIN7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_PIN4_7_GPIO, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t temp;
	temp = KeyNumer;
	KeyNumer = 0;
	return temp;
}

uint8_t Key_GetNum2Loop(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(KEY_PIN1_GPIO, KEY_PIN1) == RESET)
		KeyNum |= KEY_PIN1_NUM;
	if (GPIO_ReadInputDataBit(KEY_PIN2_3_GPIO, KEY_PIN2) == RESET)
		KeyNum |= KEY_PIN2_NUM;	
	if (GPIO_ReadInputDataBit(KEY_PIN2_3_GPIO, KEY_PIN3) == RESET)
		KeyNum |= KEY_PIN3_NUM;
	
	if (GPIO_ReadInputDataBit(KEY_PIN4_7_GPIO, KEY_PIN4) == RESET)
		KeyNum |= KEY_PIN4_NUM;
	if (GPIO_ReadInputDataBit(KEY_PIN4_7_GPIO, KEY_PIN5) == RESET)
		KeyNum |= KEY_PIN5_NUM;
	if (GPIO_ReadInputDataBit(KEY_PIN4_7_GPIO, KEY_PIN6) == RESET)
		KeyNum |= KEY_PIN6_NUM;
	if (GPIO_ReadInputDataBit(KEY_PIN4_7_GPIO, KEY_PIN7) == RESET)
		KeyNum |= KEY_PIN7_NUM;
	return KeyNum;
}

void Key_Loop(void)
{
	static uint16_t NowState, LastState;
	LastState = NowState;
	NowState = Key_GetNum2Loop();
	if (NowState != LastState)
	{
		KeyNumer = NowState;
	}
}
