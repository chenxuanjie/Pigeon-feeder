#ifndef __USART_H__
#define __USART_H__

#include <stdio.h>

#define USART_RECIEVE_TEMP   12
#define USART_MAX_TEMP  	 13
extern uint16_t Data;

void USART1_Config(void);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void USART1_GetBirdNum(uint8_t* feedTime1, uint8_t* feedTime2, uint8_t* feedTime3);

#endif
