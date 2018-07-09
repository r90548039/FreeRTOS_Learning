#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"
/************************************************
 ALIENTEK ̽����STM32F407������ FreeRTOSʵ��2-1
 FreeRTOS��ֲʵ��-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define START_TASK_STACK  120
#define START_TASK_PRIO   1
void start_task(void* pvParameters);
TaskHandle_t StartTask_Handle;


//#define TASK1_LED_STACK  120
//#define TASK1_LED_PRIO   2
//void task1_LED(void* pvParameters);
//TaskHandle_t Task1LED_Handle;	

//#define TASK2_LED_STACK  120
//#define TASK2_LED_PRIO   3
//TaskHandle_t Task2LED_Handle;	
//void task2_LED(void* pvParameters);

//#define KEY_TASK_STACK  120
//#define KEY_TASK_PRIO   4
//TaskHandle_t Key_Task_Handle;	
//void key_task(void* pvParameters);

#define TASK_INTERRUPT_STACK 120
#define TASK_INTERRUPT_PRIO  2
TaskHandle_t InterruptTask_Handle;
void Interrupt_Task(void);

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);		//��ʼ����ʱ����
	uart_init(115200);     	//��ʼ������
	LED_Init();		        //��ʼ��LED�˿�
	Key_Init();
	TIM3_Int_Init(10000-1,8400-1);
	TIM4_Int_Init(10000-1,8400-1);
	
	xTaskCreate(	(TaskFunction_t ) start_task,
								(char *					) "start_task",
								(uint16_t				) START_TASK_STACK,
								(void *					) NULL,
								(UBaseType_t		) START_TASK_PRIO,
								(TaskHandle_t*	) &StartTask_Handle ) ;
														
        
    vTaskStartScheduler();          //�����������
}
 
void start_task(void * pvParameters)//��ʼ����
{
//		xTaskCreate((TaskFunction_t ) task1_LED,
//								(char *					) "task1_LED",
//								(uint16_t				) TASK1_LED_STACK ,
//								(void *					) NULL,
//								(UBaseType_t		) TASK1_LED_PRIO,
//								(TaskHandle_t*	) &Task1LED_Handle ) ;
//								
//		xTaskCreate((TaskFunction_t ) task2_LED,
//								(char *					) "task2_LED",
//								(uint16_t				) TASK2_LED_STACK ,
//								(void *					) NULL,
//								(UBaseType_t		) TASK2_LED_PRIO,
//								(TaskHandle_t*	) &Task2LED_Handle ) ;
//								
//		xTaskCreate((TaskFunction_t ) key_task,
//								(char *					) "key_task",
//								(uint16_t				) KEY_TASK_STACK ,
//								(void *					) NULL,
//								(UBaseType_t		) KEY_TASK_PRIO,
//								(TaskHandle_t*	) &Key_Task_Handle ) ;
			xTaskCreate((TaskFunction_t ) Interrupt_Task,
									(char *					) "Interrupt_Task",
									(uint16_t				) TASK_INTERRUPT_STACK ,
									(void *					) NULL,
									(UBaseType_t		) TASK_INTERRUPT_PRIO,
									(TaskHandle_t*	) &InterruptTask_Handle ) ;
		vTaskDelete(NULL);//Startask_Handle
}

//void task1_LED(void* pvParameters)
//{
//	u8 task1_num=0;
//	while(1)
//	{
//		task1_num++;
//		//printf("task1_num:%d\r\n",task1_num);
////		if(task1_num==5)
////		{
////			vTaskDelete(Task2LED_Handle);
////			printf("Del_Task2!!\r\n");
////		}
//		LED0=0;
//		vTaskDelay(200);
//		LED0=1;
//		vTaskDelay(800);
//		printf("task1_num:%d\r\n",task1_num);
//		
//	}
//}


//void task2_LED(void* pvParameters)
//{
//	u8 task2_num=0;
//	while(1)
//	{
//		task2_num++;
//		
//		LED1=0;
//		vTaskDelay(500);
//		LED1=1;
//		vTaskDelay(500);
//		printf("task2_num:%d\r\n",task2_num);
//	}
//}

//void key_task(void* pvParameters)
//{
//	u8 key;
//	while(1)
//	{
//		key = Key_Scan(0);
//		switch(key)
//		{
//			case KEY0_PRES:
//				vTaskSuspend(Task1LED_Handle);
//				printf("Suspend task1");
//			break;
//			case KEY1_PRES:
//				vTaskResume(Task1LED_Handle);
//				printf("Resume task1");
//			break;
//			case WKUP_PRES:
//				break;
//		}
//		vTaskDelay(10);
//	}
//	
//}

void Interrupt_Task()
{
	static u8 num=1;
	while(1)
	{
		num++;
		if (num==5)
		{
			portDISABLE_INTERRUPTS();
			printf("Delete TIM4");
			
		  delay_xms(5000); 
		
			portENABLE_INTERRUPTS();
			printf("Open Interrupt TIM4");
		}
	 
		vTaskDelay(1000);
	}

}


