#ifndef __SPI1_H__
#define __SPI1_H__

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define ERROR(fmt,arg...)          printf("<<-ERROR->> "fmt"\n",##arg)

void SPI1_Init(void);
uint8_t SPI_SendByte(uint8_t Byte);

#endif
