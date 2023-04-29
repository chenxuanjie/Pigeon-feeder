#ifndef __STATE3_H__
#define __STATE3_H__

#define BASESPEED	Set_Speed/10;
#define SPEED1		Set_Speed/10 + NRF24L01_GetData(ENCODER_TRANSMIT) *6 * BASESPEED 
#define SPEED2		Set_Speed/4  + NRF24L01_GetData(ENCODER_TRANSMIT) *6* BASESPEED 
#define SPEED3		Set_Speed/2  + NRF24L01_GetData(ENCODER_TRANSMIT)*6* BASESPEED 
#define SPEED4		Set_Speed/1  + NRF24L01_GetData(ENCODER_TRANSMIT) *6* BASESPEED 
#define TURNSPEED	Set_Speed/10*4
#define TURNSPEED1	Set_Speed/5
#define TURNSPEED2	Set_Speed/5*2
#define TURNSPEED3	Set_Speed/5*3
#define TURNDOWNLEVEL	3

void Robot_SelfTurn(uint8_t X, uint8_t Y, int16_t Set_Speed);
void Robot_DifferentialTurn(uint8_t X, uint8_t Y, int16_t Set_Speed);

#endif
