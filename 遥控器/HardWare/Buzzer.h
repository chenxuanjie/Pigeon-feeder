#ifndef __Buzzer_H__
#define __Buzzer_H__

#define Buzzer_NUM			19
#define Buzzer_W(x)			GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)x);
#define Buzzer_Turn			GPIO_WriteBit(GPIOB, GPIO_Pin_12,\
								(BitAction)!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12))
#define BUZZER_SETHZ(x)		TIM2->ARR = x	//更新自动重装载值

extern uint16_t T3_BeepTime;

void Buzzer_Init(void);
void Buzzer_SetBeep(uint8_t BeepNum, uint16_t BeepTime);
void CloseBuzzer(void);

#endif
