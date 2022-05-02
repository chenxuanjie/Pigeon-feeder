/**************************************************/
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
			��4�������ƣ���������
*/
/**************************************************/

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


uint8_t Num=0, RockerNum, RockerNum_X, RockerNum_Y, KeyNum, SwitchNum, BeepNum=14;
uint8_t BuzzerChose, FeedChose, FeedSwitch, Mode=1, Select=2, LastSelect, State=1, State1;
uint8_t FeedTime1, FeedTime2, FeedTime3;
uint8_t Info1, Info2, Info3, Info4, State;
uint16_t Voltage;
uint32_t FeedTime1Temp, FeedTime2Temp, FeedTime3Temp, FeedTemp;
int16_t EncoderNum, Speed;

uint16_t StoreArray[STORENUM] = {0};

void Init(void);
void Voltage_Get(void);
void StoreData(void);
void ReadData(void);
void Transmit(void);
void GetData(void);
void StateGet(void);
void State2(void);
void Show(void);
void CursorControl(uint8_t State);
void OriginalMode(void);
void FeedMode(void);
void BuzzerMode(void);
void OtherMode(void);
void TurnMode(uint8_t TurnMode);
void Normal_IRQHandler(void);

int main(void)
{
	Init();
	ReadData();
	while (1)
	{
		GetData();
		StateGet();
		Transmit();
		if (State == SETTINGSTATE)
		{
			OLED_ShowString(1, 5, "SETTING");
			switch(Mode)
			{
				case 1:
					OriginalMode();break;
				case 2:
					FeedMode();break;
				case 3:
					BuzzerMode();break;
				case 4:
					OtherMode();break;
			}
			if (KeyNum==KEY_LEFT && Mode!=1)		//����
				TurnMode(1);
			CursorControl(SETTINGSTATE);
		}
		else if (State == CONTROLSTATE)
		{
			State2();
		}
		else if (State == DEBUGSTATE)
			Show();			
	}
}

void Voltage_Get(void)
{
	Voltage = (float)ORINGINAL_VOLTAGE/4096*3.3*10/2*5;
	OLED_ShowNum(1, 13, Voltage/10, 1);
	OLED_ShowChar(1, 14, '.');
	OLED_ShowNum(1, 15, Voltage%10, 1);
	OLED_ShowChar(1, 16, 'V');	
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
  * @brief  �����ơ�
  * @param  ?
  * @retval ?
  */
void CursorControl(uint8_t State)
{
	if (State == SETTINGSTATE)
	{
		//����ƶ�
		if (RockerNum_Y==UP_4 || KeyNum==KEY_UP)	//ѡ��λ��
				Select --;
		else if (RockerNum_Y==DOWN_4 || KeyNum==KEY_DOWN)	//ѡ��λ��
				Select ++;
	}
	else if (State == CONTROLSTATE)
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

void StateGet(void)
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
			State = SETTINGSTATE;		
		}
		else if ((SwitchNum&0x08) != 0)	//����ť�ӿ������м䲦
		{
			OLED_ShowString(Select, 15, "<-");
			State = CONTROLSTATE;
		}
		else 							//����ť�ӿ������ϲ�
			State = DEBUGSTATE;
	}
}

void State2(void)
{
	OLED_ShowString(1, 2, "CONTROL");
	OLED_ShowString(2, 1, "Speed:");
	OLED_ShowNum(2, 7, Speed, 3);
	OLED_ShowString(2, 10, "cm/s");
	OLED_ShowString(3, 1, "Feed:");

	OLED_ShowString(4, 1, "ON:");
	if (FeedTime1Temp != 0)
		OLED_ShowChar(4, 4, '1');
	else
		OLED_ShowChar(4, 4, ' ');
	if (FeedTime2Temp != 0)
		OLED_ShowChar(4, 6, '2');
	else
		OLED_ShowChar(4, 6, ' ');
	if (FeedTime2Temp != 0)
		OLED_ShowChar(4, 8, '3');
	else
		OLED_ShowChar(4, 8, ' ');
	
	if (FeedSwitch == SET)
	{
		OLED_ShowString(3, 7, "ON ");
		if (FeedTime1Temp > FeedTime2Temp)
			FeedTemp = FeedTime1Temp;
		else 
			FeedTemp = FeedTime2Temp;
		if (FeedTime3Temp > FeedTemp)
			FeedTemp = FeedTime3Temp;
		OLED_ShowNum(4, 10, FeedTemp/1000, 2);
		OLED_ShowChar(4, 12, 's');
	}
	else
	{
		OLED_ShowString(3, 7, "OFF");
		OLED_ShowString(4, 10, "   ");			
	}
	Voltage_Get();
	if (Speed < 0)
		Speed = 0;
	Speed += Encoder_Get();	
	NRF24L01_SetBuf(NORMAL_TRANSMIT, FeedSwitch + 1);	//����׼�����͵�ι�Ͽ���		
	switch(Select)
	{
		case 2:
			if (KeyNum==KEY_LEFT && Speed>0)
				Speed --;
			else if (KeyNum==KEY_RIGHT && Speed<1000)
				Speed ++;
			break;
		case 3:
			if (KeyNum==KEY_LEFT || KeyNum==KEY_RIGHT)
			{
				FeedSwitch =! FeedSwitch;
				if (FeedSwitch == SET)
				{
					FeedTime1Temp = FeedTime1 * 1000;
					FeedTime2Temp = FeedTime2 * 1000;
					FeedTime3Temp = FeedTime3 * 1000;
				}
			}
			break;
	}
	if (KeyNum == KEY_PIN2_NUM)
		NRF24L01_SetBuf(NORMAL_TRANSMIT, AUTO_FEED);
	CursorControl(CONTROLSTATE);
}
void GetData(void)
{
	KeyNum = Key_GetNum();
	if (KeyNum != 0 && BuzzerChose==SET)
		Buzzer_SetBeep(BeepNum, 100);
	EncoderNum += Encoder_Get();
	if (State == SETTINGSTATE)
	{
		//ң�����ԡ��������ҡ����ж�
		RockerNum = Rocker_GetNum();
		RockerNum_Y = RockerNum&0x0F;
		RockerNum_X = (RockerNum&0xF0)>>4;
	}
	else if (State == DEBUGSTATE)
	{
		RockerNum = Rocker_GetNum2Loop();	//ԭʼ����
		RockerNum_Y = RockerNum&0x0F;
		RockerNum_X = (RockerNum&0xF0)>>4;
		OLED_ShowNum(1, 3, RockerNum_X, 2);
		OLED_ShowNum(1, 8, RockerNum_Y, 2);	
	}
}

void OriginalMode(void)
{
	OLED_ShowString(FeedLine, 1, "1.Feed:");
	OLED_ShowString(BuzzerLine, 1, "2.Buzzer");
	OLED_ShowString(4, 1, "3.Other");
	if (FeedChose == RESET)
		OLED_ShowString(FeedLine, 9, "All  ");
	else
		OLED_ShowString(FeedLine, 9, "Every");
	if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_RIGHT)
		TurnMode(Select);
	switch(Select)
	{
		case 2:
			if (KeyNum==KEY_PIN2_NUM || KeyNum==KEY_PIN3_NUM)
				FeedChose =! FeedChose;
			break;
	}
}

void BuzzerMode(void)
{
	BeepNum %= Buzzer_NUM+1;
	OLED_ShowNum(3, 12, BeepNum, 2);
	if (BuzzerChose == SET)
		OLED_ShowString(2, 10, "ON ");
	else
		OLED_ShowString(2, 10, "OFF");
	switch(Select)
	{
		case 2:
			if (KeyNum==KEY_PIN2_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					BuzzerChose = !BuzzerChose;	
			break;
		case 3:
			//�޸İ����������ء�������ʾ��������		
			if (BeepNum>0 && KeyNum==KEY_PIN2_NUM)
				BeepNum --;
			//�޸İ����������ء�������ʾ�����ӣ�
			if (BeepNum<Buzzer_NUM && KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
				BeepNum ++;
			BeepNum += Encoder_Get();
			break;
		case 4:
			if (KeyNum==KEY_RIGHT || KeyNum==KEY_PIN1_NUM)
				TurnMode(1);
			break;
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
	OLED_ShowNum(2, 7, FeedTime1, 3);
	OLED_ShowNum(3, 7, FeedTime2, 3);
	OLED_ShowNum(4, 7, FeedTime3, 3);
	FeedTime1 %= 256;
	if (FeedChose == RESET)
	{
		if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
			FeedTime1 ++;
		else if (KeyNum==KEY_PIN2_NUM && FeedTime1>0)
			FeedTime1 --;

		FeedTime1 += Encoder_Get();
		FeedTime2 = FeedTime1;
		FeedTime3 = FeedTime1;
	}
	else
	{
		switch(Select)
		{
			case 2:		//ι��1ʱ��			
				if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					FeedTime1 ++;
				else if (KeyNum == KEY_PIN2_NUM)
					FeedTime1 --;
				FeedTime1 += Encoder_Get();
				break;
			case 3:		//ι��2ʱ��				
				if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					FeedTime2 ++;
				else if (KeyNum == KEY_PIN2_NUM)
					FeedTime2 --;
				FeedTime2 += Encoder_Get();
				break;
			case 4:		//ι��3ʱ��				
				if (KeyNum==KEY_PIN1_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_RIGHT)
					FeedTime3 ++;
				else if (KeyNum == KEY_PIN2_NUM)
					FeedTime3 --;
				FeedTime3 += Encoder_Get();
				break;
		}	
	}	
}

void OtherMode(void)
{
	switch(Select)
	{
		case 4:
			if (KeyNum==KEY_RIGHT || KeyNum==KEY_PIN1_NUM)
				TurnMode(1);
			break;
	}		
}

void TurnMode(uint8_t TurnMode)
{
	Mode = TurnMode;
	OLED_Clear();
	OLED_ShowString(Select, 15, "<-");
	CloseBuzzer();		//��ֹFlashд��ʱ���������쳣
	StoreData();
	switch(TurnMode)
	{
		case 1:
			break;
		case 3:
			OLED_ShowString(2, 1, "1.Buzzer:");
			OLED_ShowString(3, 1, "2.KeyMusic:");
			OLED_ShowString(4, 1, "3.Back");
			break;
		case 4:
			OLED_ShowString(2, 1, "1.");
			OLED_ShowString(3, 1, "2.");
			OLED_ShowString(4, 1, "3.Back");			
	}
}

void StoreData(void)
{
	StoreArray[BuzzerChoseStore] = BuzzerChose;
	StoreArray[BeepNumStore] = BeepNum;
	StoreArray[FeedTime1Store] = FeedTime1;
	StoreArray[FeedTime2Store] = FeedTime2;
	StoreArray[FeedTime3Store] = FeedTime3;
	StoreArray[FeedChoseStore] = FeedChose;
	Flash_WriteArray_HalfWord(0x00, StoreArray, STORENUM);
}

void ReadData(void)
{
	Flash_ReadArray_HalfWord(0x00, StoreArray, STORENUM);
	BuzzerChose = StoreArray[BuzzerChoseStore];
	BeepNum = StoreArray[BeepNumStore];
	FeedChose = StoreArray[FeedChoseStore];
	FeedTime1 = StoreArray[FeedTime1Store];
	FeedTime2 = StoreArray[FeedTime2Store];
	FeedTime3 = StoreArray[FeedTime3Store];
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
	NRF24L01_SetBuf(FEEDTIME1_TRANSMIT, FeedTime1Temp/1000);
	NRF24L01_SetBuf(FEEDTIME2_TRANSMIT, FeedTime2Temp/1000);
	NRF24L01_SetBuf(FEEDTIME3_TRANSMIT, FeedTime3Temp/1000);
	NRF24L01_TransmitBuf();
	NRF24L01_SetBuf(NORMAL_TRANSMIT, 0);
}

void Show(void)
{
	OLED_ShowString(1, 1, "X:");
	OLED_ShowString(1, 6, "Y:");
	OLED_ShowString(1, 11, "K:");
	OLED_ShowString(2, 1, "Encoder:");
	OLED_ShowString(3, 1, "N:");
	OLED_ShowString(3, 6, "S:");
	OLED_ShowString(3, 11, "H:");
	State1 = NRF24L01_ReadByte(STATUS);
	Info1 = NRF24L01_ReadByte(EN_AA);
	Info2 = NRF24L01_ReadByte(EN_RXADDR);
	Info3 = NRF24L01_ReadByte(RX_ADDR_P0);
	Info4 = NRF24L01_ReadByte(FIFO_STATUS);
	OLED_ShowHexNum(4, 1, State1, 2);
	OLED_ShowHexNum(4, 4, Info1, 2);
	OLED_ShowHexNum(4, 7, Info2, 2);
	OLED_ShowHexNum(4, 10, Info3, 2);
	OLED_ShowHexNum(4, 13, Info4, 2);
	if (KeyNum != 0)
		OLED_ShowNum(1, 13, KeyNum, 2);
	if (SwitchNum != 0)
		OLED_ShowNum(3, 13, SwitchNum, 2);
	OLED_ShowSignedNum(2, 9, EncoderNum, 4);
	OLED_ShowNum(3, 3, ++Num, 2);
	OLED_ShowHexNum(3, 8, RockerNum, 2);

}

void Normal_IRQHandler(void)	//1ms
{
	static uint16_t T3_Count[1];
	T3_Count[0]++;
	if ( T3_Count[0] >= 20)
	{
		T3_Count[0] = 0;
		Key_Loop();
		Switch_Loop();
		if (State==SETTINGSTATE)
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
	if (FeedTime1Temp>0 && FeedSwitch==SET)
	{
		FeedTime1Temp --;
	}
	if (FeedTime2Temp>0 && FeedSwitch==SET)
	{
		FeedTime2Temp --;
	}
	if (FeedTime3Temp>0 && FeedSwitch==SET)
	{
		FeedTime3Temp --;
	}
	if (FeedTime1Temp==0 && FeedTime2Temp==0 && FeedTime3Temp==0)
		FeedSwitch = RESET;
}
