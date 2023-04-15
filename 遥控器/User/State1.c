#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "main.h"
#include "Key.h"
#include "Buzzer.h"
#include "Encoder.h"

uint8_t BeepNum, BeepChose;

void Set_BeepNum(uint8_t Num)
{
	BeepNum = Num;
}

uint8_t Get_BeepNum(void)
{
	return BeepNum;
}

void Set_BeepChose(uint8_t Num)
{
	BeepChose = Num;
}

uint8_t Get_BeepChose(void)
{
	return BeepChose;
}

/**
  * @brief  ������ģʽ�µĳ�ʼģʽ��
  * @param  State��״̬��
			Select��������ڵ�������
			KeyNum����ǰ�����µİ�����
  * @retval StateChangeFlag: ״̬�ı��־λ����¼"State"�����Ƿ�ı䡣���ǣ���Ϊ1�����ǣ���Ϊ0.
  */
uint8_t Setting_OriginMode(uint8_t *State, uint8_t Select, uint8_t KeyNum)
{
	OLED_ShowString(1, 2, "SETTING");
	OLED_ShowString(FeedLine, 1, "1.Feed:");
	OLED_ShowString(BuzzerLine, 1, "2.Buzzer");
	OLED_ShowString(4, 1, "3.Other");
	OLED_ShowString(FeedLine, 9, "All  ");
	switch(Select)
	{
		;//��ʱû�õ�
	}
	//����ȷ������ҡ�˰�������������һ��
	if (KeyNum==KEY_ENTER || KeyNum==KEY_ROCKER)
	{
		*State = STATE_SETTING + Select -1;
		return 1;
	}
	return 0;
}

/**
  * @brief  ������ģʽ�µ�������ģʽ��
  * @param  State��״̬��
			Select��������ڵ�������
			KeyNum����ǰ�����µİ�����
  * @retval StateChangeFlag: ״̬�ı��־λ����¼"State"�����Ƿ�ı䡣���ǣ���Ϊ1�����ǣ���Ϊ0.
  */
uint8_t Setting_BuzzerMode(uint8_t *State, uint8_t Select, uint8_t KeyNum)
{
	BeepNum %= Buzzer_NUM+1;	//�����
	OLED_ShowNum(3, 12, BeepNum, 2);
	if (BeepChose == SET)
		OLED_ShowString(2, 10, "ON ");
	else
		OLED_ShowString(2, 10, "OFF");
	switch(Select)
	{
		case 2:
			if (KeyNum==KEY_PIN2_NUM || KeyNum==KEY_PIN3_NUM || KeyNum==KEY_ENTER)
					BeepChose = !BeepChose;	
			break;
		case 3:
			//�޸İ����������ء�������ʾ��������		
			if (BeepNum>0 && KeyNum==KEY_PIN2_NUM)
				BeepNum --;
			//�޸İ����������ء�������ʾ�����ӣ�
			if (BeepNum<Buzzer_NUM && (KeyNum==KEY_PIN3_NUM || KeyNum==KEY_ENTER))
				BeepNum ++;
			BeepNum += Encoder_Get();
			break;
		case 4:
			if (KeyNum==KEY_ENTER || KeyNum==KEY_ROCKER)
			{
				*State = STATE_SETTING_ORIGIN;
				return 1;	
			}break;
	}	
	return 0;
}

/**
  * @brief  ������ģʽ�µ�����ģʽ��
  * @param  State��״̬��
			Select��������ڵ�������
			KeyNum����ǰ�����µİ�����
  * @retval StateChangeFlag: ״̬�ı��־λ����¼"State"�����Ƿ�ı䡣���ǣ���Ϊ1�����ǣ���Ϊ0.
  */
uint8_t Setting_OtherMode(uint8_t *State, uint8_t Select, uint8_t KeyNum)
{
	switch(Select)
	{
		case 4:
			//����ȷ������ҡ�˰�������������һ��
			if (KeyNum==KEY_ENTER || KeyNum==KEY_ROCKER)
			{
				*State = STATE_SETTING_ORIGIN;
				return 1;
			}
			break;
	}
	return 0;
}
