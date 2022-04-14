#ifndef __BSP_USART_H_
#define __BSP_USART_H_

#include "stm32f10x.h"
#include <stdio.h>

				//选择启用的串口号(禁止出现两个串口同时开启)
#define DEBUG_USART1	0
#define DEBUG_USART2	1
#define DEBUG_USART3	1
#define DEBUG_USART4	0
#define DEBUG_USART5	0



#if DEBUG_USART1
// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1

																			//修改外设时,更换挂载的总线
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           57600

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

#endif

#if DEBUG_USART2
 //串口2-USART2
#define  DEBUG_USARTx                   USART2
#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           57600

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  DEBUG_USART_IRQ                USART2_IRQn
#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

#endif


#if DEBUG_USART3
// 串口3-USART3
#define  DEBUG_USARTx4                   USART3
#define  DEBUG_USART_CLK4                RCC_APB1Periph_USART3
#define  DEBUG_USART_APBxClkCmd4         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE4           57600

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK4           (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART_GPIO_APBxClkCmd4    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT4       GPIOB   
#define  DEBUG_USART_TX_GPIO_PIN4        GPIO_Pin_10
#define  DEBUG_USART_RX_GPIO_PORT4       GPIOB
#define  DEBUG_USART_RX_GPIO_PIN4        GPIO_Pin_11

#define  DEBUG_USART_IRQ4                USART3_IRQn
#define  DEBUG_USART_IRQHandler4         USART3_IRQHandler

#endif

#if DEBUG_USART4
 //串口4-UART4
#define  DEBUG_USARTx4                   UART4
#define  DEBUG_USART_CLK4                RCC_APB1Periph_UART4
#define  DEBUG_USART_APBxClkCmd4         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE4           57600
// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK4           (RCC_APB2Periph_GPIOC)
#define  DEBUG_USART_GPIO_APBxClkCmd4    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT4       GPIOC   
#define  DEBUG_USART_TX_GPIO_PIN4        GPIO_Pin_10
#define  DEBUG_USART_RX_GPIO_PORT4       GPIOC
#define  DEBUG_USART_RX_GPIO_PIN4        GPIO_Pin_11

#define  DEBUG_USART_IRQ4                UART4_IRQn
#define  DEBUG_USART_IRQHandler4         UART4_IRQHandler

#endif

#if DEBUG_USART5
 //串口5-UART5
#define  DEBUG_USARTx                   UART5
#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_12
#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

#define  DEBUG_USART_IRQ                UART5_IRQn
#define  DEBUG_USART_IRQHandler         UART5_IRQHandler

#endif

void USART_Config(void);		 															//串口初始化
void Usart_SendByte(USART_TypeDef* pUSARTx, int8_t data);//串口发送字节
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data);//发送两个字节
void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num);//发送一串数组

void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str);//发送字符串
#endif		/* __BSP_USART_H */
