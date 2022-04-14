#ifndef __ROCKER_H__
#define __ROCKER_H__

#define ROCKER_X			ADC_ConvertedValue[0]
#define ROCKER_Y			ADC_ConvertedValue[1]

void Rocker_Init(void);
uint8_t Rocker_GetNum2Loop(void);
uint8_t Rocker_GetNum(void);
void Rocker_Loop(void);

#endif
