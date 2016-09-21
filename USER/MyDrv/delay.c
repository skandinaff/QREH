/*******************************************************************************************************
** File name:			delay.c
********************************************************************************************************/

#include "stm32f10x_rcc.h"

/* --------- FUNCTIONS -------------------------------------------------------*/
void delay_ms(unsigned int ms)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);              	// возвращает частоты различных чипов
  nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;  	// возвращает частоту HCLK в Гц, делим на 10000 и всё множим на ms
  for (; nCount!=0; nCount--)
    {
      __asm("nop");
    }
}
void delay_us(unsigned int us)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);              	// возвращает частоты различных чипов
  nCount=(RCC_Clocks.HCLK_Frequency/10000000)*us;	// возвращает частоту HCLK в Гц, делим на 10000000 и всё множим на us
  for (; nCount!=0; nCount--)
    {
      __asm("nop");
    }
}
