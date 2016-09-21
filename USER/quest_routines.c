/****************************************************************************************************
** File name:      quest_routines.h
**  
****************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mcu_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ErrorProcessing.h"
#include "delay.h"
#include "quest_routines.h"

#include "LPH8731-3C.h"
#include "stdio.h"

/* Private define ------------------------------------------------------------*/
#define FULL_STEP()					(GPIO_ResetBits(STEP_PORT2, STEP_MS1));(GPIO_ResetBits(STEP_PORT2, STEP_MS2));
#define HALF_STEP()					(GPIO_SetBits(STEP_PORT2, STEP_MS1));(GPIO_ResetBits(STEP_PORT2, STEP_MS2));
#define QUARTER_STEP()			(GPIO_ResetBits(STEP_PORT2, STEP_MS1));(GPIO_SetBits(STEP_PORT2, STEP_MS2));
#define SIXSTEENTH_STEP()		(GPIO_SetBits(STEP_PORT2, STEP_MS1));(GPIO_SetBits(STEP_PORT2, STEP_MS2));

#define DIR1_FORWARD()			(GPIO_SetBits(STEP_PORT1, STEP1_DIR))
#define DIR1_REVERSE()			(GPIO_ResetBits(STEP_PORT1, STEP1_DIR))
#define DIR2_FORWARD()			(GPIO_SetBits(STEP_PORT1, STEP2_DIR))
#define DIR2_REVERSE()			(GPIO_ResetBits(STEP_PORT1, STEP2_DIR))

#define READ_MASTER_START_POINT()		(GPIO_ReadInputDataBit(END_POINT_PORT, MASTER_START_POINT))
#define READ_MASTER_END_POINT()			(GPIO_ReadInputDataBit(END_POINT_PORT, MASTER_END_POINT))
#define READ_USER_START_POINT()			(GPIO_ReadInputDataBit(END_POINT_PORT, USER_START_POINT))
#define READ_USER_END_POINT()				(GPIO_ReadInputDataBit(END_POINT_PORT, USER_END_POINT))

#define READ_BONUS_SENS_LOW()				(GPIO_ReadInputDataBit(BONUS_SENS_PORT, BONUS_SENS_LOW))
#define READ_BONUS_SENS_MED()				(GPIO_ReadInputDataBit(BONUS_SENS_PORT, BONUS_SENS_MED))
#define READ_BONUS_SENS_HIGH()			(GPIO_ReadInputDataBit(BONUS_SENS_PORT, BONUS_SENS_HIGH))

#define AUTOMAT_SPEED								1000
#define USER_BASE_SPEED							1100
#define USER_BONUS_LOW_SPEED				950
#define USER_BONUS_MED_SPEED				800
#define USER_BONUS_HIGH_SPEED				650

/* Private variables ---------------------------------------------------------*/
//extern unsigned char quest_start_flag;

/* Private function prototypes ---------------------------------------------------------*/
void QuestInit (void);
unsigned char QuestInProgress (void);

/*======================================================================================*/

/*******************************************************************************
* Function Name  : QuestProcess
* Description    : Мотор автомата - TIM2 на плате верхний (питание внизу) STEP2
*                  Мотор команды  - TIM3 на плате нижний                  STEP1
*******************************************************************************/
void QuestProcess (void)
{/*
	unsigned int prescaler = 0;
	unsigned char first_init_flag = 0;
	
	vTaskDelay(200);
	//AppState = QUEST_INIT;
	QuestInit();
	first_init_flag = 1;
	
	//AppState = QUEST_READY;
	do{																			// ждем сигнала о начале квеста
		prescaler++;
		if (prescaler > 5){
			prescaler = 0;
			LED_PORT->ODR ^= LED1;
		}
		vTaskDelay(20);
	} while (quest_start_flag != 1);
	
	//AppState = QUEST_IN_PROCESS;
	GPIO_SetBits(LED_PORT, LED1);
	do{
		if (first_init_flag) {
			first_init_flag = 0;
		} else {
			QuestInit();
		}
		vTaskDelay(20);
	} while (!QuestInProgress());						// добавить проверку на сброс извне
	
	//AppState = QUEST_COMPLETED;
	do{																			// ждем сигнала вернутся в исходное
		prescaler++;
		if (prescaler > 20){
			prescaler = 0;
			LED_PORT->ODR ^= LED1;
		}
		vTaskDelay(20);
	} while (quest_start_flag != 0);				// крутим, пока не сброшен из терминала quest_start_flag
	GPIO_ResetBits(LED_PORT, LED1);
	*/
}

/*******************************************************************************
* Function Name  : QuestInProgress
* Description    : прохождение квеста
* RETURN         : возвращаем 1, когда квест выполнен
*******************************************************************************/
unsigned char QuestInProgress (void)
{
	unsigned int bonus_speed_time = 0;
	
	QUARTER_STEP();
	DIR1_FORWARD();
	DIR2_FORWARD();
	STEP1_RES_1();
	STEP2_RES_1();
	STEP1_EN_0();
	STEP2_EN_0();
	vTaskDelay(50);
	TIM2->ARR = AUTOMAT_SPEED;
	TIM3->ARR = USER_BASE_SPEED;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
  while(1)
  {
		if (READ_MASTER_END_POINT() == 0) {													// автомат первый
			STEP1_RES_0();
			STEP2_RES_0();
			STEP1_EN_1();
			STEP2_EN_1();
			return 0;									
		}
		if (READ_USER_END_POINT() == 0) {														// команда первая
			STEP1_RES_0();
			STEP2_RES_0();
			STEP1_EN_1();
			STEP2_EN_1();
			return 1;
		}
		if (bonus_speed_time > 0) {
			bonus_speed_time--;
			if (bonus_speed_time == 0) TIM3->ARR = USER_BASE_SPEED;
		}
		if (READ_BONUS_SENS_LOW() == 0) {
			bonus_speed_time = 500;
			TIM3->ARR = USER_BONUS_LOW_SPEED;
		}
		if (READ_BONUS_SENS_LOW() == 0) {
			bonus_speed_time = 500;
			TIM3->ARR = USER_BONUS_MED_SPEED;
		}
		if (READ_BONUS_SENS_LOW() == 0) {
			bonus_speed_time = 500;
			TIM3->ARR = USER_BONUS_HIGH_SPEED;
		}
		vTaskDelay(10);
  }
}

/*******************************************************************************
* Function Name  : quest_init
* Description    : установка коней на стартовую позицию
*******************************************************************************/
void QuestInit (void)
{
	STEP1_RES_0();
	STEP2_RES_0();
	STEP1_EN_1();
	STEP2_EN_1();
	FULL_STEP();
	if (READ_MASTER_START_POINT() != 0){
		DIR2_REVERSE();
		STEP2_RES_1();
		STEP2_EN_0();
		TIM2->ARR = 200;
		TIM_Cmd(TIM2, ENABLE);
		do {
			vTaskDelay(1);
		} while (READ_MASTER_START_POINT() != 0);
		STEP2_RES_0();
		STEP2_EN_1();
		TIM_Cmd(TIM2, DISABLE);
	}
	if (READ_USER_START_POINT() != 0){
		DIR1_REVERSE();
		STEP1_RES_1();
		STEP1_EN_0();
		TIM3->ARR = 200;
		TIM_Cmd(TIM3, ENABLE);
		do {
			vTaskDelay(1);
		} while (READ_USER_START_POINT() != 0);
		STEP1_RES_0();
		STEP1_EN_1();
		TIM_Cmd(TIM3, DISABLE);
	}

	QUARTER_STEP();
	DIR1_FORWARD();
	DIR2_FORWARD();
	
	TIM2->ARR = 1200;						// задаем постоянную скорость автомата
	TIM3->ARR = 1400;						// задаем базовую скорость лошади команды
}


