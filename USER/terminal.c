/****************************************************************************************************
** File name:      terminal.c
** функции работы с RS485
****************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mcu_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ErrorProcessing.h"
#include "delay.h"

#include "LPH8731-3C.h"
#include "stdio.h"

/* Private variables ---------------------------------------------------------*/
extern xQueueHandle xQueue_usart_data_in;						// дескриптор очереди для данных от USART
extern xQueueHandle xQueue_usart_data_out;					// дескриптор очереди для данных в USART
extern unsigned char	rx_buffer[DATA_PACKET_LEN];		// пакет на прием (USART)
extern unsigned char quest_start_flag;

/* Private function prototypes ---------------------------------------------------------*/
void cmd_parcer (unsigned char *packet);

/*======================================================================================*/

/*************************************************************************************************************
*    Прием данных и формирование пакета
*************************************************************************************************************/
void Usart485terminal (void)
{
	unsigned char usart_packet[DATA_PACKET_LEN];
	unsigned char	rx_from_xQueue_rx_data;
	unsigned char index = 0;
	portBASE_TYPE xStatus;	
	
	do
	{
		xStatus = xQueueReceive(xQueue_usart_data_in, &rx_from_xQueue_rx_data, ( portTickType ) 10);							// считываем и обрабатываем
		if (xStatus != pdPASS) SendError(GLOBAL_ERROR);																														// Если попытка чтения не ok — индицировать ошибку.
			
		if (rx_from_xQueue_rx_data == MASTER_START_BYTE)																													// пришел признак начала пакета?
		{
			if (index > (DATA_PACKET_LEN-1)) index = 0;																															// указатель в 0 и набираем пакет
		}
		usart_packet[index++] = rx_from_xQueue_rx_data;																														// запись данных в пакет и увеличение указателя
		if (index == DATA_PACKET_LEN)																																							// пакет набран?
		{
			index = 0;																																															// сбрасываем указатель
			if ((usart_packet[0] == MASTER_START_BYTE) && (usart_packet[DATA_PACKET_LEN-1] == STOP_BYTE)) 
				cmd_parcer(usart_packet);																																							// вызываем обработчик
			usart_packet[0] = 0;																																										// обнуляем стартовый контрольный байт
			usart_packet[DATA_PACKET_LEN-1] = 0;																																		// обнуляем стоповый контрольный байт
		}
		
		vTaskDelay(1);
	}while (uxQueueMessagesWaiting(xQueue_usart_data_in) != 0);																									// крутим, пока есть данные в очереди
}

/*******************************************************************************
* Function Name  : cmd_parcer
* Description    : распарсиваем запрос
*******************************************************************************/
void cmd_parcer (unsigned char *packet)
{
	char lcd_buf[70];
	unsigned char	tx_pack[DATA_PACKET_LEN];
	portBASE_TYPE xStatus;
	
	if (packet[1] != QUEST_ID) return;
	
	tx_pack[0] = SLAVE_START_BYTE;
	tx_pack[1] = QUEST_ID;
	tx_pack[2] = 0x01;
	tx_pack[4] = STOP_BYTE;
	tx_pack[DATA_PACKET_LEN-1] = STOP_BYTE;
	
	switch (packet[2])																										// распарсиваем запрос
	{
		case CMD_MASTER_TEST:																								// gotov ili ne gotov k rabote
			tx_pack[2] = AppState;
/*			if (AppState == QUEST_READY){
				tx_pack[2] = CMD_SLAVE_READY;
			} else {
				tx_pack[2] = CMD_SLAVE_NOT_READY;
			}
		*/
			// подсчитать контрольную сумму и всунуть в tx_pack[4]
			delay_ms(5);
			xStatus = xQueueSendToBack(xQueue_usart_data_out, &tx_pack, 0);
			if (xStatus != pdPASS) SendError(GLOBAL_ERROR);										// Если попытка записи не была успешной — индицировать ошибку.
			break;
			
		case CMD_MASTER_WORK_START:																					// nacinaem kvest
			quest_start_flag = 1;
			break;
		
		case CMD_MASTER_STATUS_REQ:																					// zapros vipolnen kvest ili net
			tx_pack[2] = AppState;
/*			if (AppState == QUEST_COMPLETED){
				tx_pack[2] = CMD_SLAVE_COMPLETED;
			} else {
				tx_pack[2] = CMD_SLAVE_NOT_COMLETED;
			}
		*/
			// подсчитать контрольную сумму и всунуть в tx_pack[4]
			delay_ms(5);
			xStatus = xQueueSendToBack(xQueue_usart_data_out, &tx_pack, 0);
			if (xStatus != pdPASS) SendError(GLOBAL_ERROR);										// Если попытка записи не была успешной — индицировать ошибку.
			break;
			
		case CMD_MASTER_SET_IDLE:																						// kvest vipolnen, vse pogasitj, vernutj v ishodnoe sostojanie i zdatj zapros Test ili Work_start
			quest_start_flag = 0;
			break;
		
		case 0xFF:
			sprintf (lcd_buf,"%02X %02X %02X %02X %02X", packet[0], packet[1], packet[2], packet[3], packet[4]);
			LCD_Puts(lcd_buf, 1, 60, DARK_BLUE, WHITE,1,1);
	
			tx_pack[2] = AppState;
			tx_pack[4] = STOP_BYTE;
			tx_pack[DATA_PACKET_LEN-1] = STOP_BYTE;
		
			delay_ms(5);

			tx_pack[3] = packet[3];
			xStatus = xQueueSendToBack(xQueue_usart_data_out, &tx_pack, 0);
			if (xStatus != pdPASS) SendError(GLOBAL_ERROR);																// Если попытка записи не была успешной — индицировать ошибку.
		
			sprintf (lcd_buf,"%02X %02X %02X %02X %02X", tx_pack[0], tx_pack[1], tx_pack[2], tx_pack[3], tx_pack[4]);
			LCD_Puts(lcd_buf, 1, 40, DARK_BLUE, WHITE,1,1);
			break;
		
		default:
			break;
	}
}


