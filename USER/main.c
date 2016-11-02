/****************************************************************************************************
** File name:               main.c
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mcu_config.h"
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
//ADC_Configuration();
	
	GPIO_SetBits(LED_PORT, LED1);
	
	STEP1_RES_0();
	STEP2_RES_0();
	
	LCD_init();
	LCD_FillScreen(WHITE);
	LCD_Puts("First startup", 1, 30, DARK_BLUE, WHITE,1,1);
	
	GPIO_SetBits(MAGNET_PORT, MAGNET);
	
	
  while(1){
	/*
		int adcval = readADC1(2);
		char adcval_str[8];
		sprintf(adcval_str, "%4d", adcval);
		LCD_Puts(adcval_str, 1, 20, DARK_BLUE, WHITE,1,1);
		if(adcval>10)  BlinkOnboardLED(3);
		*/
		
		
		if (usart_has_data()) {
			
			
			
			usart_get_data_packet(packet);
			incoming_packet = usart_packet_parser(packet);
			
			if (usart_validate_crc8(incoming_packet) && usart_packet_is_addressed_to_me(incoming_packet)){
			/*	
				GPIO_SetBits(LED_PORT, LED2);
				delay_ms(25);
				GPIO_ResetBits(LED_PORT, LED2);
			*/
				switch (incoming_packet.instruction) {
					case INSTR_MASTER_TEST:
						//set_idle_received(false);
						//LCD_Puts("Test received!", 1, 1, DARK_BLUE, WHITE,1,1);
						SendInstruction(INSTR_SLAVE_READY);
						break;
					case INSTR_MASTER_WORK_START:
						set_idle_received(false);
						while (get_task_counter() <= TASK_COUNT) {
							GPIO_SetBits(STATE_LED_PORT, STATE_LED);
							PerformQuest();
							if(get_break_flag()){
								set_task_counter(0);
								set_break_flag(false);
								break;
							}
						}
						break;
					case INSTR_MASTER_STATUS_REQ:				
						/*
						if (get_task_counter() == TASK_COUNT) {
							SendInstruction(INSTR_SLAVE_COMPLETED);
						} else {
							SendInstruction(INSTR_SLAVE_NOT_COMLETED);
						}*/
						break;
					case INSTR_MASTER_SET_IDLE:
						
						/*if(!Check_if_one_at_start()) {
							MotorInit(); 
						}*/
						
						//if(get_idle_received()==false){
							//LCD_FillScreen(WHITE);
							//LCD_Puts("Idled between games", 1, 30, DARK_BLUE, WHITE,1,1);
							GPIO_ResetBits(STATE_LED_PORT, STATE_LED);
							Check_if_both_arrived(true);
							set_task_counter(0);
							set_game_state(false);
							set_break_flag(true);
							//Emergency_Stop();
						//}
						//set_idle_received(true);
					
					
						
					
						/*
						set_task_counter(0);
						set_break_flag(true);
						set_game_state(false);
						*/
						//if(!Check_if_one_at_start()) MotorInit();
						break;
					case SYS_RESET:
						NVIC_SystemReset();
						break;
					case TEST_MOTOR:
						MotorTest(incoming_packet.motor_sel_dir, 108+10*incoming_packet.motor_speed); // 108 is minimal speed possible
						break;

				}
			}
		}
			set_task_counter(0);
	}
}





void PerformQuest(void){
	
	int task_counter = get_task_counter();

	LCD_Puts("Idle between games", 1, 30, DARK_BLUE, WHITE,1,1);
	check_usart_while_playing();

	
	// Initializations
	switch (task_counter) {
		case 0:	// Horses
			if(!Check_if_both_at_start()) MotorInit();
			break;
	}

	while (task_counter == get_task_counter()) {
		switch (task_counter) {
			case 0:	// 
				if(!Check_if_both_at_start()) MotorInit();
			  HorseRace();
				break;
			case 1:
				
				if(get_game_state()) {
					GPIO_ResetBits(STATE_LED_PORT, STATE_LED);
					LCD_Puts("Game over!", 1, 1, DARK_BLUE, WHITE,1,1);
					open_magnet();
					while(get_game_state()) {
						check_usart_while_playing();
					}
					return;
				}
				if(!get_game_state()) {
					LCD_Puts("Game starts again", 1, 1, DARK_BLUE, WHITE,1,1);
					set_game_state(false);
					task_counter = 0;
					set_task_counter(0);
					break;
				}
				break;
		}

		check_usart_while_playing();
		if (get_break_flag()) return;
	}



}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
