#ifndef __TRACKINGLINE_H__
#define __TRACKINGLINE_H__

#define DIGITAL_APBPEIRPH		RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
//循迹模块从左到右5个引脚
#define DIGITAL_GPIO1	GPIOC
#define DIGITAL_1_PIN	GPIO_Pin_13
#define DIGITAL_2_PIN	GPIO_Pin_14
#define DIGITAL_3_PIN	GPIO_Pin_15
#define DIGITAL_GPIO2	GPIOA
#define DIGITAL_4_PIN	GPIO_Pin_0
#define DIGITAL_5_PIN	GPIO_Pin_8

void TrackingLine_Init(void);
uint8_t TrackingLine_ReadDigital(uint16_t DigitalPin);

#endif
