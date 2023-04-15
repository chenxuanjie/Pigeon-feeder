#ifndef __STATE1_H__
#define __STATE1_H__

void Set_BeepNum(uint8_t Num);
uint8_t Get_BeepNum(void);
void Set_BeepChose(uint8_t Num);
uint8_t Get_BeepChose(void);
uint8_t Setting_OriginMode(uint8_t *State, uint8_t Select, uint8_t KeyNum);
uint8_t Setting_BuzzerMode(uint8_t *State, uint8_t Select, uint8_t KeyNum);
uint8_t Setting_OtherMode(uint8_t *State, uint8_t Select, uint8_t KeyNum);

#endif


