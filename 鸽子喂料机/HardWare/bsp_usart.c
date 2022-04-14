#include "bsp_usart.h"
#include "stm32f10x_usart.h"
#include "led.h"
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);	  

	
	
	
	
	
	GPIO_InitTypeDef GPIO_InitStructure4;
	USART_InitTypeDef USART_InitStructure4;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd4(DEBUG_USART_GPIO_CLK4, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd4(DEBUG_USART_CLK4, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure4.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN4;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT4, &GPIO_InitStructure4);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure4.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN4;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT4, &GPIO_InitStructure4);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure4.USART_BaudRate = DEBUG_USART_BAUDRATE4;
	// ���� �������ֳ�
	USART_InitStructure4.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure4.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure4.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure4.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure4.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx4, &USART_InitStructure4);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx4, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx4, ENABLE);	

}


//����һ���ֽ�
void Usart_SendByte(USART_TypeDef* pUSARTx, int8_t data)
{
	USART_SendData(pUSARTx, data);
	
	//���ڽ��ձ�־λ  USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET )
	{LEDO_ON();}
}


//���������ֽڵ�����
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t  temp_h,temp_l;
	
	
	temp_h = (data&0xff00)>>8;
	temp_l = (data&0x00ff);
	
	USART_SendData(pUSARTx, temp_h);
	//���ڽ��ձ�־λ  USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	
	USART_SendData(pUSARTx, temp_l);
	//���ڽ��ձ�־λ  USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}


//����8λ���ݵ�����

void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		Usart_SendByte( pUSARTx,array[i] );
	}
	
	//���TC��־λ,�����ж�һ�����ֽ�,���жϵ��ֽڱ�־λ��ͬ
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
	
}


//�����ַ���
void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str)
{
	
	uint8_t i=0;
	do
	{
		Usart_SendByte( pUSARTx,*(str+i) );
		i++;
	}while(*(str+i)!= '\0');
		//���TC��־λ,�����ж�һ�����ֽ�,���жϵ��ֽڱ�־λ��ͬ
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
	
	
}

/////�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
//int fputc(int ch, FILE *f)
//{
//		/* ����һ���ֽ����ݵ����� */
//		USART_SendData(DEBUG_USARTx4, (uint8_t) ch);
//		
//		/* �ȴ�������� */
//		while (USART_GetFlagStatus(DEBUG_USARTx4, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

/////�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
//int fgetc(FILE *f)
//{
//		/* �ȴ������������� */
//		while (USART_GetFlagStatus(DEBUG_USARTx4, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(DEBUG_USARTx4);
//}

//		Usart_SendStr(DEBUG_USARTx, "CXJ�����\n");
//		Usart_SendByte(DEBUG_USARTx, 100);
//	Usart_SendArray(DEBUG_USARTx, a,10);
//		Usart_SendHalfWord(DEBUG_USARTx, 0xff56);
//	EXTI_Key_Config();   //����1�жϳ�ʼ��
//	printf("hhh\n");


