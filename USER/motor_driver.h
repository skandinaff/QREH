#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "mcu_config.h"

#define STEP1_RES_0()				(GPIO_ResetBits(STEP_PORT3, STEP1_RES))
#define STEP1_RES_1()				(GPIO_SetBits(STEP_PORT3, STEP1_RES))
#define STEP2_RES_0()				(GPIO_ResetBits(STEP_PORT3, STEP2_RES))
#define STEP2_RES_1()				(GPIO_SetBits(STEP_PORT3, STEP2_RES))
#define STEP1_EN_0()				(GPIO_ResetBits(STEP_PORT3, STEP1_EN))
#define STEP1_EN_1()				(GPIO_SetBits(STEP_PORT3, STEP1_EN))
#define STEP2_EN_0()				(GPIO_ResetBits(STEP_PORT3, STEP2_EN))
#define STEP2_EN_1()				(GPIO_SetBits(STEP_PORT3, STEP2_EN))

void MotorInit (void);

#endif
