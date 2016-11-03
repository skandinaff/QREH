/*********************************************
*	Chip main config
*********************************************/

#include "mcu_config.h"

/* ----------------------------------- Typedef -----------------------------------------------------------*/

uint8_t _task_counter = 0; // Here we'll have only 2 states. 0 - Waiting to start quest. 1 - quest, and when it's finished - back to 0

/*******************************************************************************
* Function Name  : ChipInit
*******************************************************************************/
void ChipInit ( void )
{
	GPIO_InitTypeDef 					GPIO_InitStructure;
	USART_InitTypeDef 				USART_InitStruct;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef 				TIM_OCConfig;
	
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );						// Configure HCLK clock as SysTick clock source
	
//--------------- Clock ----------------------------------------------------------------------------------------------------
	RCC_APB2PeriphClockCmd(LED_PORT_RCC , ENABLE);
	RCC_APB2PeriphClockCmd(STATE_LED_PORT_RCC , ENABLE);
	
	RCC_APB2PeriphClockCmd(USART_PORT_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(USART_RCC, ENABLE);
	
	RCC_APB2PeriphClockCmd(STEP_PORT1_RCC , ENABLE);
	RCC_APB2PeriphClockCmd(STEP_PORT2_RCC , ENABLE);
	RCC_APB2PeriphClockCmd(STEP_PORT3_RCC , ENABLE);
	
	RCC_APB2PeriphClockCmd(END_POINT_RCC , ENABLE);
	RCC_APB2PeriphClockCmd(BONUS_SENS_RCC , ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//------------- I/O Ports --------------------------------------------------------------------------------------------------	
	GPIO_InitStructure.GPIO_Pin = LED1 | LED2 | LED3 | LED4;			// CONTROL LEDs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STATE_LED;											// STATE LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(STATE_LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MAGNET;											// MAGNET
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MAGNET_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;										// RS - 485
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RS485DIR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STEP_MS1 | STEP_MS2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(STEP_PORT2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STEP1_DIR | STEP2_DIR;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(STEP_PORT1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STEP1_PULSE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(STEP_PORT2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STEP2_PULSE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(STEP_PORT1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STEP1_EN | STEP2_EN | STEP1_RES | STEP2_RES;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(STEP_PORT3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MASTER_START_POINT | MASTER_END_POINT | USER_START_POINT | USER_END_POINT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(END_POINT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BONUS_SENS_LOW | BONUS_SENS_MED | BONUS_SENS_HIGH;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BONUS_SENS_PORT, &GPIO_InitStructure);

//----------- USART ----------------------------------------------------------------------------------------------------
  USART_InitStruct.USART_BaudRate = 19200; 																				//Скорость обмена 19200 бод
  USART_InitStruct.USART_WordLength = USART_WordLength_8b; 												//Длина слова 8 бит
  USART_InitStruct.USART_StopBits = USART_StopBits_1; 														//1 стоп-бит
  USART_InitStruct.USART_Parity = USART_Parity_No ; 															//Без проверки четности
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 		//Без аппаратного контроля
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 										//Включен передатчик и приемник USART1
  USART_Init(USART1, &USART_InitStruct); 																					//Заданные настройки сохраняем в регистрах USART1
	
	NVIC_SetPriority (USART1_IRQn, 12);
	NVIC_EnableIRQ(USART1_IRQn);																										// Включаем общие прерывания от USART
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																	// Включаем прерывания по приёму
	USART_Cmd(USART1, ENABLE);																											// Включаем USART

//------------- ADC --------------------------------------------------------------------------------------------------------

//----------- Interrupt ----------------------------------------------------------------------------------------------------

//------------ Timers ------------------------------------------------------------------------------------------------------
	TIM_TimeBaseStructure.TIM_Prescaler = 300 ;								// на шине 36 МГц Важно!!! - если TIM_Prescaler != 1 таймер clk = шина Х2
	TIM_TimeBaseStructure.TIM_Period = 800 ;									// получим     Гц 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCStructInit(&TIM_OCConfig);
	TIM_OCConfig.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM2, &TIM_OCConfig);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 300 ;								// на шине 36 МГц Важно!!! - если TIM_Prescaler != 1 таймер clk = шина Х2
	TIM_TimeBaseStructure.TIM_Period = 800 ;									// получим     Гц 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCStructInit(&TIM_OCConfig);
	TIM_OCConfig.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM3, &TIM_OCConfig);
}

//void ADC_Configuration(void)
//{
//  ADC_InitTypeDef  ADC_InitStructure;
//  /* PCLK2 is the APB2 clock */
//  /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
//  RCC_ADCCLKConfig(RCC_PCLK2_Div6);

//  /* Enable ADC1 clock so that we can talk to it */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//  /* Put everything back to power-on defaults */
//  ADC_DeInit(ADC1);

//  /* ADC1 Configuration ------------------------------------------------------*/
//  /* ADC1 and ADC2 operate independently */
//  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//  /* Disable the scan conversion so we do one at a time */
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//  /* Don't do contimuous conversions - do them on demand */
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//  /* Start conversin by software, not an external trigger */
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  /* Say how many channels would be used by the sequencer */
//  ADC_InitStructure.ADC_NbrOfChannel = 1;

//  /* Now do the setup */
//  ADC_Init(ADC1, &ADC_InitStructure);
//  /* Enable ADC1 */
//  ADC_Cmd(ADC1, ENABLE);

//  /* Enable ADC1 reset calibaration register */
//  ADC_ResetCalibration(ADC1);
//  /* Check the end of ADC1 reset calibration register */
//  while(ADC_GetResetCalibrationStatus(ADC1));
//  /* Start ADC1 calibaration */
//  ADC_StartCalibration(ADC1);
//  /* Check the end of ADC1 calibration */
//  while(ADC_GetCalibrationStatus(ADC1));
//}


//uint16_t readADC1(uint8_t channel)
//{
//  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);
//  // Start the conversion
//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//  // Wait until conversion completion
//  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
//  // Get the conversion value
//  return ADC_GetConversionValue(ADC1);
//}

void BlinkOnboardLED(uint8_t L){
	

	switch(L) {
		case 1:
			GPIO_SetBits(LED_PORT, LED1);
			delay_ms(150);
			GPIO_ResetBits(LED_PORT, LED1);
			break;
		case 2:
			GPIO_SetBits(LED_PORT, LED2);
			delay_ms(50);
			GPIO_ResetBits(LED_PORT, LED2);
			break;
		case 3:
			GPIO_SetBits(LED_PORT, LED3);
			delay_ms(150);
			GPIO_ResetBits(LED_PORT, LED3);
			break;
		case 4:
			GPIO_SetBits(LED_PORT, LED4);
			delay_ms(150);
			GPIO_ResetBits(LED_PORT, LED4);
			break;
	}

}

void reset_task_counter(void) {
    _task_counter = 0;
}


int get_task_counter(void) {
    return _task_counter;
}

void set_task_counter(int counter) {
		if(counter > _task_counter) BlinkOnboardLED(4); // To indicate change of task
    _task_counter = counter;
}

void open_magnet(void){
	GPIO_ResetBits(MAGNET_PORT, MAGNET);
	delay_ms(500);
	GPIO_SetBits(MAGNET_PORT, MAGNET);
}
