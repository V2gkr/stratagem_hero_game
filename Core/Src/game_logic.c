/*
 * game_logic.c
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#include "game_logic.h"
#include "stdint.h"

#define TIMEOUTS_COUNT    3

GameEvents event_queue[10]={0};
uint8_t event_tail=0;
uint8_t event_head=0;

GameSubState GameSubStateMachine=START_COUNTDOWN;
AppStates    AppStateMachine=WAIT_FOR_START;

TimeoutDataStruct CtdownTimeout={.timeout_type=CTDOWN_TIMEOUT,.timespan=3000,.is_active=0};
TimeoutDataStruct GameTimeout={.timeout_type=GAME_TIMEOUT,.timespan=10000,.is_active=0};
TimeoutDataStruct IdleTimeout={.timeout_type=IDLE_TIMEOUT,timespan=20000,.is_active=0};
TimeoutDataStruct * Timeouts[]={&CtdownTimeout,&GameTimeout,&IdleTimeout,.is_active=0};


void PlaceEventInQueue(GameEvents event){
  if(event_tail!=event_head){
    event_queue[event_head]=event;
    if(event_head!=10)
      event_head++;
    else
      event_head=0;
  }
}

GameEvents GetLastEvent(void){
  if(event_tail!=event_head){
    GameEvents return_event=event_queue[event_tail];
    if(event_tail!=10)
      event_tail++;
    else
      event_tail=0;
    return return_event;
  }
  else
    return NO_EVENT;
}

void AppStateProcessor(void){
  //get last event from queue
  GameEvents last_event=GetLastEvent();
  switch(AppStateMachine){
    case WAIT_FOR_START:
      //check if event is any
      //display smth (press any key to start)
      if(last_event==ANY_BUTTON_PRESSED){
        //place event GAME_IS_ACTIVE
        AppStateMachine=GAME_IS_ACTIVE;
        StartTimeout(CTDOWN_TIMEOUT);

      }
      break;
    case GAME_IS_ACTIVE:
      GameProcessor(last_event);
      // GameProcessor and
      break;
    case GAME_ENDED:
      if(last_event==IDLE_TIMEOUT){
        AppStateMachine=WAIT_FOR_START;
      }
      break;
  }
}

void GameProcessor(GameEvents last_event){
  switch(GameSubStateMachine){
    case START_COUNTDOWN:
      if(last_event==CTDOWN_TIMEOUT){
        GameSubStateMachine=ACTIVE_ROUND;
        StartTimeout(GAME_TIMEOUT);
      }
      //place event 3..2..1 and display it
      break;
    case ACTIVE_ROUND:
      //checking sequence and sheet
      //check for GAME_TIMEOUT
//      if(all_rounds_ok){
//        GameSubStateMachine=ROUND_COMPLETE;
//        StartTimeout(IDLE_TIMEOUT);
//      }
      if(last_event==GAME_TIMEOUT){
        StartTimeout(IDLE_TIMEOUT);
        GameSubStateMachine=START_COUNTDOWN;
        AppStateMachine=GAME_ENDED;
      }
      break;
    case ROUND_COMPLETE:
      if(last_event==IDLE_TIMEOUT){
        GameSubStateMachine=START_COUNTDOWN;
        AppStateMachine=WAIT_FOR_START;
      }
      if(last_event==ANY_BUTTON_PRESSED){
        StartTimeout(CTDOWN_TIMEOUT);
        GameSubStateMachine=START_COUNTDOWN;
      }
      //wait for button - start countdown , start idle
      break;
  }

}


void StartTimeout(GameEvents TimeoutType){
  TimeoutDataStruct * timeout_local=NULL;
  uint8_t i=0;
  while(i<TIMEOUTS_COUNT){
    if(Timeouts[i]->timeout_type==TimeoutType){
      timeout_local=Timeouts[i];
      break;
    }
    else
      i++;
  }
  if(timeout_local==NULL)
    return;
  timeout_local->start_timestamp=HAL_GetTick();
  timeout_local->stop_timestamp+=timeout_local->start_timestamp;
  timeout_local->is_active=1;
}

void TimeoutProcessor(void){
  for(uint8_t i=0;i<TIMEOUTS_COUNT;i++){
    if((HAL_GetTick()-Timeouts[i]->start_timestamp)>Timeouts[i]->timespan){
      PlaceEventInQueue(Timeouts[i]->timeout_type);
      Timeouts[i]->is_active=0;
    }
  }
}

