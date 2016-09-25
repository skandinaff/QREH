#include "motor_driver.h"

#include "delay.h"
#include "stdio.h"


/* Private define ------------------------------------------------------------*/

/* Setting the microstep precision */
#define FULL_STEP()					(GPIO_ResetBits(STEP_PORT2, STEP_MS1));(GPIO_ResetBits(STEP_PORT2, STEP_MS2));
#define HALF_STEP()					(GPIO_SetBits(STEP_PORT2, STEP_MS1));(GPIO_ResetBits(STEP_PORT2, STEP_MS2));
#define QUARTER_STEP()			(GPIO_ResetBits(STEP_PORT2, STEP_MS1));(GPIO_SetBits(STEP_PORT2, STEP_MS2));
#define SIXSTEENTH_STEP()		(GPIO_SetBits(STEP_PORT2, STEP_MS1));(GPIO_SetBits(STEP_PORT2, STEP_MS2));
/* Selecting motor rotation direction */
#define DIR1_FORWARD()			(GPIO_SetBits(STEP_PORT1, STEP1_DIR))
#define DIR1_REVERSE()			(GPIO_ResetBits(STEP_PORT1, STEP1_DIR))
#define DIR2_FORWARD()			(GPIO_SetBits(STEP_PORT1, STEP2_DIR))
#define DIR2_REVERSE()			(GPIO_ResetBits(STEP_PORT1, STEP2_DIR))
/* Reading end switches */
#define READ_MASTER_START_POINT()		(GPIO_ReadInputDataBit(END_POINT_PORT, MASTER_START_POINT))
#define READ_MASTER_END_POINT()			(GPIO_ReadInputDataBit(END_POINT_PORT, MASTER_END_POINT))
#define READ_USER_START_POINT()			(GPIO_ReadInputDataBit(END_POINT_PORT, USER_START_POINT))
#define READ_USER_END_POINT()				(GPIO_ReadInputDataBit(END_POINT_PORT, USER_END_POINT))
/* Reading photodiode to sense the ball */
#define READ_BONUS_SENS_LOW()				(GPIO_ReadInputDataBit(BONUS_SENS_PORT, BONUS_SENS_LOW))
#define READ_BONUS_SENS_MED()				(GPIO_ReadInputDataBit(BONUS_SENS_PORT, BONUS_SENS_MED))
#define READ_BONUS_SENS_HIGH()			(GPIO_ReadInputDataBit(BONUS_SENS_PORT, BONUS_SENS_HIGH))
/* Avaliable speeds. I assume greater number means less speed */
#define AUTOMAT_SPEED								1000
#define USER_BASE_SPEED							1100
#define USER_BONUS_LOW_SPEED				950
#define USER_BONUS_MED_SPEED				800
#define USER_BONUS_HIGH_SPEED				650


void MotorInit (void)
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
			//vTaskDelay(1);
			delay_ms(1);
		} while (READ_MASTER_START_POINT() != 0);
		STEP2_RES_0();
		STEP2_EN_1();
		TIM_Cmd(TIM2, DISABLE);
		LCD_Puts("Computer Motor \n Found Start", 1, 1, DARK_BLUE, WHITE,1,1);
	}
	if (READ_USER_START_POINT() != 0){
		DIR1_REVERSE();
		STEP1_RES_1();
		STEP1_EN_0();
		TIM3->ARR = 200;
		TIM_Cmd(TIM3, ENABLE);
		do {
			//vTaskDelay(1);
			delay_ms(1);
		} while (READ_USER_START_POINT() != 0);
		STEP1_RES_0();
		STEP1_EN_1();
		TIM_Cmd(TIM3, DISABLE);
		LCD_Puts("User Motor \n Found Start", 1, 1, DARK_BLUE, WHITE,1,1);
	}

	QUARTER_STEP();
	DIR1_FORWARD();
	DIR2_FORWARD();
	
	TIM2->ARR = 1200;						// Setting computer's speed 
	TIM3->ARR = 1400;						// Setting user's base speed
}

unsigned char HorseRace (void) 
{
	unsigned int bonus_speed_time = 0;
	
	QUARTER_STEP();
	DIR1_FORWARD();
	DIR2_FORWARD();
	STEP1_RES_1();
	STEP2_RES_1();
	STEP1_EN_0();
	STEP2_EN_0();
	//vTaskDelay(50);
	delay_ms(50);
	TIM2->ARR = AUTOMAT_SPEED;
	TIM3->ARR = USER_BASE_SPEED;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
  while(1)
  {
		check_usart_while_playing();
		
		if (READ_MASTER_END_POINT() == 0) {													
			STEP1_RES_0();
			STEP2_RES_0();
			STEP1_EN_1();
			STEP2_EN_1();
			set_task_counter(get_task_counter() + 1); // Added by me
			return 0;									
		}
		if (READ_USER_END_POINT() == 0) {														
			STEP1_RES_0();
			STEP2_RES_0();
			STEP1_EN_1();
			STEP2_EN_1();
			set_task_counter(get_task_counter() + 1); // Added by me
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
		//vTaskDelay(10);
		delay_ms(10);
  }
}
