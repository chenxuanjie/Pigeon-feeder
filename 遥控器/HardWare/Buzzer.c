#include "stm32f10x.h"                  // Device header
#include "TIM2.h"
#include "Buzzer.h"
#include "Delay.h"

uint16_t T3_BeepTime;

const uint16_t Buzzer_Counter[Buzzer_NUM]={
	19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1
};

void Buzzer_Init(void)
{
	TIM2_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Buzzer_W(1);
}

void Buzzer_SetBeep(uint8_t BeepNum, uint16_t BeepTime)
{
	T3_BeepTime = BeepTime;
	BUZZER_SETHZ(Buzzer_Counter[BeepNum]);
	TIM_Cmd(TIM2, ENABLE);
}

void CloseBuzzer(void)
{
	Buzzer_W(1);
	TIM_Cmd(TIM2, DISABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update))
	{
		Buzzer_Turn;
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	}

}
