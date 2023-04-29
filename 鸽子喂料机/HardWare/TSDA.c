#include "TSDA.h"
#include "bsp_usart.h"
#include "DELAY.h"
#include "ROBOT.H"
void TSDA_Order(USART_TypeDef* pUSARTx,uint32_t order32)
{

	uint8_t orders[4],i;
	orders[0]=(order32&0XFF000000)>>24;
	orders[1]=(order32&0XFF0000)>>16;
	orders[2]=(order32&0XFF00)>>8;
	orders[3]=(order32&0XFF);
for(i=0;i<4;i++)
	{
		
//		/* 发送一个字节数据到USART */
//	USART_SendData(TSDA_Usart,orders[i]);
////SysTick_Delay_ms(1);
//	/* 等待发送数据寄存器为空 */
//	while (USART_GetFlagStatus(TSDA_Usart, USART_FLAG_TXE) == RESET);
//	
		Usart_SendByte(pUSARTx,orders[i] );
		Delay_ms(2);	
	
	}
//	while (USART_GetFlagStatus(TSDA_Usart, USART_FLAG_TXE) == RESET);
//	checkorder=USART_ReceiveData(TSDA_Usart);
//	if(checkorder==orders[0]){printf("%x=%x",checkorder,orders[0]);return 1;}
//	
//	else return 0;
	
//	if(scanf("%x",&k)==orders[0]){printf("%x",k);return 1;}
//	else return 0;
}

void TSDA_Data(USART_TypeDef* pUSARTx,int8_t order8,int16_t data)
{
	
	int8_t datas[4],i;
	int16_t chack;
	datas[0]=order8;
	datas[1]=(data&0XFF00)>>8;
	datas[2]=(data&0XFF);
	chack=(order8+datas[1]+datas[2]);
	datas[3]=(chack&0XFF);
for(i=0;i<4;i++)
	{
		
//		/* 发送一个字节数据到USART */
//	USART_SendData(TSDA_Usart,datas[i]);
	Usart_SendByte(pUSARTx,datas[i] );
	Delay_ms(2);
//	/* 等待发送数据寄存器为空 */
//	while (USART_GetFlagStatus(TSDA_Usart, USART_FLAG_TXE) == RESET);
//		
//		SysTick_Delay_ms(5);
	}
//	checkorder=USART_ReceiveData(TSDA_Usart);
//	if(checkorder==order8)return 1;
//	else return 0;
}



