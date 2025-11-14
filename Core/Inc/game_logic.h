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
  ACTIVE_ROUNG=1,
  ROUND_COMPLETE=2
}GameSubState;

typedef enum{
  ANY_BUTTON_PRESSED=0,
  CTDOWN_TIMEOUT=1,
  GAME_TIMEOUT=2,
  IDLE_TIMEOUT=3,
}GameEvents;

#endif /* INC_GAME_LOGIC_H_ */
