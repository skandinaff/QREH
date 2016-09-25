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

uint8_t SendInstruction(unsigned char instruction);
void check_usart_while_playing(void);
void PerformQuest(void);

bool break_flag = false;

/*******************************************************************************
* Function Name  : main
* группировка прерываний, запуск фоновой задачи
*******************************************************************************/
int main(void)
{

	unsigned char packet[DATA_PACKET_LEN + 1];
	incoming_packet_t incoming_packet;
	
  ChipInit();
	
	//init_usart();
	
	GPIO_SetBits(LED_PORT, LED1);
	
	STEP1_RES_0();
	STEP2_RES_0();
	
	LCD_init();
	LCD_FillScreen(WHITE);
	LCD_Puts("Status: Idle", 1, 30, DARK_BLUE, WHITE,1,1);
	
  while(1){
	
		if (usart_has_data()) {
			
			
			
			usart_get_data_packet(packet);
			incoming_packet = usart_packet_parser(packet);
			
			if (usart_validate_crc8(incoming_packet) && usart_packet_is_addressed_to_me(incoming_packet)){
				GPIO_SetBits(LED_PORT, LED2);
				delay_ms(50);
				GPIO_ResetBits(LED_PORT, LED2);
				switch (incoming_packet.instruction) {
					case INSTR_MASTER_TEST:
						SendInstruction(INSTR_SLAVE_NOT_READY);				
						break;
					case INSTR_MASTER_WORK_START:
						while (get_task_counter() <= TASK_COUNT) {
							//GPIO_SetBits(LED_GPIO, STATE_LED);
							PerformQuest();
							if(break_flag){
								set_task_counter(0);
								break_flag = false;
								break;
							}
						}
						break;
					case INSTR_MASTER_STATUS_REQ:				
						if (get_task_counter() == TASK_COUNT) {
							SendInstruction(INSTR_SLAVE_COMPLETED);
						} else {
							SendInstruction(INSTR_SLAVE_NOT_COMLETED);
						}
						break;
					case INSTR_MASTER_SET_IDLE:
						set_task_counter(0);
						break;
					case SYS_RESET:
						NVIC_SystemReset();
						break;
				}
			}
		}
			set_task_counter(0);
	}
}

/*******************************************************************************
* Function Name  : SendInstruction
* Description    : Sends assembled packet via 485
*******************************************************************************/

uint8_t SendInstruction(unsigned char instruction){
	unsigned char* packet = malloc((OUTGOING_PACKET_LENGTH + 1) * sizeof(char));
	outgoing_packet_t outgoing_packet = usart_assemble_response(instruction);	
	GPIO_SetBits(USART_PORT, RS485DIR_PIN);
	usart_convert_outgoing_packet(packet, outgoing_packet);
	put_str(packet);
	delay_ms(100);
	free(packet);
	GPIO_ResetBits(USART_PORT, RS485DIR_PIN);
	return 1;
}

/******************************************************************************
* Function Name  : check_usart_while_playing
* Description    : Listens for incoming commands via 485 while quest is in process
*******************************************************************************/
void check_usart_while_playing(){
		incoming_packet_t incoming_packet;
	
		unsigned char* packet = malloc((OUTGOING_PACKET_LENGTH + 1) * sizeof(char));
	
		if (usart_has_data()) {
			
			usart_get_data_packet(packet);
			incoming_packet = usart_packet_parser(packet);
			if (usart_validate_crc8(incoming_packet) && usart_packet_is_addressed_to_me(incoming_packet)){
			BlinkOnboardLED(2);
				switch (incoming_packet.instruction) {
					case INSTR_MASTER_TEST:
						SendInstruction(INSTR_SLAVE_NOT_READY); 
						break;
					case INSTR_MASTER_WORK_START:
						
						break;
					case INSTR_MASTER_STATUS_REQ:				
						if (get_task_counter() == TASK_COUNT) {
							SendInstruction(INSTR_SLAVE_COMPLETED);
						} else {
							SendInstruction(INSTR_SLAVE_NOT_COMLETED);

						}
						break;
					case INSTR_MASTER_SET_IDLE:
						//GPIO_ResetBits(LED_GPIO, STATE_LED);
						break_flag = true;
						return;
					case CINSTR_GOTO_END:
						set_task_counter(get_task_counter() + 1); // Skips a task
						PerformQuest();
						break;
					case CINSTR_RESTART_TASK:
						set_task_counter(get_task_counter());
						PerformQuest();
						break;
					case TASK_REQUEST:
					  SendInstruction(get_task_counter()+1);
						break;
					case SYS_RESET:
						NVIC_SystemReset();
						break;
				}	
			}
		}
		
		free(packet);

}

void PerformQuest(void){
	
	int task_counter = get_task_counter();


	check_usart_while_playing();

	
	
	switch (task_counter) {
		case 0:	// Horses
			MotorInit();
			break;
	}

	while (task_counter == get_task_counter()) {
		switch (task_counter) {
			case 0:	// Horses
			  // Move horses here
				break;
		}

		check_usart_while_playing();
		if (break_flag) return;
	}



}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
