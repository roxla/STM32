#include "flash.h"

void flash_set_pwd(u32 pwd)
{
	OS_ERR err;
	OS_FLAGS flag;
	// 等待事件标志位
	flag = OSFlagPend(&g_flag_grp, FLAG_CHANGE_PWD, 0, OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING, NULL, &err);
	if(flag & FLAG_CHANGE_PWD)
	{
		printf("change_pwd\r\n");
		/* 解锁Flash */
		FLASH_Unlock();

		// 清除所有标志位
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
						FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

		// 擦除扇区4，选择电压等级3（2.7V~3.6V），每个时间单位擦除4个字节，直到扇区擦除完毕。
		if(FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			printf("FLASH_EraseSector failed!\r\n");
			while (1);
		}

		// 写入数据到扇区4：（当前以32位数据长度写入）
			// 可选：字节、半字、字、双字
		if(FLASH_ProgramWord(0x08010000, pwd) != FLASH_COMPLETE)
		{
			printf("FLASH_ProgramWord failed!\r\n");
			while (1);
		}
		
		// 锁定Flash，保护数据内容不被更改。
		FLASH_Lock();
	}
}

void flash_get_pwd(u32 *pwd)
{
	volatile uint32_t *p_data = (volatile uint32_t *)FLASH_ADDR_SECTOR_4;
	if(*p_data != 0xFFFFFFFF)
	{
		*pwd = *p_data;
	}
}

uint8_t virtual_pwd(char *pwd, char *o_pwd)
{
	char temp[O_PWD_LEN];
	int i;
	
	while(*(pwd + O_PWD_LEN - 1))
	{
		memset(temp, 0 ,O_PWD_LEN);
		for(i = 0; i < O_PWD_LEN; i++)
		{
			temp[i] = *(pwd + i);
		}
		
		if(strcmp(o_pwd,temp) == 0)
		{
			return 1;
		}
		
		pwd++;
	}
	
	return 0;
}
