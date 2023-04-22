/*
	�����յ������鼰����ֵ�������������main.h��
	
	NRF24L01����Ԫ�أ�
		1.NORMAL_TRANSMIT�����档
		2.KEY_TRANSMIT��������
		3.SWITCH_TRANSMIT��ť�ӿ��ء�
		4.ROCKER_TRANSMIT��ҡ�ˡ�
		5.ENCODER_TRANSMIT����ת��������
*/

/*
Program: pigeon-feeder
History:
	2023/4/17	Shane	17th release
	1. change speed() -> Robot_SetSpeed()
*/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "main.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "TSDA.H"
#include "bsp_usart.h"
#include "LED.H"
#include "ROBOT.H"
#include "Relay.H"
#include "Hcsr04.h"
#include "TIM2.h"
#include "Feed.h"
#include "TrackingLine.h"
#include "State3.h"

int16_t Set_Data,Set_Speed=20, Num;		//Set_Speed ��Ĭ�ϵ��ٶȣ�����ҡ�˽Ƕȣ��趨������ո��ٶȰٷֱ���ת����λ��cm/s
uint8_t Flag, Hcsr04_StartFlag,LINK_FLAG, Error = 0;
uint8_t X, Y, Value, Feeding_AutoFlag=RESET, Feeding_ManualFlag=RESET, TrackLineFlag=RESET;
int16_t SpeedRight_Robot=0,SpeedLeft_Robot=0;
int8_t LeftCalibration=1,RightCalibration=-1 ;
uint8_t DataReset=1,StopFlag=0;
uint8_t Left=0,Right=0, State=1;
uint32_t Timeout=0;
uint32_t T2Count[3];

machine feeder1, feeder2, feeder3;

void Init(void);
void Data_Analyse(uint8_t *Rocker_XY);
void State1(void);
void State2(void);
void State3(void);
void While_Init(void);
void Remote_Managment(void);
void TrackingLine_Managment(void);
void HandleData(void);
	
int main(void)
{	
	Init();
	while (1)
	{
		While_Init();		
		//ѭ��ģʽ
		if (TrackLineFlag == SET)
		{
//			TrackLineFlag = !TrackingLine_Test();
			TrackLineFlag = !(TrackingLine(&feeder1, &feeder2, &feeder3));
//			TrackingLine_Managment();
		}
		else
		{
			Remote_Managment();
			switch (State)		//ң�����Ͻǲ��˵�λ
			{
				case SETTINGSTATE://��
					State3();
					break;
				case CONTROLSTATE://��
					State2();
					break;
				case DEBUGSTATE://��
					State1();
					break;
			}
			/*********************�������************************************/
			SpeedLeft_Robot=Get_LeftDirection()*LeftCalibration*SpeedConversion(Robot_GetSpeedLeft());
			SpeedRight_Robot=Get_RightDirection()*RightCalibration*SpeedConversion(Robot_GetSpeedRight());
			Robot_Move(SpeedLeft_Robot,SpeedRight_Robot);
		}
		HandleData();
		//ֹͣ
		if(StopFlag==SET ){Robot_SetSpeed(0);}

		LEDO_OFF();			
	}
}

/**		��ʼ��
  * @brief  Init pigeon-feeder.
  * @param  None
  * @retval None
  */
void Init(void)
{
	TrackingLine_Init();
	LED_Init();
	OLED_Init( );
	NRF24L01_Init();
	RX_Mode();
	Hcsr04_Init();
	TIM2_Init();
	USART_Config();
	Feeding_Init();	
	Robot_Init();
	OLED_printf(3, 1, "1:   2:   3:");
}

void TrackingLine_Managment(void)
{
//	switch(NRF24L01_GetData(NORMAL_TRANSMIT))
//	{
//		case AUTO_FEED_ON:	//�Զ�ι�Ͽ�
//			Get_FeedTime(&feeder1, &feeder2, &feeder3, Feeding_GetRemoteAutoTimes());
//			Feeding_AutoFlag = SET;
//			Feeding_SetRemoteAutoTimes(Feeding_GetRemoteAutoTimes()+1);
//		break;
//		case AUTO_FEED_OFF:	//�Զ�ι�Ϲ�
//			Feeding_ResetFeeder(&feeder1, &feeder2, &feeder3); 
//			Feeding_AutoFlag = RESET;
//			Feeding_SetRemoteAutoTimes(0);
//		break;
//	}
//	switch (NRF24L01_GetData(TRACKINGLINE_TRANSMIT))
//	{
//		case TRACKINGLINE_ON:	//�Զ�ѭ������
//			TrackLineFlag = SET;
//		break;
//		case TRACKINGLINE_OFF:
//			TrackLineFlag = RESET;
//		break;
//	}
}

void Remote_Managment(void)
{	
	//�����ֶ�����ʱ�������
	if (Feeding_AutoFlag==RESET && Feeding_ManualFlag==RESET)
		StartFeed(feeder1.Time, feeder2.Time, feeder3.Time);
	else if (Feeding_AutoFlag==SET && Feeding_ManualFlag==RESET)	//�����Զ�����
		StartFeed(feeder1.SecondAutoTime_ms, feeder2.SecondAutoTime_ms, feeder3.SecondAutoTime_ms);
	//ι�Ͻ�������0��־λ
	if (feeder1.SecondAutoTime_ms==0 && feeder2.SecondAutoTime_ms==0 && feeder3.SecondAutoTime_ms==0)
		Feeding_AutoFlag = RESET;		
}

void While_Init()
{
	OLED_ShowHexNum(1,15,Value,2);  //ң������
	OLED_ShowNum(2,1,feeder1.SecondAutoTime_ms,4);
	OLED_ShowNum(2,6,feeder2.SecondAutoTime_ms,4);
	OLED_ShowNum(2,11,feeder3.SecondAutoTime_ms,4);
	//��ʱ��ȡ����ʶ������
	Get_BirdNum(&Timeout);
	MonitorFeed(&Hcsr04_StartFlag);
	Flag = NRF24L01_ReceiveData();
	//NRF�Ĵ���
	OLED_ShowHexNum(4, 1, NRF24L01_ReadByte(STATUS), 2);
	OLED_ShowHexNum(4, 4, NRF24L01_ReadByte(EN_AA), 2);
	OLED_ShowHexNum(4, 7, NRF24L01_ReadByte(EN_RXADDR), 2);
	OLED_ShowHexNum(4, 10, NRF24L01_ReadByte(RX_ADDR_P0), 2);
	OLED_ShowHexNum(4, 13, NRF24L01_ReadByte(FIFO_STATUS), 2);

	if (NRF24L01_GetData(NORMAL_TRANSMIT) != 0)
	OLED_ShowHexNum(1,1,NRF24L01_GetData(NORMAL_TRANSMIT),2);

	if (Flag)
		Flag = 0;
//	Delay_ms(8);
	Data_Analyse(&Value);	
}

void State1(void)
{
		if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM+SWITCH2_PIN1_NUM)	//��ť����05������ͨ��������
	{
	/*********************ҡ�˼��************************************/
		//����ת��
//		Robot_SelfTurn(X, Y, Set_Speed);
		//����ת��
		Robot_DifferentialTurn(X, Y, Set_Speed);
	/********************�������************************************/
	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
//			case EmergencyFault	:	Robot_SetSpeed(0);break;//��ͣ
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//����
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//�ر�
			case KEY_PIN3_NUM		:	Robot_SetSpeed(0);break;//��ͣ
			case KEY_PIN4_NUM		:	break;//ʹ����ת������
			case KEY_PIN5_NUM		:	Set_Speed=NRF24L01_GetData(ENCODER_TRANSMIT);break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		
		}

	}
}

void State2(void)
{	
		/* �л�����ģʽʱ���ǵð�����������ֹͣ�����������л���ͨģʽ���±�����һ�ΰ�������*/
	if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM)//��ť����04����У׼ģʽ,У׼�������ҡ�˿���	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case KEY_PIN4_NUM		:	LeftCalibration=-1;RightCalibration=1;break;
			case KEY_PIN5_NUM		:	LeftCalibration=1;RightCalibration=-1;break;
			case KEY_PIN6_NUM		:	LeftCalibration=1;RightCalibration=1;break;
			case KEY_PIN7_NUM		:	LeftCalibration=-1;RightCalibration=-1;break;
		}
		
		
//		/*********************��ת���������************************************/
//	
//		if(NRF24L01_GetData(KEY_TRANSMIT)==KEY_PIN4_NUM)
//		{
//			Set_Data=NRF24L01_GetData(ENCODER_TRANSMIT);
//			Robot_SetSpeed(Set_Data);
//		}

	
/*********************ι����ѭ��************************************/		
		switch( NRF24L01_GetData(NORMAL_TRANSMIT))
		{
			case FEED_OFF:	//ι������
				if(Feeding_AutoFlag == RESET)
					Relay_Set(ALL, RESET);
				Feeding_ManualFlag = RESET;
			break;
			case FEED_ON:	//ι������
				Relay_Set(ALL, SET);
				Feeding_ManualFlag = SET;
			break;
			case AUTO_FEED_ON:	//�Զ�ι�Ͽ�
				Get_FeedTime(&feeder1, &feeder2, &feeder3, Feeding_GetRemoteAutoTimes());
				Feeding_AutoFlag = SET;
				Feeding_SetRemoteAutoTimes(Feeding_GetRemoteAutoTimes()+1);
			break;
			case AUTO_FEED_OFF:	//�Զ�ι�Ϲ�(���رգ���Ҫ����һ����������ʶ��ǰҪι�ϵ�����)
				Feeding_ResetFeeder(&feeder1, &feeder2, &feeder3); 
				Feeding_AutoFlag = RESET;
				Feeding_SetRemoteAutoTimes(0);
			break;
		}
		switch (NRF24L01_GetData(TRACKINGLINE_TRANSMIT))
		{
			case TRACKINGLINE_ON:	//�Զ�ѭ������
				TrackLineFlag = SET;
			break;
			case TRACKINGLINE_OFF:
				TrackLineFlag = RESET;
			break;
		}
		
		
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	Robot_SetSpeed(0);break;//��ͣ
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//����
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//�ر�
			case KEY_PIN3_NUM		:	Robot_SetSpeed(0);break;//��ͣ
		}
		
		
	}

void State3(void)
{
	switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault		:	Robot_SetSpeed(0);break;//��ͣ
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//����
			case KEY_PIN2_NUM		:	break;
			case KEY_PIN3_NUM		:	Robot_SetSpeed(0);break;//��ͣ
			case KEY_PIN4_NUM		:	break;
			case KEY_PIN5_NUM		:	break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		}
}

void Data_Analyse(uint8_t *Rocker_XY)
{
	static uint8_t SwitchNum, LastSwitchNum;	
	*Rocker_XY = NRF24L01_GetData(ROCKER_TRANSMIT);
	//X��ʾʵ�ʲ���ʱҡ�����Ҳ�(Ӧ�ò�)
	X = (Value&0xF0)>>4;
	Y = Value&0x0F;
	
	LastSwitchNum = SwitchNum;
	//Stateֵ�ĸı�
	SwitchNum = NRF24L01_GetData(SWITCH_TRANSMIT);	
	if (SwitchNum != LastSwitchNum)
	{
		if ((SwitchNum&0x02) != 0)	//����ť�ӿ������²�
		{
			State = SETTINGSTATE;		
		}
		else if ((SwitchNum&0x08) != 0)	//����ť�ӿ������м䲦
		{
			State = CONTROLSTATE;
		}
		else 							//����ť�ӿ������ϲ�
			State = DEBUGSTATE;
	}
}

void HandleData(void)
{
	static uint8_t NormalNum, LastLinkFlag, NowLinkFlag;
	NormalNum = NRF24L01_GetData(NORMAL_TRANSMIT);

	//�Ͽ����ӵ����
	LastLinkFlag = NowLinkFlag;
	NowLinkFlag = LINK_FLAG;
	if (LastLinkFlag==0 && NowLinkFlag==1)
	StopFlag=RESET;
	else if (LastLinkFlag==1 && NowLinkFlag==0) //ͨѶ�Ͽ�
	{StopFlag=SET;
	}
	//���淢��λ���λ��Ϊ���ӱ�־λ	
	if ((NormalNum&0x80)!=RESET && Error==SET)	//��������λû�б����
	{
		LINK_FLAG = 0;
	}
	else if ((NormalNum&0x80)==RESET && Error==SET)//��������
	{

		LINK_FLAG = 1;
		Error = RESET;		//���Ǵ������
		T2Count[2] = 0;		
		NRF24L01_SetRXBuf(NORMAL_TRANSMIT, (NormalNum |= 0x80));	//��λ��־λ�����´��Ƿ����

	}
	
}

void TIM2_IRQHandler(void)
{

	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
		//0.5s����Ӧ����Ͽ�����
		T2Count[2] ++;
		if (T2Count[2] >= 500)
		{
			Error = SET;
		}
		else
			Error = RESET;
		
		T2Count[0]++;
		if (T2Count[0] >= 100)
		{
			T2Count[0] = 0;
			if ((Hcsr04_StartFlag&0x07) < 4)	//С��4����
				Hcsr04_StartFlag ++;
			else 
				Hcsr04_StartFlag = 0x01;		//����4����Ϊ1
			Hcsr04_StartFlag |= 0x08;			//���������ʹ��λ
		}

        //Timeout����0�����ȡ��������
        if (Timeout > 0) 
			Timeout --;
		//ι����ʱ��
		if (feeder1.Time > 0) feeder1.Time --;
		if (feeder2.Time > 0) feeder2.Time --;
		if (feeder3.Time > 0) feeder3.Time --;
		//�Զ����ϵ�ʱ��
		if (feeder1.SecondAutoTime_ms > 0) feeder1.SecondAutoTime_ms --;
		if (feeder2.SecondAutoTime_ms > 0) feeder2.SecondAutoTime_ms --;
		if (feeder3.SecondAutoTime_ms > 0) feeder3.SecondAutoTime_ms --;
		//ѭ��ģʽ�µ���ʱʱ��
		if (Robot_DelayGet() != 0)
			Robot_DelaySet(Robot_DelayGet()-1);
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	
}
