#ifndef __RELAY_H__
#define __RELAY_H__

#define RELAY_APBPEIRPH		RCC_APB2Periph_GPIOB
#define RELAY_GPIO			GPIOB
#define RELAY1_PIN			GPIO_Pin_13
#define RELAY2_PIN			GPIO_Pin_12
#define RELAY3_PIN			GPIO_Pin_14
#define RELAY4_PIN			GPIO_Pin_15

#define ALL					4

void Relay_Init(void);
void Relay_Set(uint8_t Value, uint8_t State);


#endif
