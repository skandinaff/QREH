#include "quest.h"

state game_state;
result game_result;
unsigned char sound = 0xFF;

state get_game_state(){
	return game_state;
}

void set_game_state(state s){
	game_state = s;
}

result get_game_result(){
	return game_result;
}

void set_game_result(result r){
	game_result = r;
}

void set_sound(unsigned char s){
	sound = s;
}

unsigned char get_sound(void){
	return sound;
}
