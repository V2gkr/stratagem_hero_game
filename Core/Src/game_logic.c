/*
 * game_logic.c
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#include "game_logic.h"
#include "display.h"
#include "stddef.h"
#include "stdint.h"
#include "lcd.h"
#include "main.h"
#define TIMEOUTS_COUNT      3
#define STRATAGEM_PER_GAME  5

typedef struct{
  uint8_t sequence_array_cursor;
  uint8_t sequence_cursor;
  uint8_t last_pressed_button;
  fourBitSequenceStruct input;
  fourBitSequenceStruct stratagems[5];
}GameDataStruct;

GameEvents event_queue[10]={0};
uint8_t event_tail=0;
uint8_t event_head=0;

GameSubState GameSubStateMachine=START_COUNTDOWN;
AppStates    AppStateMachine=WAIT_FOR_START;
GameDataStruct GameData;


TimeoutDataStruct CtdownTimeout={.timeout_type=CTDOWN_TIMEOUT,.timespan=3000,.is_active=0};
TimeoutDataStruct GameTimeout={.timeout_type=GAME_TIMEOUT,.timespan=10000,.is_active=0};
TimeoutDataStruct IdleTimeout={.timeout_type=IDLE_TIMEOUT,.timespan=20000,.is_active=0};
TimeoutDataStruct * Timeouts[]={&CtdownTimeout,&GameTimeout,&IdleTimeout};

fourBitSequenceStruct REVIVE_STRATAGEM={.sequence=0x14281};//up 1 / down 8 / right 2 / left 4 / up 1
fourBitSequenceStruct RESSUPLY_STRATAGEM={.sequence=0x2188};
fourBitSequenceStruct LASER_CANNON_STRATAGEM={.sequence=0x41848};
fourBitSequenceStruct ORBITAL_LASER_STRATAGEM={.sequence=0x82182};//right down up right down
fourBitSequenceStruct HELLBOMB_STRATAGEM={.sequence=0x81281481};// up down left up down right up down

void PlaceEventInQueue(GameEvents event){
//  if(event_tail!=event_head){
    event_queue[event_head]=event;
    if(event_head!=10)
      event_head++;
    else
      event_head=0;
//  }
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
      DisplayWaitForStartScreen();
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
      if(last_event==ANY_BUTTON_PRESSED){
        StartTimeout(CTDOWN_TIMEOUT);
        GameSubStateMachine=START_COUNTDOWN;
        AppStateMachine=GAME_IS_ACTIVE;
      }
      break;
  }
}

void GameProcessor(GameEvents last_event){
  switch(GameSubStateMachine){
    case START_COUNTDOWN:
      DisplayStartCountDownScreen(1+(((Timeouts[0]->timespan+Timeouts[0]->start_timestamp-HAL_GetTick()))/1000));
      if(last_event==CTDOWN_TIMEOUT){
        GameSubStateMachine=ACTIVE_ROUND;
        StartTimeout(GAME_TIMEOUT);
        GameData.sequence_cursor=0;
        GameData.sequence_array_cursor=0;
        //prepare stratagem list
        GameData.stratagems[0].sequence=REVIVE_STRATAGEM.sequence;
        GameData.stratagems[1].sequence=ORBITAL_LASER_STRATAGEM.sequence;
        GameData.stratagems[2].sequence=HELLBOMB_STRATAGEM.sequence;
        GameData.stratagems[3].sequence=LASER_CANNON_STRATAGEM.sequence;
        GameData.stratagems[4].sequence=RESSUPLY_STRATAGEM.sequence;
        lcd_clear_buffer();
      }
      //place event 3..2..1 and display it
      break;
    case ACTIVE_ROUND:
      DisplayActiveGameScreen(GameData.stratagems[GameData.sequence_array_cursor].sequence);
      if(last_event==ANY_BUTTON_PRESSED){
        //uint8_t result=0;//0x1,0x2,0x4,0x8 only
        if(GameData.last_pressed_button==((GameData.stratagems[GameData.sequence_array_cursor].sequence>>GameData.sequence_cursor)&0xF)){
          //update stratagem
          uint8_t new_arrow=ParseKeysToLcdArrows(GameData.last_pressed_button);
          DisplayNextSequenceArrow(new_arrow,GameData.sequence_cursor/4);
          GameData.input.sequence=new_arrow<<GameData.sequence_cursor;
          GameData.sequence_cursor+=4;
          if(((GameData.stratagems[GameData.sequence_array_cursor].sequence>>GameData.sequence_cursor)&0xF)==0x00){
            //single stratagem is complete
            GameData.sequence_array_cursor++;   //increment cursor for next stratagem
            GameData.sequence_cursor=0;         //clear cursor of each arrow in sequence
            GameData.input.sequence=0;          //clear previous sequence
            ClearStratagemOnDisplay();
            if(GameData.sequence_array_cursor==STRATAGEM_PER_GAME){
              GameSubStateMachine=ROUND_COMPLETE; //change state
              DisplayAfterRoundInfo(0,0);            //update lcd
              StartTimeout(IDLE_TIMEOUT);         //
            }
          }
        }
        else{
          //error in input
          GameData.input.sequence=0;
          GameData.sequence_cursor=0;
          ClearStratagemOnDisplay();
          //clear displayed stratagem
        }
      }
      if(last_event==GAME_TIMEOUT){
        StartTimeout(IDLE_TIMEOUT);
        DisplayFinalRoundInfo(0);
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
  //timeout_local->stop_timestamp+=timeout_local->start_timestamp;
  timeout_local->is_active=1;
}

void TimeoutProcessor(void){
  for(uint8_t i=0;i<TIMEOUTS_COUNT;i++){
    if((HAL_GetTick()-Timeouts[i]->start_timestamp)>Timeouts[i]->timespan && Timeouts[i]->is_active){
      PlaceEventInQueue(Timeouts[i]->timeout_type);
      Timeouts[i]->is_active=0;
    }
  }
}

uint8_t ParseKeysToLcdArrows(uint8_t result){
  switch(result){
    case KEY_UP_Pin:
      return ARROW_UP;
    case KEY_LEFT_Pin:
      return ARROW_LEFT;
    case KEY_DOWN_Pin:
      return ARROW_DOWN;
    case KEY_RIGHT_Pin:
      return ARROW_RIGHT;
    default:
      return ' ';
  }
}

void UpdateLastPressedKey(uint8_t last_key){
  GameData.last_pressed_button=last_key;
}
