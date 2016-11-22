/****************************************************************************************************
** File name:               main.c
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "mcu_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "delay.h"
#include "terminal.h"
#include "ErrorProcessing.h"
#include "quest_routines.h"

#include "LPH8731-3C.h"
#include "stdio.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "usart.h"
#include "quest_routines.h"
#include "motor_driver.h"
#include "quest.h"

/* Function prototypes -----------------------------------------------*/

void PerformQuest(void);

/*******************************************************************************
* Function Name  : main
* группировка прерываний, запуск фоновой задачи
*******************************************************************************/
int main(void)
{

	unsigned char packet[DATA_PACKET_LEN + 1];
	incoming_packet_t incoming_packet;
	
  ChipInit();

	
	GPIO_SetBits(LED_PORT, LED1); // ON indicator
	
	STEP1_RES_0();
	STEP2_RES_0();

// Enable only for debugging purpouses	
	if(LCD) LCD_init();
	if(LCD) LCD_FillScreen(WHITE);

	
	GPIO_SetBits(MAGNET_PORT, MAGNET);
	
	RS485DIR_RX();
	
  while(1){
	
		check_usart_while_playing();
		
		switch(get_game_state()){
			case IDLE:
				set_game_result(NOT_COMPLETED);
				if(LCD) LCD_Puts("State: Idle", 1, 30, DARK_BLUE, WHITE,1,1);
				if(LCD) LCD_Puts("Result: ", 1, 40, DARK_BLUE, WHITE,1,1);
				if(get_game_result()==COMPLETED) if(LCD) LCD_Puts("COMPL", 50, 40, DARK_BLUE, WHITE,1,1);
				if(get_game_result()==NOT_COMPLETED) if(LCD) LCD_Puts("NOT_C", 50, 40, DARK_BLUE, WHITE,1,1);
				GPIO_ResetBits(STATE_LED_PORT, STATE_LED);
			
				//open_magnet();
			
				//Emergency_Stop();
				if(!Check_if_one_at_start()) {
					MotorInit(); 
				} 

				break;
			case GAME:
				if(LCD) LCD_Puts("State: Game", 1, 30, DARK_BLUE, WHITE,1,1);
				if(get_game_result()==COMPLETED) {
					if(LCD) LCD_Puts("COMPL", 50, 40, DARK_BLUE, WHITE,1,1);
					GPIO_ResetBits(STATE_LED_PORT, STATE_LED);
					open_magnet();
					// TODO:put a break here
				}
				if(get_game_result()==NOT_COMPLETED) {
					if(LCD) LCD_Puts("NOT_C", 50, 40, DARK_BLUE, WHITE,1,1);
					GPIO_SetBits(STATE_LED_PORT, STATE_LED);
				}
				while(get_game_result()==NOT_COMPLETED && get_game_state()==GAME) {
					HorseRace();
				}
				break;
		}

	}
}











/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
