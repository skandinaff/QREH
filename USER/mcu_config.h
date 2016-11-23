/****************************************************************************************************
** File name:      Config.h
****************************************************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"

//------------ Основные параметры ---------------------------------------------------------------
//#define QUEST_ID					0x0B
#define TASK_COUNT 1

#define DATA_PACKET_LEN		7												// размер пакета USART
#define MASTER_START_BYTE	0xC1
#define SLAVE_START_BYTE	0xC2
#define STOP_BYTE					0xC0

typedef enum {
	QUEST_INIT = 0,													// Квест в процессе инициализации
	QUEST_READY,														// Квест готов к старту
  QUEST_IN_PROCESS,												// Квест в процессе выполнения
	QUEST_COMPLETED,												// Квест выполнен
}AppStateTypeDef;

extern AppStateTypeDef AppState;

//------------- ПРИНИМАЕМЫЕ КОМАНДЫ -------------------------------------------------------------
#define CMD_MASTER_TEST					0x01			// gotov ili ne gotov k rabote
#define CMD_MASTER_WORK_START		0x02			// nacinaem kvest
#define CMD_MASTER_STATUS_REQ		0x03			// zapros vipolnen kvest ili net
#define CMD_MASTER_SET_IDLE			0x04			// kvest vipolnen, vse pogasitj, vernutj v ishodnoe sostojanie  i zdatj zapros Test ili Work_start

//------------- ПЕРЕДАВАЕМЫЕ КОМАНДЫ ------------------------------------------------------------
#define CMD_SLAVE_NOT_READY			0x01			// ne nahoditsa v ishodnom sostojanii, ne gotov k rabote
#define CMD_SLAVE_READY					0x02 			// Ready
#define CMD_SLAVE_NOT_COMLETED	0x03			// Not_completed -   Kvest ese ne vipolnen
#define CMD_SLAVE_COMPLETED			0x04 			// Completed   Kvest vipolnen

//------------ Н О Г И --------------------------------------------------------------------------
// CONTROL LEDs
#define LED_PORT					GPIOB
#define LED_PORT_RCC			RCC_APB2Periph_GPIOB
#define LED1							GPIO_Pin_12							// 
#define LED2							GPIO_Pin_13							// 
#define LED3							GPIO_Pin_14							// 
#define LED4							GPIO_Pin_15							// Global error

#define STATE_LED_PORT 			GPIOE
#define STATE_LED_PORT_RCC	RCC_APB2Periph_GPIOE
#define STATE_LED 					GPIO_Pin_8	

#define MAGNET_PORT 				GPIOE
#define MAGNET_PORT_RCC			RCC_APB2Periph_GPIOE
#define MAGNET							GPIO_Pin_15

// RS - 485
#define USART_PORT				GPIOA
#define USART_PORT_RCC		RCC_APB2Periph_GPIOA
#define USART_RCC					RCC_APB2Periph_USART1
#define USART_TX_PIN			GPIO_Pin_9
#define USART_RX_PIN			GPIO_Pin_10
#define RS485DIR_PIN			GPIO_Pin_8

// END POINTS
#define END_POINT_PORT			GPIOC
#define END_POINT_RCC				RCC_APB2Periph_GPIOC
#define MASTER_START_POINT	GPIO_Pin_11
#define MASTER_END_POINT		GPIO_Pin_12
#define USER_START_POINT		GPIO_Pin_4
#define USER_END_POINT			GPIO_Pin_3

// BUNUS SENS
#define BONUS_SENS_PORT			GPIOE
#define BONUS_SENS_RCC			RCC_APB2Periph_GPIOE
#define BONUS_SENS_LOW			GPIO_Pin_2
#define BONUS_SENS_MED			GPIO_Pin_3
#define BONUS_SENS_HIGH			GPIO_Pin_4

// STEP MOTORS
#define STEP_PORT1				GPIOA
#define STEP_PORT2				GPIOB
#define STEP_PORT3				GPIOD
#define STEP_PORT1_RCC		RCC_APB2Periph_GPIOA
#define STEP_PORT2_RCC		RCC_APB2Periph_GPIOB
#define STEP_PORT3_RCC		RCC_APB2Periph_GPIOD
#define STEP1_PULSE				GPIO_Pin_1							// GPIOB
#define STEP2_PULSE				GPIO_Pin_2							// GPIOA
#define STEP_MS1					GPIO_Pin_11							// GPIOB
#define STEP_MS2					GPIO_Pin_10							// GPIOB
#define STEP1_DIR					GPIO_Pin_11							// GPIOA
#define STEP2_DIR					GPIO_Pin_12							// GPIOA

#define STEP1_EN					GPIO_Pin_2							// GPIOD
#define STEP2_EN					GPIO_Pin_4							// GPIOD

#define STEP1_RES					GPIO_Pin_3							// GPIOD
#define STEP2_RES					GPIO_Pin_5							// GPIOD

//------------- Макросы ногодрыга ---------------------------------------------------------------
#define RS485DIR_TX()			(GPIO_SetBits(USART_PORT, RS485DIR_PIN))
#define RS485DIR_RX()			(GPIO_ResetBits(USART_PORT, RS485DIR_PIN))

#define LED1_0()					(GPIO_ResetBits(LED_PORT, LED1));
#define LED1_1()					(GPIO_SetBits(LED_PORT, LED1));
#define LED2_0()					(GPIO_ResetBits(LED_PORT, LED2))
#define LED2_1()					(GPIO_SetBits(LED_PORT, LED2))
#define LED3_0()					(GPIO_ResetBits(LED_PORT, LED3));
#define LED3_1()					(GPIO_SetBits(LED_PORT, LED3));
#define LED4_0()					(GPIO_ResetBits(LED_PORT, LED4));
#define LED4_1()					(GPIO_SetBits(LED_PORT, LED4));


/* --------- FUNCTIONS -------------------------------------------------------*/
void ChipInit ( void );
void BlinkOnboardLED(uint8_t L);
void reset_task_counter(void);
int get_task_counter(void);
void set_task_counter(int counter);
void ADC_Configuration(void);
void open_magnet(void);
uint16_t readADC1(uint8_t channel);

#endif

