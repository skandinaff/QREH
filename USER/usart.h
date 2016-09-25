/* ----------------------------------------------------------
*
* Valid USART commands:
*
* 0xC1 0x0B 0x01 0x90 0xC0 Tests
* 0xC1 0x0B 0x02 0x99 0xC0 Work Start
* 0xC1 0x0B 0x03 0x9E 0xC0 Stauts request
* 0xC1 0x0B 0x04 0x8B 0xC0 Idle		
* -- Custom commands, not listed in original protocol

* 0xC1 0x0B 0x79 0xFF 0xC0 Perform system reset
*
* ---------------------------------------------------------- */

#ifndef USART_H
#define USART_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#define USART_BAUD_RATE 					19200

#define QUEST_ID									0x0B
#define DATA_PACKET_LEN		        7
#define OUTGOING_PACKET_LENGTH    5
#define MASTER_START_BYTE	        0xC1
#define SLAVE_START_BYTE	        0xC2
#define STOP_BYTE									0xC0
#define RESTRICTED_BYTE           0x7B
#define TEST_DISP									0x7D
#define TASK_REQUEST							0x7C
#define SIL_THR_REQUEST						0x7A
#define SYS_RESET									0x79

//------------- Instructions from main device
#define INSTR_MASTER_TEST					0x01
#define INSTR_MASTER_WORK_START		0x02			
#define INSTR_MASTER_STATUS_REQ		0x03			
#define INSTR_MASTER_SET_IDLE			0x04

//------------- Instructions from this device
#define INSTR_SLAVE_NOT_READY			0x01
#define INSTR_SLAVE_READY					0x02
#define INSTR_SLAVE_NOT_COMLETED	0x03			
#define INSTR_SLAVE_COMPLETED			0x04
//------------- Supplemetery instructions for test purpouses
#define CINSTR_GOTO_END           0x7F // Not end but next task
#define CINSTR_RESTART_TASK				0x7E

#define CRC_INIT_VAL 							0x00
#define CRC_POLYNOM 							0x07

// RS - 485
#define USART_PORT				GPIOA
#define USART_PORT_RCC		RCC_APB2Periph_GPIOA
#define USART_RCC					RCC_APB2Periph_USART1
#define USART_TX_PIN			GPIO_Pin_9
#define USART_RX_PIN			GPIO_Pin_10
#define RS485DIR_PIN			GPIO_Pin_8

typedef struct {
	int packet_length;
	
	char master_stat_byte;
	char slave_address;
	char instruction;
	char player_count;
	char hint_count;
	char crc8;
	char stop_byte;
} incoming_packet_t;


typedef struct {
	char slave_start_byte;
	char slave_address;
	char instruction;
	char crc8;
	char stop_byte;
} outgoing_packet_t;

void init_usart(void);
void send_data(unsigned char tx_data[DATA_PACKET_LEN]); 
void usart_put_data_on_lcd(unsigned char* input);
void USART3_IRQHandler(void);
unsigned char get_char(void);
bool usart_has_data(void);
void usart_get_data_packet(unsigned char* packet);
void put_char(uint8_t c);
void put_str(unsigned char *s); 
bool usart_packet_is_addressed_to_me(incoming_packet_t incoming_packet);
incoming_packet_t usart_packet_parser(unsigned char* packet);
outgoing_packet_t usart_assemble_response(unsigned char instruction);
void usart_convert_outgoing_packet (unsigned char* packet, outgoing_packet_t outgoing_packet);
uint8_t usart_crc8(uint8_t init, uint8_t *packet);
bool usart_validate_crc8(incoming_packet_t incoming_packet);
bool usart_break_required(void);

#endif