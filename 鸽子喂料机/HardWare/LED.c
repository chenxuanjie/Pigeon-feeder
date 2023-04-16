#include "stm32f10x.h"                  // Device header
#include "LED.h"                  // Device header

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*����LED��ص�GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIO*/
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
