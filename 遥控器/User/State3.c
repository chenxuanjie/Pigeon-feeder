#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "main.h"
#include "NRF24L01.h"

uint8_t Num=0;

uint8_t Show(uint8_t KeyNum, uint8_t SwitchNum, uint8_t EncoderNum, uint8_t RockerNum)
{
	OLED_ShowString(1, 1, "X:");
	OLED_ShowString(1, 6, "Y:");
	OLED_ShowString(1, 11, "K:");
	OLED_ShowString(2, 1, "Encoder:");
	OLED_ShowString(3, 1, "N:");
	OLED_ShowString(3, 6, "S:");
	OLED_ShowString(3, 11, "H:");

	OLED_ShowHexNum(4, 1, NRF24L01_ReadByte(STATUS), 2);
	OLED_ShowHexNum(4, 4, NRF24L01_ReadByte(EN_AA), 2);
	OLED_ShowHexNum(4, 7, NRF24L01_ReadByte(EN_RXADDR), 2);
	OLED_ShowHexNum(4, 10, NRF24L01_ReadByte(RX_ADDR_P0), 2);
	OLED_ShowHexNum(4, 13, NRF24L01_ReadByte(FIFO_STATUS), 2);
	if (KeyNum != 0)
		OLED_ShowNum(1, 13, KeyNum, 2);
	if (SwitchNum != 0)
		OLED_ShowNum(3, 13, SwitchNum, 2);
	OLED_ShowSignedNum(2, 9, EncoderNum, 4);
	OLED_ShowNum(3, 3, ++Num, 2);
	OLED_ShowHexNum(3, 8, RockerNum, 2);
	return 0;
}
