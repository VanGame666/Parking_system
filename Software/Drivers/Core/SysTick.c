#include "config.h"
#include "SysTick.h"





void SysTickInit(void)
{

	SysTick_Config(SystemCoreClock/RT_TICK_PER_SECOND);//
	
}


void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    rt_uint32_t current_delay;

    /* 获得延时经过的tick数 */
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));

    /* 获得当前时间 */
    delta = SysTick->VAL;

    /* 循环获得当前时间，直到达到指定的时间后退出循环 */
    do
    {
			if ( delta > SysTick->VAL )
				current_delay = delta - SysTick->VAL;
			else
        /* 延时跨越了一次OS tick的边界时的处理 */
				current_delay = SysTick->LOAD + delta - SysTick->VAL;
    } while( current_delay < us );
}
