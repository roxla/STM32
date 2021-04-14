#include <flash.h>

int32_t flash_test(void)
{
	uint32_t i;
	// ����Flash
	FLASH_Unlock();
	// ������б�־λ
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	// ��������4��ѡ���ѹ�ȼ�3��2.7V~3.6V����ÿ��ʱ�䵥λ����4���ֽڣ�ֱ������������ϡ�
	if(FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
	{
		printf("FLASH_EraseSector failed!\r\n");
		return -1;
	}
	// д�����ݵ�����4
	for(i = 0; i < 64; i++)
	{
		if(FLASH_ProgramWord(0x08010000+i*4, i) != FLASH_COMPLETE)
		{
			printf("FLASH_ProgramWord failed!\r\n");
			return -1;
		}
	}
	// ����Flash�������������ݲ������ġ�
	FLASH_Lock();
	
	return 0;
}
