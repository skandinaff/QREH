/*******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *****************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "ErrorProcessing.h"
#include "mcu_config.h"

/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern xQueueHandle xQueue_usart_data_in;				// дескриптор очереди для данных от USART

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

void NMI_Handler(void){
}


void HardFault_Handler(void){
  while (1){
  }
}

void MemManage_Handler(void){
  while (1){
  }
}

void BusFault_Handler(void){
  while (1){
  }
}

void UsageFault_Handler(void){
  while (1){
  }
}

void DebugMon_Handler(void){
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/






/*****END OF FILE****/
