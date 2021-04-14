#include <systick.h>

int32_t delay_s(uint32_t ns)
{
	uint32_t t = ns;

	uint32_t temp;
	
	t = t * 1000;

#if 1	
	while(t--)
	{
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
		SysTick->LOAD = 21000-1;
		SysTick->VAL = 0;
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启定时器
		while(1)
		{

			temp=SysTick->CTRL;
			
			//检测count flag
			if(temp & 0x00010000)
				break;
			
			//检测系统定时器是否意外关闭	
			if((temp & 0x1)==0)
				return -1;		
		}
	}
#else
	while(t--)
	{
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
		SysTick->LOAD = 168000-1;
		SysTick->VAL = 0;
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启定时器
		while(1)
		{

			temp=SysTick->CTRL;
			
			//检测count flag
			if(temp & 0x00010000)
				break;
			
			//检测系统定时器是否意外关闭	
			if((temp & 0x1)==0)
				return -1;		
		}
	}
#endif
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
	return 0;
}

int32_t delay_ms(uint32_t nms)
{
	uint32_t t = nms;

	uint32_t temp;
	
	
	#if 1	
	while(t--)
	{
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
		SysTick->LOAD = 21000-1;
		SysTick->VAL = 0;
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启定时器
		while(1)
		{

			temp=SysTick->CTRL;
			
			//检测count flag
			if(temp & 0x00010000)
				break;
			
			//检测系统定时器是否意外关闭	
			if((temp & 0x1)==0)
				return -1;		
		}
	}
#else
	while(t--)
	{
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
		SysTick->LOAD = 168000-1;
		SysTick->VAL = 0;
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启定时器
		while(1)
		{

			temp=SysTick->CTRL;
			
			//检测count flag
			if(temp & 0x00010000)
				break;
			
			//检测系统定时器是否意外关闭	
			if((temp & 0x1)==0)
				return -1;		
		}
	}
#endif	
	
	SysTick->CTRL = 0; 	
	
	return 0;
}

int32_t delay_us(uint32_t nus)
{
	uint32_t temp;

#if 1
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;				
	SysTick->LOAD = (nus*21)-1; 		
	SysTick->VAL = 0; 					
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	while(1)
	{
	
		temp=SysTick->CTRL;
		
		//检测count flag
		if(temp & 0x00010000)
			break;
		
		//检测系统定时器是否意外关闭	
		if((temp & 0x1)==0)
			return -1;		
	}
#else
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = (nus*168)-1; 		
	SysTick->VAL = 0; 					
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启定时器
	
	while(1)
	{
	
		temp=SysTick->CTRL;
		
		//检测count flag
		if(temp & 0x00010000)
			break;
		
		//检测系统定时器是否意外关闭	
		if((temp & 0x1)==0)
			return -1;		
	}
#endif
	
	SysTick->CTRL = 0; 					

	return 0;
}
