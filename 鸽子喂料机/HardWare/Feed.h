#ifndef __FEED_H__
#define __FEED_H__

#define FEEDTIME_PERBIRD	1000

extern uint32_t Timeout;

void Feed_Init(void);
void GetBirdNum(void);
void GetFeedTime(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3);
void StartFeed(uint32_t* feedTime1, uint32_t* feedTime2, uint32_t* feedTime3);

#endif
