#include "stm32f10x.h"                  // Device header
#include "Internal_Flash.h"
#include "OLED.h"

/**
  * @brief  ��һ��32λ����д�뵽�ڲ�flash�У�������Ƿ�д��ɹ���
  * @param  2��������
  *     @arg Address: д��ĵ�ַ.    
  *     @arg Data: д������� .     
  * @retval Status ��ʾ�Ƿ�д��ɹ�.
  *     @arg PASSED: д��ɹ�.      
  *     @arg FAILED: д��ʧ��.      
  */
uint8_t Flash_Write(uint32_t Address, uint32_t Data)
{
	uint32_t EraseCounter = 0x00; 		//��¼Ҫ��������ҳ
	uint32_t WriteAddress = 0x00;		//��¼д��ĵ�ַ
	uint32_t NbrOfPage = 0x00;			//��¼д�����ҳ
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //��¼ÿ�β����Ľ��	
	uint8_t Status = PASSED;//��¼�������Խ��
	

	/* ���� */
	FLASH_Unlock();

	/* ����Ҫ��������ҳ */
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;

	/* ������б�־λ */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	/* ��ҳ����*/
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)	
		FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));

	/* ���ڲ�FLASHд������ */
	WriteAddress = WRITE_START_ADDR + Address;
	FLASHStatus = FLASH_ProgramWord(WriteAddress, Data);

	FLASH_Lock();

	/* ���д��������Ƿ���ȷ */
	WriteAddress = WRITE_START_ADDR + Address;
	if((*(__IO uint32_t*) WriteAddress) != Data)
	{
	  Status = FAILED;
	}
	return Status;
}

/**
  * @brief  ��һ��16λ����д�뵽�ڲ�flash�У�������Ƿ�д��ɹ���
  * @param  3��������
  *     @arg Address: д��ĵ�ַ.    
  *     @arg Data: д������� .     
  *     @arg Data: д����������� .     
  * @retval Status ��ʾ�Ƿ�д��ɹ�.
  *     @arg PASSED: д��ɹ�.      
  *     @arg FAILED: д��ʧ��.      
  */
void Flash_WriteArray_HalfWord(uint32_t Address, uint16_t *Array, uint16_t WriteNum)
{
	uint16_t i=0;
	uint32_t EraseCounter = 0x00; 		//��¼Ҫ��������ҳ
	uint32_t WriteAddress = 0x00;		//��¼д��ĵ�ַ
	uint32_t NbrOfPage = 0x00;			//��¼д�����ҳ
	uint32_t WriteEndAddress = WRITE_START_ADDR + Address + WriteNum*2;
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //��¼ÿ�β����Ľ��	
	
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
	uint32_t EraseCounter = 0x00; 		//��¼Ҫ��������ҳ
	uint32_t WriteAddress = 0x00;		//��¼д��ĵ�ַ
	uint32_t NbrOfPage = 0x00;			//��¼д�����ҳ
	uint32_t WriteEndAddress = WRITE_START_ADDR + Address + WriteNum*4;
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //��¼ÿ�β����Ľ��	
	uint8_t Status = PASSED;//��¼�������Խ��
	
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
	/* ���д��������Ƿ���ȷ */
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
  * @brief  ���ڲ�Flash�ж�ȡһ���ֽ����ݡ�
* @param  Address: ��ȡ�ĵ�ַ��
  * @retval �ڲ�Flash��ȡ�����ֽڡ�
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
  * @brief  ���ڲ�Flash�ж�ȡ����ֽڣ����洢�������С�
* @param  Address: ��ȡ�ĵ�ַ��
  *     @arg Address: �����ĵ�ַ.    
  *     @arg Address: �洢������.    
  *     @arg Address: ��������������.    
  * @retval ��
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
