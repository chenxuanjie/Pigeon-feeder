#ifndef __TIM3_H
#define __TIM3_H
#include "stm32f10x.h"

void TIM3_Init(void);
void TIM3_Cmd(FunctionalState NewState);
void TIM3_SetIRQHandler(void (*Function)(void));

#endif
