/*
 * game_logic.c
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#include "game_logic.h"
#include "stdint.h"
GameEvents event_queue[10]={0};
uint8_t event_tail=0;
uint8_t event_head=0;

GameSubState GameSubStateMachine=START_COUNTDOWN;
AppStates    AppStateMachine=WAIT_FOR_START;

void PlaceEventInQueue(GameEvents event){
  event_queue[event_head]=event;
  if(event_head!=10)
    event_head++;
  else
    event_head=0;
}

GameEvents GetLastEvent(void){
  GameEvents return_event=event_queue[event_tail];
  if(event_tail!=10)
    event_tail++;
  else
    event_tail=0;
  return return_event;
}

void AppStateProcessor(void){
  //get last event from queue
  GameEvents last_event=GetLastEvent();
  switch(AppStateMachine){
    case WAIT_FOR_START:
      //check if event is any
      break;
  }
}
