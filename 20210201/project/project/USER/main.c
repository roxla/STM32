#include <myhead.h>
#include "bmp.h"

int main(void)
{
	u8 status, card_size ,i;
	//MFRC522数据区
	u8  mfrc552pidbuf[18];
	u8  card_pydebuf[2];
	u8  card_numberbuf[5];
	
	systick_init();
	beep_init();
	uart1_init();
	uart3_init();
	
	my_delay_s(2);
	
	while(1){
		MF522_Init();
		//请求卡
		status = MFRC522_Request(0x52, card_pydebuf);	
		if(status!=MI_OK){
			printf("MFRC522_Request failed!\r\n");
			my_delay_s(2);
			continue;
		}
		
		//防碰撞
		status = MFRC522_Anticoll(card_numberbuf);
		if(status!=MI_OK){
			printf("MFRC522_Anticoll failed!\r\n");
			my_delay_s(2);
			continue;
		}
		
		my_delay_s(2);
	}
}
