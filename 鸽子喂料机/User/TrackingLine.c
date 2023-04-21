#include "stm32f10x.h"    // Device header
#include "TrackingLine.h"
#include "NRF24L01.h"
#include "main.h"
#include "OLED.h"
#include "Robot.h"
#include "Feed.h"

uint8_t Digital_StopFlag;

/**
  * @brief  �Ҷȴ�������ʼ������
  * @param  ��
  * @retval ��
  */
void TrackingLine_Init(void)
{
	RCC_APB2PeriphClockCmd(DIGITAL_APBPEIRPH, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//��ʼ��GPIOC
	GPIO_InitStructure.GPIO_Pin = DIGITAL_1_PIN | DIGITAL_2_PIN | DIGITAL_3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DIGITAL_GPIO1, &GPIO_InitStructure);
	//��ʼ��GPIOA
	GPIO_InitStructure.GPIO_Pin = DIGITAL_4_PIN | DIGITAL_5_PIN;
	GPIO_Init(DIGITAL_GPIO2, &GPIO_InitStructure);
}

/**
  * @brief  ��ȡ�Ҷȴ���������������
  * @param  DigitalPin��Ҫ��ȡ������
  * @retval ���ŵ���ֵ��������Ĳ���ָ�������ţ��򷵻�2����ʾ����
  */
uint8_t TrackingLine_ReadDigital(uint16_t DigitalPin)
{
	if (DigitalPin==DIGITAL_1_PIN || DigitalPin==DIGITAL_2_PIN || DigitalPin==DIGITAL_3_PIN)
		return GPIO_ReadInputDataBit(DIGITAL_GPIO1, DigitalPin);
	else if (DigitalPin==DIGITAL_4_PIN || DigitalPin==DIGITAL_5_PIN)
		return GPIO_ReadInputDataBit(DIGITAL_GPIO2, DigitalPin);
	else
		return 2;
}

void TrackingLine_Test(void)
{
		OLED_ShowNum(1,1,TrackingLine_ReadDigital(DIGITAL_1_PIN),2);
		OLED_ShowNum(1,4,TrackingLine_ReadDigital(DIGITAL_2_PIN),2);
		OLED_ShowNum(1,7,TrackingLine_ReadDigital(DIGITAL_3_PIN),2);
		OLED_ShowNum(1,10,TrackingLine_ReadDigital(DIGITAL_4_PIN),2);
		OLED_ShowNum(1,13,TrackingLine_ReadDigital(DIGITAL_5_PIN),2);
}

/**     �ж��Ƿ��˳�ѭ��ģʽ
  * @brief  Judge if exit the Trackingline mode.
  * @param  None
  * @retval return 0 continue Trackingline mode;
			return 1 exit Trackingline mode. 
  */
uint8_t TrackingLine_IfExit(void)
{
	NRF24L01_ReceiveData();
	if (NRF24L01_GetData(NORMAL_TRANSMIT) == TRACKINGLINE_OFF)
	{
		Robot_Stop();
		return 1;
	}
	else
		return 0;
}

/**			ѭ��ģʽ����
  * @brief  Start Tracking mode.
  * @param  None
* @retval Flag: 0: still in the Tracking mode.
				1: exit the Tracking mode. 
  */
uint8_t TrackingLine(void)
{
	uint8_t Digital_Mode;
	//�Ƿ��л��ֶ�
	if (TrackingLine_IfExit())
		Digital_Mode = DIGITAL_STOP;
	else
		Digital_Mode = Digital_ModeJudge();
	switch (Digital_Mode)
	{
		case DIGITAL_STRAIGHT:		//����ֱ��
			return Digital_Straight();
		case DIGITAL_TURNLEFT:		//��ת
			return Digital_Turn(LEFT);
		case DIGITAL_TURNRIGHT:	//��ת
			return Digital_Turn(RIGHT);
		case DIGITAL_FEEDING:	//ι��ģʽ
			return Digital_Feeding();
		case DIGITAL_STOP:		//ѭ���������Զ��л�Ϊң��������ģʽ
			Robot_Stop();
			return 1;
		default: Robot_Stop();return 1;	//�������
	}
}

/**     ģʽ�ж�
  * @brief  mode will be judged and determinte how to run by a correct way.
  * @param  None.
* @retval return 0: Unexpected return.
		  return DIGITAL_STRAIGHT:  Go straight.
		  return DIGITAL_TURNLEFT:  Turn left.
		  return DIGITAL_TURNRIGHT: Turn right.
		  return DIGITAL_FEEDING:   Feeding mode.
		  return DIGITAL_STOP:		Stop robot and return the remote control mode.
  */
uint8_t Digital_ModeJudge(void)
{
	if (Digital_L2==RESET && Digital_R2==RESET)		//ֱ��
		return DIGITAL_STRAIGHT;
	else if (Digital_L2==SET && Digital_R2!=SET)	//��ת
		return DIGITAL_TURNLEFT;
	else if (Digital_L2!=SET && Digital_R2==SET)	//��ת
		return DIGITAL_TURNRIGHT;
	else if (Digital_L2==SET && Digital_R2==SET && Digital_StopFlag<=DIGITAL_FEEDINGTIMES)	//ι��
		return DIGITAL_FEEDING;
	else if (Digital_L2==SET && Digital_R2==SET && Digital_StopFlag>DIGITAL_FEEDINGTIMES)	//ֹͣ
		return DIGITAL_STOP;
	else		
		return 0;
	
}

/**     ����ֱ��
  * @brief  run forward straightly by recognizing  black line.
  * @param  
  * @retval return 0 meeting crossroad; return 1 in a straight road. 
  */
uint8_t Digital_Straight(void)
{
	if (Digital_L1==1 && Digital_M==0 && Digital_R1==0)	//ƫ��
	{
		Robot_TurnLeft(FRONT);
	}
	else if (Digital_L1==1 && Digital_M==1 && Digital_R1==0)	//΢΢ƫ��
	{
		Robot_TurnLeft(FRONT);
	}
	else if (Digital_L1==0 && Digital_M==0 && Digital_R1==1)	//ƫ��
	{
		Robot_TurnRight(FRONT);
	}
	else if (Digital_L1==0 && Digital_M==1 && Digital_R1==1)	//΢΢ƫ��
	{
		Robot_TurnRight(FRONT);
	}
	else if (Digital_L1==0 && Digital_M==1 && Digital_R1==0)	//����
	{
		Robot_Front();
	}
	else if (Digital_L1==0 && Digital_M==0 && Digital_R1==0)	//����ⲻ��
	{
		Robot_Front();
	}
	else 
		Robot_Front();
	return 0;
}

/**
  * @brief  ת�䡣���ô˺�����ת����ͣ0.5�롣
* @param  Direction:����LEFT����RIGHT���ҡ�
  * @retval ��
  */
uint8_t Digital_Turn(uint8_t Direction)
{
	if (Robot_StopTime(500) != 0)
		return 1;
	switch(Direction)
	{
		case LEFT:
		{
			while (!(Digital_L1==0 && Digital_M==0 && Digital_R1==1))	//ת���ұ���������
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(NI);
			Robot_StopTime(500);
			while (!(Digital_L1==0 && Digital_M==1 && Digital_R1==0))	//�м����������
			{
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(NI);
			}
		}break;
		case RIGHT:
		{
			while (!(Digital_L1==1 && Digital_M==0 && Digital_R1==0))	//ת�������������
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(SHUN);
			Robot_StopTime(500);
			while (!(Digital_L1==0 && Digital_M==1 && Digital_R1==0))	//�м����������
			{
				if (TrackingLine_IfExit())
					return 1;
				else
					Robot_Cirle(SHUN);
			}
		}break;
	/*ת��������жϣ��Ƿ��߹�ͷ*/
	// if (Digital_L1==0 && Digital_M==0 && Digital_R1==0)
	// {
	// 	SetPwm(50);
	// 	Robot_Front();
	// 	while (!(Digital_L1==0 && Digital_M==1 && Digital_R1==0))	//������һ����������ֹͣ���
	// 	{
	// 		switch(Direction)
	// 		{
	// 			case LEFT: Robot_TurnRight(FRONT);break;
	// 			case RIGHT:Robot_TurnLeft(FRONT);break;
	// 		}
	// 	}
	}
	if (Robot_StopTime(500) != 0)
		return 1;
	return 0;
}

uint8_t Digital_Feeding(void)
{
	uint32_t feederTime1=0, feederTime2=0, feederTime3=0;
	//ֹͣ1�룬׼������
	if (Robot_StopTime(1000) != 0)
		return 1;
	Get_FeedTime(&feederTime1, &feederTime2, &feederTime3);
	//����ι�ϣ�ֱ��ι�����
	while(feederTime1!=0 && feederTime2!=0 && feederTime3!=0)
	{
		if (TrackingLine_IfExit())
			return 1;
		else
			StartFeed(feederTime1, feederTime2, feederTime3);
	}
	Digital_StopFlag ++;	//ÿιһ���ϼ�һ
	//ֹͣ0.5�룬��������û������
	if (Robot_StopTime(500) != 0)
		return 1;	
	//������ǰ��һ�㣬����´μ�⵽����ι��
	Robot_DelaySet(1000);
	while (!(Robot_DelayGet()))
	{
		if (TrackingLine_IfExit())
			return 1;
		else		
			Digital_Straight();
	}
	return 0;
}
