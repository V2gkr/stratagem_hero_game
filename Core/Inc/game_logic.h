/*
 * game_logic.h
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

#include "qpc.h"
#include "stdint.h"

enum GameEventSignals{
  ANY_BUTTON_PRESSED=Q_USER_SIG,
  CNTDOWN_TIMEOUT,
  GAME_TIMEOUT,
  IDLE_TIMEOUT,
  SW_BLOCK_TIMEOUT,
  WRONG_SEQUENCE
};

typedef union{
  uint32_t sequence;
  struct{
    uint32_t key1:4;
    uint32_t key2:4;
    uint32_t key3:4;
    uint32_t key4:4;
    uint32_t key5:4;
    uint32_t key6:4;
    uint32_t key7:4;
    uint32_t key8:4;
  }halfbyte_element;
}fourBitSequenceStruct;

typedef struct{
  QActive super;
  uint8_t sequence_array_cursor;
  uint8_t sequence_cursor;
  uint8_t last_pressed_button;
  uint8_t user_score;
  uint8_t display_full_stratagem_flag;
  uint8_t sw_unlock_flag;
  uint8_t round_num;
  uint8_t countdown_timer;
  fourBitSequenceStruct input;
  fourBitSequenceStruct stratagems[15]; //not to say that thats vulnerable , but who tf can go to 10 round and 15 stratagems
  //const char * stratagem_names[5];
  QTimeEvt countdown_timeout;
  QTimeEvt game_timeout;
  QTimeEvt idle_timeout;
  QTimeEvt sw_block_timeout;
}GameDataStruct;

uint8_t ParseKeysToLcdArrows(uint8_t result);

uint8_t ParseInvertedKeysToLcdArrows(uint8_t result);

void UpdateLastPressedKey(uint8_t last_key);

void Game_ctor(GameDataStruct * const me);
#endif /* INC_GAME_LOGIC_H_ */
