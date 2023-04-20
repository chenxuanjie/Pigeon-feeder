#ifndef __FEED_H__
#define __FEED_H__

#define FEEDTIME_PERBIRD	1000


void Get_Distance(uint8_t *Hcsr04_StartFlag);
void Feeding_Init(void);
void Get_BirdNum(uint32_t *Timeout);
void Get_FeedTime(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3);
void StartFeed(uint32_t feedTime1_ms, uint32_t feedTime2_ms, uint32_t feedTime3_ms);
void MonitorFeed(uint8_t *Hcsr04_StartFlag);
void Feeding_CloseFeeder(uint32_t* feedTime1_ms, uint32_t* feedTime2_ms, uint32_t* feedTime3_ms);

#endif
