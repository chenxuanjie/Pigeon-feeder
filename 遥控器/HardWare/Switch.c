#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "main.h"
#include "switch.h"
#include "TIM3.h"

uint8_t SwitchNumer;

void Switch_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = SWITCH2_PIN1 | SWITCH3_PIN1 | SWITCH3_PIN2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SWITCH_GPIO, &GPIO_InitStructure);
	
	TIM3_Init();
}

uint8_t Switch_GetNum(void)
{
	return SwitchNumer;
}

uint8_t Switch_GetNum2Looop(void)
{
	uint8_t SwitchNum = 0;
	if (GPIO_ReadInputDataBit(SWITCH_GPIO, SWITCH2_PIN1) == SET)
		SwitchNum |= SWITCH2_PIN1_NUM;
	if (GPIO_ReadInputDataBit(SWITCH_GPIO, SWITCH3_PIN1) == RESET)
		SwitchNum |= SWITCH3_PIN1_NUM;	
	else if (GPIO_ReadInputDataBit(SWITCH_GPIO, SWITCH3_PIN2) == RESET)
		SwitchNum |= SWITCH3_PIN2_NUM;
	else
		SwitchNum |= SWITCH3_NONE;
	
	return SwitchNum;
}

void Switch_Loop(void)
{
	SwitchNumer = Switch_GetNum2Looop();
}
