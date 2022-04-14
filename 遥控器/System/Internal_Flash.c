#include "stm32f10x.h"                  // Device header
#include "Internal_Flash.h"
#include "OLED.h"

/**
  * @brief  将一个32位数据写入到内部flash中，并检查是否写入成功。
  * @param  2个参数。
  *     @arg Address: 写入的地址.    
  *     @arg Data: 写入的数据 .     
  * @retval Status 表示是否写入成功.
  *     @arg PASSED: 写入成功.      
  *     @arg FAILED: 写入失败.      
  */
uint8_t Flash_Write(uint32_t Address, uint32_t Data)
{
	uint32_t EraseCounter = 0x00; 		//记录要擦除多少页
	uint32_t WriteAddress = 0x00;		//记录写入的地址
	uint32_t NbrOfPage = 0x00;			//记录写入多少页
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //记录每次擦除的结果	
	uint8_t Status = PASSED;//记录整个测试结果
	

	/* 解锁 */
	FLASH_Unlock();

	/* 计算要擦除多少页 */
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;

	/* 清空所有标志位 */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	/* 按页擦除*/
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)	
		FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));

	/* 向内部FLASH写入数据 */
	WriteAddress = WRITE_START_ADDR + Address;
	FLASHStatus = FLASH_ProgramWord(WriteAddress, Data);

	FLASH_Lock();

	/* 检查写入的数据是否正确 */
	WriteAddress = WRITE_START_ADDR + Address;
	if((*(__IO uint32_t*) WriteAddress) != Data)
	{
	  Status = FAILED;
	}
	return Status;
}

/**
  * @brief  将一个16位数组写入到内部flash中，并检查是否写入成功。
  * @param  3个参数。
  *     @arg Address: 写入的地址.    
  *     @arg Data: 写入的数组 .     
  *     @arg Data: 写入的数据数量 .     
  * @retval Status 表示是否写入成功.
  *     @arg PASSED: 写入成功.      
  *     @arg FAILED: 写入失败.      
  */
void Flash_WriteArray_HalfWord(uint32_t Address, uint16_t *Array, uint16_t WriteNum)
{
	uint16_t i=0;
	uint32_t EraseCounter = 0x00; 		//记录要擦除多少页
	uint32_t WriteAddress = 0x00;		//记录写入的地址
	uint32_t NbrOfPage = 0x00;			//记录写入多少页
	uint32_t WriteEndAddress = WRITE_START_ADDR + Address + WriteNum*2;
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //记录每次擦除的结果	
	
	FLASH_Unlock();
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}
	
	WriteAddress = WRITE_START_ADDR + Address;
	while((WriteAddress < WriteEndAddress) && (FLASHStatus == FLASH_COMPLETE))
	{
		FLASHStatus = FLASH_ProgramHalfWord(WriteAddress, Array[i++]);
		WriteAddress += 2;
	}
	FLASH_Lock();

}

uint8_t Flash_WriteArray_Word(uint32_t Address, uint32_t *Array, uint16_t WriteNum)
{
	uint16_t i=0;
	uint32_t EraseCounter = 0x00; 		//记录要擦除多少页
	uint32_t WriteAddress = 0x00;		//记录写入的地址
	uint32_t NbrOfPage = 0x00;			//记录写入多少页
	uint32_t WriteEndAddress = WRITE_START_ADDR + Address + WriteNum*4;
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //记录每次擦除的结果	
	uint8_t Status = PASSED;//记录整个测试结果
	
	FLASH_Unlock();
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}
	
	WriteAddress = WRITE_START_ADDR + Address;
	while((WriteAddress < WriteEndAddress) && (FLASHStatus == FLASH_COMPLETE))
	{
		FLASHStatus = FLASH_ProgramWord(WriteAddress, Array[i++]);
		WriteAddress += 4;
		OLED_ShowHexNum(3, 1, WriteAddress, 8);
		OLED_ShowHexNum(4, 1, WriteAddress + WriteNum*4, 8);
	}
	FLASH_Lock();
	/* 检查写入的数据是否正确 */
	WriteAddress = WRITE_START_ADDR + Address;
	i = 0;
	while((WriteAddress < WriteEndAddress) && (Status != FAILED))
	{
		if((*(__IO uint32_t*) WriteAddress) != Array[i++])
		{
		  Status = FAILED;
		}
		WriteAddress += 4;
	}
	return Status;
}

/**
  * @brief  从内部Flash中读取一个字节数据。
* @param  Address: 读取的地址。
  * @retval 内部Flash读取到的字节。
  */
uint8_t Flash_ReadByte(uint32_t Address)
{
	uint32_t ReadAddress = WRITE_START_ADDR + Address;
	
	return (*(__IO uint8_t*) ReadAddress);
}

uint16_t Flash_ReadHalfWord(uint32_t Address)
{
	uint32_t ReadAddress = WRITE_START_ADDR + Address;
	
	return (*(__IO uint16_t*) ReadAddress);
}

uint32_t Flash_ReadWord(uint32_t Address)
{
	uint32_t ReadAddress = WRITE_START_ADDR + Address;
	
	return (*(__IO uint32_t*) ReadAddress);
}

/**
  * @brief  从内部Flash中读取多个字节，并存储到数组中。
* @param  Address: 读取的地址。
  *     @arg Address: 读出的地址.    
  *     @arg Address: 存储的数组.    
  *     @arg Address: 读出的数据数量.    
  * @retval 无
  */
void Flash_ReadArray_Byte(uint32_t Address, uint8_t *Array, uint16_t WriteNum)
{
	uint16_t i;
	for (i=0; i<WriteNum; i++)
	{
		Array[i] = Flash_ReadByte(Address);
		Address += 1;
	}
}

void Flash_ReadArray_HalfWord(uint32_t Address, uint16_t *Array, uint16_t WriteNum)
{
	uint16_t i;
	for (i=0; i<WriteNum; i++)
	{
		Array[i] = Flash_ReadHalfWord(Address);
		Address += 2;
	}
}

void Flash_ReadArray_Word(uint32_t Address, uint32_t *Array, uint16_t WriteNum)
{
	uint16_t i;
	for (i=0; i<WriteNum; i++)
	{
		Array[i] = Flash_ReadWord(Address);
		Address += 4;
	}
}
