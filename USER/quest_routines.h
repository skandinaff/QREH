/****************************************************************************************************
** File name:      quest_routines.h
**  
****************************************************************************************************/

#ifndef __QUEST_ROUTINES_H
#define __QUEST_ROUTINES_H


/* Private define ------------------------------------------------------------*/
#define STEP1_RES_0()				(GPIO_ResetBits(STEP_PORT3, STEP1_RES))
#define STEP1_RES_1()				(GPIO_SetBits(STEP_PORT3, STEP1_RES))
#define STEP2_RES_0()				(GPIO_ResetBits(STEP_PORT3, STEP2_RES))
#define STEP2_RES_1()				(GPIO_SetBits(STEP_PORT3, STEP2_RES))
#define STEP1_EN_0()				(GPIO_ResetBits(STEP_PORT3, STEP1_EN))
#define STEP1_EN_1()				(GPIO_SetBits(STEP_PORT3, STEP1_EN))
#define STEP2_EN_0()				(GPIO_ResetBits(STEP_PORT3, STEP2_EN))
#define STEP2_EN_1()				(GPIO_SetBits(STEP_PORT3, STEP2_EN))


void QuestProcess (void);

#endif			// __QUEST_ROUTINES_H
