#ifndef __FEED_H__
#define __FEED_H__

#define FEEDTIME_PERBIRD	1000

extern uint32_t Timeout;

void Get_Distance(uint8_t *Hcsr04_StartFlag);
void Feeding_Init(void);
void Get_BirdNum(void);
void GetFeedTime(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3);
void StartFeed(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3);
void MonitorFeed(void);

#endif
