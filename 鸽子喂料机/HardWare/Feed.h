#ifndef __FEED_H__
#define __FEED_H__

#include "main.h"
#define FEEDTIME_PERBIRD	1000


void Feeding_ShowTime(machine* machine1, machine* machine2, machine* machine3);
uint16_t Feeding_GetRemoteAutoTimes(void);
void Feeding_SetRemoteAutoTimes(uint16_t Num);
void Get_Distance(uint8_t *Hcsr04_StartFlag);
void Feeding_Init(void);
void Get_BirdNum(uint32_t *Timeout);
void Get_FeedTime(machine* machine1, machine* machine2, machine* machine3, uint8_t Flag);
void StartFeed(uint32_t feedTime1_ms, uint32_t feedTime2_ms, uint32_t feedTime3_ms);
void MonitorFeed(uint8_t *Hcsr04_StartFlag);
void Feeding_CloseFeeder(void);
void Feeding_ResetFeeder(machine* machine1, machine* machine2, machine* machine3);

#endif
