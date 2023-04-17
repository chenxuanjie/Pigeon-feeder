#ifndef __STATE2_H__
#define __STATE2_H__

void Feeding_AutoFeedSet(uint8_t *Flag, uint8_t BlinkTimes);
void Feeding_AutoFeedShow(uint8_t Flag);
void Feeding_DebugFeedSet(machine* feeder1, machine* feeder2, machine* feeder3);

#endif
