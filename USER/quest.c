#include "quest.h"

state game_state;
result game_result;

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
