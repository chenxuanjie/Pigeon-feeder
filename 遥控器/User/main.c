/*
			ң����
	״̬1������ģʽ
			��1����������ģʽ
			��2��ι��ʱ���趨
			��3����������
	״̬2������ģʽ
			��1���ٶȿ���ģʽ
			��2��ι�ϰ�ť
			��3����ͣ��
			��4�������ƣ��������� */
/*
Program: ң����
History:
	2023/4/17	Shane	17th release
*/

#include "stm32f10x.h"                  // Device header
#include "ADC1.h"
#include "Delay.h"
#include "DMA1.h"
#include "Encoder.h"
#include "Key.h"
#include "Switch.h"
#include "main.h"
#include "NRF24L01.h"
#include "OLED.h"
#include "Rocker.h"
#include "Buzzer.h"
#include "TIM3.h"
#include "Internal_Flash.h"
#include "State1.h"
#include "State2.h"
#include "State3.h"
#include <stdio.h>

uint8_t RockerNum, RockerNum_X, RockerNum_Y, KeyNum, SwitchNum, Feeding_AutoFlag;
uint8_t FeedChose, FeedSwitch=RESET, Select=2, LastSelect, State=10, State1;
uint8_t State, StateChangeFlag;
uint16_t Voltage;
uint32_t FeedTemp;
int16_t EncoderNum;

uint16_t StoreArray[STORENUM] = {0};

machine feeder1, feeder2, feeder3;

void Init(void);
void Voltage_Get(void);
void StoreData(void);
void ReadData(void);
void Transmit(void);
void GetData(void);
void Get_State(void);
uint8_t State2(void);
void CursorControl(uint8_t State);
void FeedMode(void);
void Normal_IRQHandler(void);
void State_Management(uint8_t inputState, uint8_t *StateChangeFlag);
void Back(void);
void Turn(uint8_t inputState);
uint8_t Get_FirstState(void);
uint8_t Get_SecondState(void);

int main(void)
{
	Init();
	ReadData();
	while (1)
	{
		OLED_ShowNum(4, 15, Feeding_AutoFlag, 2);
		OLED_ShowNum(4, 1, NRF24L01_GetData(NORMAL_TRANSMIT), 2);

		GetData();
		Get_State();
		Transmit();
		switch (Get_FirstState())
		{
			//�Ҳ�ť�������²�ʱ������ģʽ
			case STATE_SETTING:
			{
				switch (State)
				{
					case STATE_SETTING_ORIGIN:
						StateChangeFlag = Setting_OriginMode(&State, Select, KeyNum);break;
					case STATE_SETTING_FEED:
						FeedMode();break;
					case STATE_SETTING_BUZZER:
						StateChangeFlag = Setting_BuzzerMode(&State, Select, KeyNum);break;
					case STATE_SETTING_OTHER:
						StateChangeFlag = Setting_OtherMode(&State, Select, KeyNum);break;
				}
			}break;
			//�Ҳ�ť�������в�ʱ������ģʽ
			case STATE_CONTROL:
			{
				State2();
			}break;
			//�Ҳ�ť�������ϲ�ʱ������ģʽ
			case STATE_DEBUG:
			{
				Show(KeyNum, SwitchNum, EncoderNum, RockerNum);
				//Feeding_DebugFeedSet(&feeder1, &feeder2, &feeder3);
			}break;
		}
		CursorControl(State);
		State_Management(State, &StateChangeFlag);
	}
}

void Voltage_Get(void)
{
	Voltage = (float)ORINGINAL_VOLTAGE/4096*3.3*10/2*5;
	OLED_printf(1, 13, "%d.%dV",Voltage/10, Voltage%10);	
}
void GUI_Init(void)
{
	
}

void Init(void)
{
	TIM3_Init();
	TIM3_SetIRQHandler(*(Normal_IRQHandler));
	Buzzer_Init();
	Switch_Init();
	Key_Init();
	Rocker_Init();
	Encoder_Init();
	OLED_Init( );
	NRF24L01_Init();	
	GUI_Init();
}

/**
  * @brief  Get the tens place in variable "State".
  * @param  None
  * @retval None
  */
uint8_t Get_FirstState(void)
{
	return State/10*10;
}

/**
  * @brief  Get the ones place in variable "State".
  * @param  None
  * @retval None
  */
uint8_t Get_SecondState(void)
{
	return State%10;
}

/**		
  * @brief  �����ơ�
  * @param  ?
  * @retval ?
  */
void CursorControl(uint8_t inputState)
{
	inputState = inputState /10*10;
	if (inputState == STATE_SETTING)
	{
		//����ƶ�
		if (RockerNum_Y==UP_4 || KeyNum==KEY_UP)	//ѡ��λ��
				Select --;
		else if (RockerNum_Y==DOWN_4 || KeyNum==KEY_DOWN)	//ѡ��λ��
				Select ++;
	}
	else if (inputState == STATE_CONTROL)
	{
		//����ƶ�
		if (KeyNum==KEY_UP)	//ѡ��λ��
			Select --;
		if (KeyNum==KEY_DOWN)	//ѡ��λ��
			Select ++;
	}
	if (Select >= 5)
		Select = 2;
	else if (Select <= 1)
		Select = 4;
	//�����ʾ
	if (Select!= LastSelect)
	{
		OLED_ShowString(2, 15, "  ");
		OLED_ShowString(3, 15, "  ");
		OLED_ShowString(4, 15, "  ");
		OLED_ShowString(Select, 15, "<-");
		LastSelect = Select;
	}		
}

void Get_State(void)
{
	static uint8_t LastSwitchNum;
	LastSwitchNum = SwitchNum;
	SwitchNum = Switch_GetNum();	
	if (SwitchNum != LastSwitchNum)
	{
		OLED_Clear();
		if ((SwitchNum&0x02) != 0)	//����ť�ӿ������²�
		{
			OLED_ShowString(Select, 15, "<-");
			State = STATE_SETTING + Get_SecondState();		
		}
		else if ((SwitchNum&0x08) != 0)	//����ť�ӿ������м䲦
		{
			OLED_ShowString(Select, 15, "<-");
			State = STATE_CONTROL + Get_SecondState();
		}
		else 							//����ť�ӿ������ϲ�
			State = STATE_DEBUG + Get_SecondState();
	}
}

uint8_t State2(void)
{
	OLED_ShowString(1, 2, "CONTROL");
	OLED_ShowString(3, 1, "HandFeed:");
	//������ͬ������ʱ��
	if (FeedSwitch==SET && Feeding_AutoFlag==RESET)
	{
		OLED_ShowNum(3, 10, FeedTemp/1000, 3);
		OLED_ShowChar(3, 13, 's');
		if (feeder1.Time_ms > feeder2.Time_ms)
			FeedTemp = feeder1.Time_ms;
		else 
			FeedTemp = feeder2.Time_ms;
		if (feeder3.Time_ms > FeedTemp)
			FeedTemp = feeder3.Time_ms;
	}
	else
		OLED_ShowString(3, 10, " OFF");
	Voltage_Get();
	NRF24L01_SetBuf(NORMAL_TRANSMIT, FeedSwitch + 1);	//����׼�����͵�ι�Ͽ���		
	switch(Select)
	{
		//�Զ�ι��
		case 2:
		{
			if (KeyNum==KEY_ENTER && FeedSwitch==RESET)	//�Զ����ֶ�����ͬʱ����
				Feeding_AutoFeedSet(&Feeding_AutoFlag, 6);			
		}break;
		case 3:
			if ((KeyNum==KEY_LEFT || KeyNum==KEY_ENTER) && Feeding_AutoFlag==RESET)
			{
				FeedSwitch =! FeedSwitch;
				if (FeedSwitch == SET)
				{
					feeder1.Time_ms = feeder1.Time_s * 1000;
					feeder2.Time_ms = feeder2.Time_s * 1000;
					feeder3.Time_ms = feeder3.Time_s * 1000;
				}
			}
			break;
	}
	Feeding_AutoFeedShow(Feeding_AutoFlag);
	return 0;
}
void GetData(void)
{
	KeyNum = Key_GetNum();
	if (KeyNum != 0 && Get_BeepChose()==SET)
		Buzzer_SetBeep(Get_BeepNum(), 100);
	EncoderNum += Encoder_Get();
	if (State == STATE_SETTING)
	{
		//ң�����ԡ��������ҡ����ж�
		RockerNum = Rocker_GetNum();	//ֻ��ҡ�˲�������ͬλ�ã�������ֵ������ִֵ��һ�κ����0
		RockerNum_X = (RockerNum&0xF0)>>4;
		RockerNum_Y = RockerNum&0x0F;
	}
	else if (State == STATE_DEBUG)
	{
 		RockerNum = Rocker_GetNum2Loop();	//��ֵ���ϸ��£��Ҳ���0	
		RockerNum_X = (RockerNum&0xF0)>>4;
		RockerNum_Y = RockerNum&0x0F;
		//Ӧ�ò��ϵ�XY��ʾ ���ڵ���
		OLED_ShowNum(1, 3, RockerNum_X, 2);	//Ӧ���ϵ�X
		OLED_ShowNum(1, 8, RockerNum_Y, 2);	//Ӧ���ϵ�Y
	}
}



void FeedMode(void)
{	
	OLED_ShowString(2, 1, "Time1:");
	OLED_ShowString(3, 1, "Time2:");
	OLED_ShowString(4, 1, "Time3:");
	OLED_ShowString(2, 10, "s");
	OLED_ShowString(3, 10, "s");
	OLED_ShowString(4, 10, "s");
	OLED_ShowNum(2, 7, feeder1.Time_s, 3);
	OLED_ShowNum(3, 7, feeder2.Time_s, 3);
	OLED_ShowNum(4, 7, feeder3.Time_s, 3);
	//Խ���ж�
	feeder1.Time_s %= 256;
	if (FeedChose == RESET)
	{
		if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
			feeder1.Time_s ++;
		else if (KeyNum==KEY_PIN2_NUM && feeder1.Time_s>0)
			feeder1.Time_s --;

		feeder1.Time_s += Encoder_Get();
		feeder2.Time_s = feeder1.Time_s;
		feeder3.Time_s = feeder1.Time_s;
	}
	else
	{
		switch(Select)
		{
			case 2:		//ι��1ʱ��			
				if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					feeder1.Time_s ++;
				else if (KeyNum == KEY_PIN2_NUM)
					feeder1.Time_s --;
				feeder1.Time_s += Encoder_Get();
				break;
			case 3:		//ι��2ʱ��				
				if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					feeder2.Time_s ++;
				else if (KeyNum == KEY_PIN2_NUM)
					feeder2.Time_s --;
				feeder2.Time_s += Encoder_Get();
				break;
			case 4:		//ι��3ʱ��				
				if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					feeder3.Time_s ++;
				else if (KeyNum == KEY_PIN2_NUM)
					feeder3.Time_s --;
				feeder3.Time_s += Encoder_Get();
				break;
		}	
	}	
}


void State_Management(uint8_t inputState, uint8_t *StateChangeFlag)
{
	//����
	if (KeyNum==KEY_BACK && Get_FirstState()==STATE_SETTING)
	{
		Back();
		State = inputState /10*10;
	}
	//��ת
	else if (*StateChangeFlag)
		Turn(inputState);
	//�����־λ
	*StateChangeFlag = 0;
}

/**
  * @brief  Back to last state.
  * @param  None
  * @retval None
  */
void Back(void)
{
	OLED_Clear();
	OLED_ShowString(Select, 15, "<-");
	CloseBuzzer();		//��ֹFlashд��ʱ���������쳣
	StoreData();
}
/**
  * @brief  Turn to any state.
  * @param  None
  * @retval None
  */
void Turn(uint8_t inputState)
{
	Back();
	switch(inputState)
	{
		case STATE_SETTING_BUZZER:
			OLED_ShowString(2, 1, "1.Buzzer:");
			OLED_ShowString(3, 1, "2.KeyMusic:");
			OLED_ShowString(4, 1, "3.Back");
			break;
		case STATE_SETTING_OTHER:
			OLED_ShowString(2, 1, "1.");
			OLED_ShowString(3, 1, "2.");
			OLED_ShowString(4, 1, "3.Back");			
	}	
}

void StoreData(void)
{
	StoreArray[BuzzerChoseStore] = Get_BeepChose();
	StoreArray[BeepNumStore] = Get_BeepNum();
	StoreArray[FeedTime1Store] = feeder1.Time_s;
	StoreArray[FeedTime2Store] = feeder2.Time_s;
	StoreArray[FeedTime3Store] = feeder3.Time_s;
	StoreArray[FeedChoseStore] = FeedChose;
	Flash_WriteArray_HalfWord(0x00, StoreArray, STORENUM);
}

void ReadData(void)
{
	Flash_ReadArray_HalfWord(0x00, StoreArray, STORENUM);
	Set_BeepChose(StoreArray[BuzzerChoseStore]);
	Set_BeepNum(StoreArray[BeepNumStore]);
	FeedChose = StoreArray[FeedChoseStore];
	feeder1.Time_s = StoreArray[FeedTime1Store];
	feeder2.Time_s = StoreArray[FeedTime2Store];
	feeder3.Time_s = StoreArray[FeedTime3Store];
}
/**
  * @brief  �����ݷ��ͳ�ȥ��
  * @param  ��
  * @retval ��
  */
void Transmit(void)
{
	if (KeyNum != 0)
		NRF24L01_SetBuf(KEY_TRANSMIT, KeyNum);
	if (SwitchNum != 0)
		NRF24L01_SetBuf(SWITCH_TRANSMIT, SwitchNum);
	
	NRF24L01_SetBuf(ROCKER_TRANSMIT, RockerNum);
	NRF24L01_SetBuf(ENCODER_TRANSMIT, EncoderNum);
	NRF24L01_SetBuf(FEEDTIME1_TRANSMIT, feeder1.Time_ms/1000);
	NRF24L01_SetBuf(FEEDTIME2_TRANSMIT, feeder2.Time_ms/1000);
	NRF24L01_SetBuf(FEEDTIME3_TRANSMIT, feeder3.Time_ms/1000);
	NRF24L01_TransmitBuf();
	NRF24L01_SetBuf(NORMAL_TRANSMIT, 0);
}



void Normal_IRQHandler(void)	//1ms
{
	static uint16_t T3_Count[2];
	T3_Count[0]++;
	if ( T3_Count[0] >= 20)
	{
		T3_Count[0] = 0;
		Key_Loop();
		Switch_Loop();
		if (State==STATE_SETTING)
			Rocker_Loop();
	}
	
	//������
	if (T3_BeepTime > 0)
	{
		T3_BeepTime --;
	}
	else
		CloseBuzzer();
	
	//ι��ʱ��
	if (FeedSwitch==SET)
	{
		if (feeder1.Time_ms > 0) feeder1.Time_ms --;
		if (feeder2.Time_ms > 0) feeder2.Time_ms --;
		if (feeder3.Time_ms > 0) feeder3.Time_ms --;
	}
	if (feeder1.Time_ms==0 && feeder2.Time_ms==0 && feeder3.Time_ms==0)
		FeedSwitch = RESET;
	//��˸
	T3_Count[1] ++;
	if (Feeding_AutoFlag>0 && T3_Count[1]>500)
	{
		T3_Count[1] = 0;
		Feeding_AutoFlag --;
	}
}
