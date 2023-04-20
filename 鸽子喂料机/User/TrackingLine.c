#include "stm32f10x.h"    // Device header
#include "TrackingLine.h"
#include "NRF24L01.h"
#include "main.h"
#include "OLED.h"

/**
  * @brief  �Ҷȴ�������ʼ������
  * @param  ��
  * @retval ��
  */
void TrackingLine_Init(void)
{
	RCC_APB2PeriphClockCmd(DIGITAL_APBPEIRPH, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//��ʼ��GPIOC
	GPIO_InitStructure.GPIO_Pin = DIGITAL_1_PIN | DIGITAL_2_PIN | DIGITAL_3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DIGITAL_GPIO1, &GPIO_InitStructure);
	//��ʼ��GPIOA
	GPIO_InitStructure.GPIO_Pin = DIGITAL_4_PIN | DIGITAL_5_PIN;
	GPIO_Init(DIGITAL_GPIO2, &GPIO_InitStructure);
	
}

/**
  * @brief  ��ȡ�Ҷȴ���������������
  * @param  DigitalPin��Ҫ��ȡ������
  * @retval ���ŵ���ֵ��������Ĳ���ָ�������ţ��򷵻�2����ʾ����
  */
uint8_t TrackingLine_ReadDigital(uint16_t DigitalPin)
{
	if (DigitalPin==DIGITAL_1_PIN || DigitalPin==DIGITAL_2_PIN || DigitalPin==DIGITAL_3_PIN)
		return GPIO_ReadInputDataBit(DIGITAL_GPIO1, DigitalPin);
	else if (DigitalPin==DIGITAL_4_PIN || DigitalPin==DIGITAL_5_PIN)
		return GPIO_ReadInputDataBit(DIGITAL_GPIO2, DigitalPin);
	else
		return 2;
}

void TrackingLine_Test(void)
{
		OLED_ShowNum(1,1,TrackingLine_ReadDigital(DIGITAL_1_PIN),2);
		OLED_ShowNum(1,4,TrackingLine_ReadDigital(DIGITAL_2_PIN),2);
		OLED_ShowNum(1,7,TrackingLine_ReadDigital(DIGITAL_3_PIN),2);
		OLED_ShowNum(1,10,TrackingLine_ReadDigital(DIGITAL_4_PIN),2);
		OLED_ShowNum(1,13,TrackingLine_ReadDigital(DIGITAL_5_PIN),2);
}
