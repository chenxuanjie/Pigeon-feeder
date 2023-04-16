#include "stm32f10x.h"                  // Device header
#include "LED.h"                  // Device header

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	LEDO_OFF();
}

void LEDO_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LEDO_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
