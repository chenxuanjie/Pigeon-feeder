#include "bsp_usart.h"
#include "stm32f10x_usart.h"
#include "led.h"
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	  

	
	
	
	
	
	GPIO_InitTypeDef GPIO_InitStructure4;
	USART_InitTypeDef USART_InitStructure4;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd4(DEBUG_USART_GPIO_CLK4, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd4(DEBUG_USART_CLK4, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure4.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN4;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT4, &GPIO_InitStructure4);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure4.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN4;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT4, &GPIO_InitStructure4);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure4.USART_BaudRate = DEBUG_USART_BAUDRATE4;
	// 配置 针数据字长
	USART_InitStructure4.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure4.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure4.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure4.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure4.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx4, &USART_InitStructure4);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTx4, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx4, ENABLE);	

}


//发送一个字节
void Usart_SendByte(USART_TypeDef* pUSARTx, int8_t data)
{
	USART_SendData(pUSARTx, data);
	
	//串口接收标志位  USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET )
	{LEDO_ON();}
}


//发送两个字节的数据
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t  temp_h,temp_l;
	
	
	temp_h = (data&0xff00)>>8;
	temp_l = (data&0x00ff);
	
	USART_SendData(pUSARTx, temp_h);
	//串口接收标志位  USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	
	USART_SendData(pUSARTx, temp_l);
	//串口接收标志位  USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}


//发送8位数据的数组

void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		Usart_SendByte( pUSARTx,array[i] );
	}
	
	//检查TC标志位,用于判断一连串字节,与判断单字节标志位不同
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
	
}


//发送字符串
void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str)
{
	
	uint8_t i=0;
	do
	{
		Usart_SendByte( pUSARTx,*(str+i) );
		i++;
	}while(*(str+i)!= '\0');
		//检查TC标志位,用于判断一连串字节,与判断单字节标志位不同
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
	
	
}

/////重定向c库函数printf到串口，重定向后可使用printf函数
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到串口 */
//		USART_SendData(DEBUG_USARTx4, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(DEBUG_USARTx4, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

/////重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//		/* 等待串口输入数据 */
//		while (USART_GetFlagStatus(DEBUG_USARTx4, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(DEBUG_USARTx4);
//}

//		Usart_SendStr(DEBUG_USARTx, "CXJ别卷啦\n");
//		Usart_SendByte(DEBUG_USARTx, 100);
//	Usart_SendArray(DEBUG_USARTx, a,10);
//		Usart_SendHalfWord(DEBUG_USARTx, 0xff56);
//	EXTI_Key_Config();   //按键1中断初始化
//	printf("hhh\n");


