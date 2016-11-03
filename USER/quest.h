#ifndef QUEST_H
#define QUEST_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stdlib.h"

typedef enum {IDLE, GAME} state;
typedef enum {COMPLETED, NOT_COMPLETED} result;

state get_game_state(void);
void set_game_state(state s);

result get_game_result(void);
void set_game_result(result r);

#endif
