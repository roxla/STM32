#include <flash.h>

int32_t flash_test(void)
{
	uint32_t i;
	// 解锁Flash
	FLASH_Unlock();
	// 清除所有标志位
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	// 擦除扇区4，选择电压等级3（2.7V~3.6V），每个时间单位擦除4个字节，直到扇区擦除完毕。
	if(FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
	{
		printf("FLASH_EraseSector failed!\r\n");
		return -1;
	}
	// 写入数据到扇区4
	for(i = 0; i < 64; i++)
	{
		if(FLASH_ProgramWord(0x08010000+i*4, i) != FLASH_COMPLETE)
		{
			printf("FLASH_ProgramWord failed!\r\n");
			return -1;
		}
	}
	// 锁定Flash，保护数据内容不被更改。
	FLASH_Lock();
	
	return 0;
}
