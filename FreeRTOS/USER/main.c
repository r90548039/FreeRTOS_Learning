#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验2-1
 FreeRTOS移植实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define START_TASK_STACK  120
#define START_TASK_PRIO   1
void start_task(void* pvParameters);
TaskHandle_t StartTask_Handle;


#define TASK1_LED_STACK  120
#define TASK1_LED_PRIO   2
void task1_LED(void* pvParameters);
TaskHandle_t Task1LED_Handle;	

#define TASK2_LED_STACK  120
#define TASK2_LED_PRIO   3
TaskHandle_t Task2LED_Handle;	
void task2_LED(void* pvParameters);

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	LED_Init();		        //初始化LED端口
	xTaskCreate(	(TaskFunction_t ) start_task,
								(char *					) "start_task",
								(uint16_t				) START_TASK_STACK,
								(void *					) NULL,
								(UBaseType_t		) START_TASK_PRIO,
								(TaskHandle_t*	) &StartTask_Handle ) ;
														
        
    vTaskStartScheduler();          //开启任务调度
}
 
void start_task(void * pvParameters)//开始任务�
{
		xTaskCreate((TaskFunction_t ) task1_LED,
								(char *					) "task1_LED",
								(uint16_t				) TASK1_LED_STACK ,
								(void *					) NULL,
								(UBaseType_t		) TASK1_LED_PRIO,
								(TaskHandle_t*	) &Task1LED_Handle ) ;
								
		xTaskCreate((TaskFunction_t ) task2_LED,
								(char *					) "task2_LED",
								(uint16_t				) TASK2_LED_STACK ,
								(void *					) NULL,
								(UBaseType_t		) TASK2_LED_PRIO,
								(TaskHandle_t*	) &Task2LED_Handle ) ;
		vTaskDelete(NULL);//Startask_Handle
}

void task1_LED(void* pvParameters)
{
	u8 task1_num=0;
	while(1)
	{
		task1_num++;
		//printf("task1_num:%d\r\n",task1_num);
		if(task1_num==5)
		{
			vTaskDelete(Task2LED_Handle);
			printf("Del_Task2!!\r\n");
		}
		LED0=0;
		vTaskDelay(200);
		LED0=1;
		vTaskDelay(800);
		printf("task1_num:%d\r\n",task1_num);
		
	}
}


void task2_LED(void* pvParameters)
{
	u8 task2_num=0;
	while(1)
	{
		task2_num++;
		
		LED1=0;
		vTaskDelay(500);
		LED1=1;
		vTaskDelay(500);
		printf("task2_num:%d\r\n",task2_num);
	}
}


