#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "string.h"
#include "OLED.h"

uint16_t Data;
char USART_Temp[USART_MAX_TEMP];
uint8_t i;

void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	//�ж�ʹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  ��������Ե����Ӿ����õ�Ҫι�ϵ�ʱ�䡣
  * @param  
  * @retval 
  */
void USART1_GetBirdNum(uint8_t* Bird1, uint8_t* Bird2, uint8_t* Bird3)
{
    uint8_t j;
    i = 0;
    USART_SendData(USART1, 'a');
    for (j = 0; j < USART_RECIEVE_TEMP; j ++)
	{
//        USART_SendData(USART1, USART_Temp[j]);
//        OLED_ShowChar(1,j+1+3,USART_Temp[j]);
	}
//    if ( strcmp(USART_Temp, "100020003000") == 0)  //�ж��Ƿ���������
//        OLED_ShowNum(1,16,1,1);
//    else
//        OLED_ShowNum(1,16,0,1);
    //ι��ʱ��1
	if (USART_Temp[1]<'0' || USART_Temp[2]<'0' || USART_Temp[3]<'0')
		*Bird1 = 0;
	else
		*Bird1 = (USART_Temp[1]-'0')*100 + (USART_Temp[2]-'0')*10 + (USART_Temp[3]-'0');
	//ι��ʱ��2
	if (USART_Temp[5]<'0' || USART_Temp[6]<'0' || USART_Temp[7]<'0')
		*Bird2 = 0;
	else
		*Bird2 = (USART_Temp[5]-'0')*100 + (USART_Temp[6]-'0')*10 + (USART_Temp[7]-'0');
	//ι��ʱ��3
	if (USART_Temp[9]<'0' || USART_Temp[10]<'0' || USART_Temp[11]<'0')
		*Bird3 = 0;
	else
		*Bird3 = (USART_Temp[9]-'0')*100 + (USART_Temp[10]-'0')*10 + (USART_Temp[11]-'0');
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,*(str + k));
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)	
    k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		Data = USART_ReceiveData(USART1);
        USART_Temp[i++] = Data;
       if (i > USART_MAX_TEMP)
       {
          i = 0;
           USART_Temp[USART_RECIEVE_TEMP] = '\0';
       }
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)!=SET);
	}
	USART_ClearITPendingBit(USART1, USART_FLAG_RXNE);
}
