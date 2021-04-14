#include "flash.h"

void flash_set_pwd(u32 pwd)
{
	OS_ERR err;
	OS_FLAGS flag;
	// �ȴ��¼���־λ
	flag = OSFlagPend(&g_flag_grp, FLAG_CHANGE_PWD, 0, OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING, NULL, &err);
	if(flag & FLAG_CHANGE_PWD)
	{
		printf("change_pwd\r\n");
		/* ����Flash */
		FLASH_Unlock();

		// ������б�־λ
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
						FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

		// ��������4��ѡ���ѹ�ȼ�3��2.7V~3.6V����ÿ��ʱ�䵥λ����4���ֽڣ�ֱ������������ϡ�
		if(FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			printf("FLASH_EraseSector failed!\r\n");
			while (1);
		}

		// д�����ݵ�����4������ǰ��32λ���ݳ���д�룩
			// ��ѡ���ֽڡ����֡��֡�˫��
		if(FLASH_ProgramWord(0x08010000, pwd) != FLASH_COMPLETE)
		{
			printf("FLASH_ProgramWord failed!\r\n");
			while (1);
		}
		
		// ����Flash�������������ݲ������ġ�
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
