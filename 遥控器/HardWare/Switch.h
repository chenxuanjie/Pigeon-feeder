#ifndef __SWITCH_H
#define __SWITCH_H

#define SWITCH_GPIO				GPIOB
#define SWITCH2_PIN1			GPIO_Pin_15		//2µµ
#define SWITCH3_PIN1			GPIO_Pin_13		//3µµ
#define SWITCH3_PIN2			GPIO_Pin_14		//3µµ

#define SWITCH2_PIN1_NUM		0x1
#define SWITCH3_PIN1_NUM		(0x1 << 1)
#define SWITCH3_PIN2_NUM		(0x1 << 2)
#define SWITCH3_NONE			(0x1 << 3)


void Switch_Init(void);
void Switch_Loop(void);
uint8_t Switch_GetNum(void);

#endif
