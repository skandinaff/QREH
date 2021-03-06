#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "mcu_config.h"
#include "LPH8731-3C.h"
#include <usart.h>

/* That is something that has been added after PCB was made.. */
/* RESET pin of the driver */
#define STEP1_RES_0()				(GPIO_ResetBits(STEP_PORT3, STEP1_RES))
#define STEP1_RES_1()				(GPIO_SetBits(STEP_PORT3, STEP1_RES))
#define STEP2_RES_0()				(GPIO_ResetBits(STEP_PORT3, STEP2_RES))
#define STEP2_RES_1()				(GPIO_SetBits(STEP_PORT3, STEP2_RES))
/* ENABLE pin of the driver */
#define STEP1_EN_0()				(GPIO_ResetBits(STEP_PORT3, STEP1_EN))
#define STEP1_EN_1()				(GPIO_SetBits(STEP_PORT3, STEP1_EN))
#define STEP2_EN_0()				(GPIO_ResetBits(STEP_PORT3, STEP2_EN))
#define STEP2_EN_1()				(GPIO_SetBits(STEP_PORT3, STEP2_EN))

typedef enum {HIGH, MED, LOW, NONE} bonus;

void MotorInit (void);
unsigned char HorseRace (void);
void MotorTest(char dir, int speed);
bool Check_if_both_arrived(bool reset);
bool Check_if_one_at_start(void);
bool Check_if_both_at_start(void);
void Emergency_Stop(void);

#endif
