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
#define AUTOMAT_SPEED								450 // was 1200 initally // The was 1150 - 6.45 minutes //
#define USER_BASE_SPEED							2800 // was 1400

#define USER_BONUS_LOW_SPEED				400 // was 950
#define USER_BONUS_MED_SPEED				250 // was 800
#define USER_BONUS_HIGH_SPEED				150 // was 650

volatile bool master_start = false;
volatile bool user_start = false;

bonus game_bonus = NONE;

volatile float incr;
volatile int incr_count = 1;

void MotorInit (void)
{
	//LCD_Puts("Please Init motors!", 1, 50, DARK_BLUE, WHITE,1,1);
	STEP1_RES_0();
	STEP2_RES_0();
	STEP1_EN_1();
	STEP2_EN_1();
	FULL_STEP();
	
	DIR1_REVERSE();
	DIR2_REVERSE();
	
	STEP1_RES_1();	
	STEP2_RES_1();
	STEP1_EN_0();
	STEP2_EN_0();
	
		
	TIM2->ARR = 200;
	TIM3->ARR = 200;
		
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
  while( !Check_if_both_arrived(false) )
  {
		check_usart_while_playing();
		
		if (READ_MASTER_START_POINT() == 0) {													
			STEP1_RES_0();
			STEP1_EN_1();
			TIM_Cmd(TIM3, DISABLE);
			user_start = true;
			//LCD_Puts("Master at start!", 1, 60, DARK_BLUE, WHITE,1,1);
		}
		if (READ_USER_START_POINT() == 0) {														
			STEP2_RES_0();
			STEP2_EN_1();
			TIM_Cmd(TIM2, DISABLE);
			master_start = true;
			//LCD_Puts("User at start!", 1, 70, DARK_BLUE, WHITE,1,1);
		}
		delay_ms(1);
	}
	//LCD_Puts("                   ", 1, 50, DARK_BLUE, WHITE,1,1);
	//LCD_Puts("                   ", 1, 60, DARK_BLUE, WHITE,1,1);
	//LCD_Puts("                   ", 1, 70, DARK_BLUE, WHITE,1,1);
	Check_if_both_arrived(true);

}

unsigned char HorseRace (void) 
{

	unsigned int bonus_speed_time = 0;
		//LCD_Puts("Game on!            ", 1, 20, DARK_BLUE, WHITE,1,1);
	QUARTER_STEP();
	DIR1_FORWARD();
	DIR2_FORWARD();
	STEP1_RES_1();
	STEP2_RES_1();
	STEP1_EN_0();
	STEP2_EN_0();
	delay_ms(50);
	TIM2->ARR = AUTOMAT_SPEED;
	TIM3->ARR = USER_BASE_SPEED;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
  while(1)
  {
		if (get_game_state()==IDLE) return 0;
		
		check_usart_while_playing();
		
		if (READ_MASTER_END_POINT() == 0) {													
			STEP1_RES_0();
			STEP2_RES_0();
			STEP1_EN_1();
			STEP2_EN_1();
			set_game_result(COMPLETED); // Swapped places. Appears that motors are connected other way around
			return 0;									
		}
		if (READ_USER_END_POINT() == 0) {														
			STEP1_RES_0();
			STEP2_RES_0();
			STEP1_EN_1();
			STEP2_EN_1();
			MotorInit();
			return 0;
		}
		
		if (bonus_speed_time > 0) {
			bonus_speed_time--;
			
			if(bonus_speed_time < 150){ // initially = 250
				
				if(game_bonus == LOW){
				 incr = (USER_BASE_SPEED - USER_BONUS_LOW_SPEED)/bonus_speed_time;
				 TIM3->ARR = USER_BONUS_LOW_SPEED + incr*incr_count;
				}
				
				if(game_bonus == MED){
				 incr = (USER_BASE_SPEED - USER_BONUS_MED_SPEED)/bonus_speed_time;
				 TIM3->ARR = USER_BONUS_MED_SPEED + incr*incr_count;
				}
				
				if(game_bonus == HIGH){
				 incr = (USER_BASE_SPEED - USER_BONUS_HIGH_SPEED)/bonus_speed_time;
				 TIM3->ARR = USER_BONUS_HIGH_SPEED + incr*incr_count;
				}
				incr_count+=2; // Initally = 1
			}
			

			
			if (bonus_speed_time == 0) {
				//LCD_Puts("             ", 1, 1, DARK_BLUE, WHITE,1,1);
				//LCD_Puts("             ", 1, 10, DARK_BLUE, WHITE,1,1);
				//LCD_Puts("             ", 1, 20, DARK_BLUE, WHITE,1,1);
				incr=0;
				incr_count=1;
				game_bonus=NONE;
				TIM3->ARR = USER_BASE_SPEED;
			}
		}
		if(game_bonus == NONE){
			if (READ_BONUS_SENS_LOW() != 0) {
				//LCD_Puts("LOW BONUS!", 1, 1, DARK_BLUE, WHITE,1,1);
				bonus_speed_time = 500;
				game_bonus = LOW;
				TIM3->ARR = USER_BONUS_LOW_SPEED;
			}
			if (READ_BONUS_SENS_MED() != 0) {
				//LCD_Puts("MEDIUM BONUS!", 1, 10, DARK_BLUE, WHITE,1,1);
				bonus_speed_time = 500;
				game_bonus = MED;
				TIM3->ARR = USER_BONUS_MED_SPEED;
			}
			if (READ_BONUS_SENS_HIGH() != 0) {
				//LCD_Puts("HIGH BONUS!", 1, 20, DARK_BLUE, WHITE,1,1);
				bonus_speed_time = 500;
				game_bonus = HIGH;
				TIM3->ARR = USER_BONUS_HIGH_SPEED;
			}
		}
		 delay_ms(10);
  }
}

void MotorTest(char sel_dir, int speed){

	STEP1_RES_0();
	STEP2_RES_0();
	
	STEP1_EN_1();
	STEP2_EN_1();
	
	FULL_STEP();
		
	if(sel_dir == M1F || sel_dir == M1R){
		STEP1_RES_1();
		STEP1_EN_0();
		if(sel_dir == M1F) DIR1_FORWARD();
		if(sel_dir == M1R) DIR1_REVERSE();
		TIM3->ARR = speed;
		TIM_Cmd(TIM3, ENABLE);
	}
	
	if(sel_dir == M2F || sel_dir == M2R){
		STEP2_RES_1();
		STEP2_EN_0();
		if(sel_dir == M2F) DIR2_FORWARD();
		if(sel_dir == M2R) DIR2_REVERSE();
		TIM2->ARR = speed;
		TIM_Cmd(TIM2, ENABLE);
	}	
	
	/* Once motor is initializet, spin it!*/
	
	if(sel_dir == M1F){
	do {
		//vTaskDelay(1);
		check_usart_while_playing();
		delay_ms(1);
	} while (READ_MASTER_END_POINT() != 0);
	STEP1_RES_0();
	STEP1_EN_1();
	TIM_Cmd(TIM3, DISABLE);
	}
	
	if(sel_dir == M1R){
	do {
		//vTaskDelay(1);
		check_usart_while_playing();
		delay_ms(1);
	} while (READ_MASTER_START_POINT() != 0);
	STEP1_RES_0();
	STEP1_EN_1();
	TIM_Cmd(TIM3, DISABLE);
	}

	if(sel_dir == M2F){
	do {
		//vTaskDelay(1);
		delay_ms(1);
		check_usart_while_playing();
	} while (READ_USER_END_POINT() != 0);
	STEP2_RES_0();
	STEP2_EN_1();
	TIM_Cmd(TIM2, DISABLE);
	}
	
	if(sel_dir == M2R){
	do {
		//vTaskDelay(1);
		delay_ms(1);
		check_usart_while_playing();
	} while (READ_USER_START_POINT() != 0);
	STEP2_RES_0();
	STEP2_EN_1();
	TIM_Cmd(TIM2, DISABLE);
	}
			
}

bool Check_if_both_arrived(bool reset){
	
	if(reset){
		user_start=false;
		master_start=false;
		return false;
	}
	
	if( user_start == true && master_start == true ) return true;
	else return false;
}

bool Check_if_one_at_start(void){
	if(READ_MASTER_START_POINT() == 0 || READ_USER_START_POINT() == 0) return true;
	else return false;
}

bool Check_if_both_at_start(void){
	if(READ_MASTER_START_POINT() == 0 && READ_USER_START_POINT() == 0) return true;
	else return false;
}


void Emergency_Stop(void){
		STEP1_RES_0();
		STEP2_RES_0();
		STEP1_EN_1();
		STEP2_EN_1();
		TIM_Cmd(TIM2, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		return;
}

	

