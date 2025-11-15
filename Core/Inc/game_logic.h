/*
 * game_logic.h
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

typedef enum{
  WAIT_FOR_START=0,
  GAME_IS_ACTIVE=1,
  GAME_ENDED=2
}AppStates;

typedef enum{
  START_COUNTDOWN=0,
  ACTIVE_ROUND=1,
  ROUND_COMPLETE=2
}GameSubState;

typedef enum{
  NO_EVENT=0,
  ANY_BUTTON_PRESSED=1,
  CTDOWN_TIMEOUT=2,
  GAME_TIMEOUT=3,
  IDLE_TIMEOUT=4,
}GameEvents;

typedef struct{
  GameEvent timeout_type;
  uint8_t is_active;
  uint32_t start_timestamp;
  const uint32_t timespan;
}TimeoutDataStruct;


void PlaceEventInQueue(GameEvents event);

GameEvents GetLastEvent(void);

void AppStateProcessor(void);

void GameProcessor(GameEvents last_event);

void StartTimeout(GameEvents TimeoutType);

void TimeoutProcessor(void);

#endif /* INC_GAME_LOGIC_H_ */
