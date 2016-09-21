/*
    FreeRTOS V6.1.0 - Copyright (C) 2010 Real Time Engineers Ltd.
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1																		// Использовать вытесняющую многозадачность?
																																		// Если этот параметр поставить в 0, то получим кооперативку,
																																		// с требованием вызывать taskYIELD() для отдачи управления диспетчеру
#define configUSE_TICK_HOOK			0
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )			// Частота проца. Нужна для вычисления задержек
#define configTICK_RATE_HZ			( ( portTickType ) 1000 )						// Частота системных тиков ОС
#define configMAX_PRIORITIES		( ( unsigned portBASE_TYPE ) 5 )		// Количество приоритетов задач
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )				// Минимальный размер стека. Не влияет ни на что
																																		// кроме тех мест где создаем задачи (макроопределение)
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 15 * 1024 ) )				// Размер кучи где лежит и стек и локальные переменные.
																																		// Зависит от доступной оперативки
#define configMAX_TASK_NAME_LEN		( 16 )
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0																	// Разрядность таймера ОС. 1—16; 0—32. У STM32 SysTick 24 разрядный -> 0
#define configIDLE_SHOULD_YIELD   1																	// Этот параметр определяет поведение задач с приоритетом IDLE болтающихся
																																		// одновременно с IDLE ядра. Если стоит 1, то ядро будет отдавать управление
																																		// сразу же как только задача станет готовой к запуску. А если оставить 0,
																																		// то время будет равномерно делиться между задачами с приоритетом IDLE и IDLE ядра
/* Hook function related definitions. */
#define configUSE_IDLE_HOOK       0										// Есть ли функция перехватывающая IDLE? В FreeRTOS мы можем поставить перехват IDLE,
																											// т.е. при попадании в Idle будет выполнена некая функция. Например загон проца
																											// в спячку или еще какая нужная вещь. Если Hook на IDLE не используется, то 0
#define configUSE_TICK_HOOK				0										// Аналогичная функция, но про перехват диспетчера. Т.е. можно поставить зацепку
																											// за диспетчер и выполнять какую-нибудь фигню каждый тик


/* Используются ли сопрограммы и приоритеты для них. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Run time and task stats gathering related definitions. */
//#define configGENERATE_RUN_TIME_STATS			0
//#define configUSE_TRACE_FACILITY        		0	// Используем ли мы отладочные примочки ОС

/* Set the following definitions to 1 to include the API function, or zero to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255 (lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */

/* This is the value being used as per the ST library which permits 16 priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15


#endif /* FREERTOS_CONFIG_H */





