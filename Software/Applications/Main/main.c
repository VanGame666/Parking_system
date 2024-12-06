#define  MAIN_CONFIG


#include "config.h"
#include "stm32f10x.h"


int main(void)
{
  TaskInit();
	
	while(1)
	{
	  rt_thread_mdelay(5000);
	}
	
}

