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
	
	LCD_init();
	LCD_FillScreen(WHITE);

	
	GPIO_SetBits(MAGNET_PORT, MAGNET);
	
	SendInstruction(INSTR_SLAVE_READY);
	
  while(1){
	
		check_usart_while_playing();
		
		switch(get_game_state()){
			case IDLE:
				LCD_Puts("Status: Idle", 1, 30, DARK_BLUE, WHITE,1,1);
				GPIO_ResetBits(STATE_LED_PORT, STATE_LED);
				//Check_if_both_arrived(true);
				Emergency_Stop();
				if(!Check_if_one_at_start()) {
					MotorInit(); 
				} 

				break;
			case GAME:
				LCD_Puts("Status: Game", 1, 30, DARK_BLUE, WHITE,1,1);
				HorseRace();
				break;
		}

	}
}











/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
