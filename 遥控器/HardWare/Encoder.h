#ifndef __ENCODER_H__
#define __ENCODER_H__

#define ENCODER_GPIO	GPIOB	//����޸Ĵ˴�������Ҫ�޸�ʱ�ӡ��ж�
#define ENCODER_PIN1	GPIO_Pin_1
#define ENCODER_PIN2	GPIO_Pin_10

void Encoder_Init(void);
int16_t Encoder_Get(void);


#endif
