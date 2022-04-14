#ifndef __KEY_H
#define __KEY_H

#define KEY_PIN1_GPIO		GPIOC
#define KEY_PIN1			GPIO_Pin_15

#define KEY_PIN2_3_GPIO		GPIOA
#define KEY_PIN2			GPIO_Pin_12
#define KEY_PIN3			GPIO_Pin_15
#define KEY_PIN4_7_GPIO		GPIOB
#define KEY_PIN4			GPIO_Pin_5
#define KEY_PIN5			GPIO_Pin_6
#define KEY_PIN6			GPIO_Pin_4
#define KEY_PIN7			GPIO_Pin_7

//按键
#define KEY_PIN1_NUM		0x1
#define KEY_PIN2_NUM		(0x1 << 1)
#define KEY_PIN3_NUM		(0x1 << 2)
#define KEY_PIN4_NUM		(0x1 << 3)	//上
#define KEY_PIN5_NUM		(0x1 << 4)	//右
#define KEY_PIN6_NUM		(0x1 << 5)	//左
#define KEY_PIN7_NUM		(0x1 << 6)	//下


//钮子开关
#define SWITCH2_PIN1		GPIO_Pin_15		//2档
#define SWITCH3_PIN1		GPIO_Pin_13		//3档
#define SWITCH3_PIN2		GPIO_Pin_14		//3档

void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Loop(void);

#endif
