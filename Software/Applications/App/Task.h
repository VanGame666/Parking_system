#ifndef __TASK_H__
#define __TASK_H__

#define EVENT_FLAG0 (1 << 0)
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)

typedef struct
{
  char *name;
  void (*entry)(void *parameter);
  void *parameter;
  rt_uint32_t stack_size;
  rt_uint8_t  priority;
  rt_uint32_t tick;
}TaskStruct;

typedef enum
{ 
  Crossing_exit = 0,
  Crossing_entr,
}message_em;

typedef enum //定义消息枚举类型
{
	MSG_NULL = 0,
	/******************************/
	//添加用户消息常量，例如:MSG_XXX,
	MSG_exit,
	MSG_entr,
	
	
	/******************************/
	MSG_NUM
	
}MSG_TYPE;

void TaskInit(void);

#endif
