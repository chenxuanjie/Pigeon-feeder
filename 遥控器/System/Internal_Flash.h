#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

/* STM32大容量产品每页大小2KByte，中、小容量产品每页大小1KByte */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)	//2048
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)	//1024
#endif

#define WRITE_START_ADDR  ((uint32_t)0x08008000)
#define WRITE_END_ADDR    ((uint32_t)0x0800C000)

typedef enum 
{
	FAILED = 0, 
  PASSED = !FAILED
} TestStatus;

uint8_t Flash_Write(uint32_t Address, uint32_t Data);
void Flash_WriteArray_HalfWord(uint32_t Address, uint16_t *Array, uint16_t WriteNum);
uint8_t Flash_WriteArray_Word(uint32_t Address, uint32_t *Array, uint16_t WriteNum);
uint8_t Flash_ReadByte(uint32_t Address);
uint16_t Flash_ReadHalfWord(uint32_t Address);
uint32_t Flash_ReadWord(uint32_t Address);
void Flash_ReadArray_Byte(uint32_t Address, uint8_t *Array, uint16_t WriteNum);
void Flash_ReadArray_HalfWord(uint32_t Address, uint16_t *Array, uint16_t WriteNum);
void Flash_ReadArray_Word(uint32_t Address, uint32_t *Array, uint16_t WriteNum);


#endif
