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
  * @brief  在设置模式下的初始模式。
  * @param  State：状态。
			Select：光标所在的行数。
			KeyNum：当前所按下的按键。
  * @retval StateChangeFlag: 状态改变标志位。记录"State"变量是否改变。若是，则为1，不是，则为0.
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
		;//暂时没用到
	}
	//按下确定键、摇杆按键，返回至上一级
	if (KeyNum==KEY_ENTER || KeyNum==KEY_ROCKER)
	{
		*State = STATE_SETTING + Select -1;
		return 1;
	}
	return 0;
}

/**
  * @brief  在设置模式下的扬声器模式。
  * @param  State：状态。
			Select：光标所在的行数。
			KeyNum：当前所按下的按键。
  * @retval StateChangeFlag: 状态改变标志位。记录"State"变量是否改变。若是，则为1，不是，则为0.
  */
uint8_t Setting_BuzzerMode(uint8_t *State, uint8_t Select, uint8_t KeyNum)
{
	BeepNum %= Buzzer_NUM+1;	//超额保护
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
			//修改按键声音开关、按键提示音（减）		
			if (BeepNum>0 && KeyNum==KEY_PIN2_NUM)
				BeepNum --;
			//修改按键声音开关、按键提示音（加）
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
  * @brief  在设置模式下的其他模式。
  * @param  State：状态。
			Select：光标所在的行数。
			KeyNum：当前所按下的按键。
  * @retval StateChangeFlag: 状态改变标志位。记录"State"变量是否改变。若是，则为1，不是，则为0.
  */
uint8_t Setting_OtherMode(uint8_t *State, uint8_t Select, uint8_t KeyNum)
{
	switch(Select)
	{
		case 4:
			//按下确定键、摇杆按键，返回至上一级
			if (KeyNum==KEY_ENTER || KeyNum==KEY_ROCKER)
			{
				*State = STATE_SETTING_ORIGIN;
				return 1;
			}
			break;
	}
	return 0;
}
