/*
 * switch.h
 *
 *  Created on: Nov 13, 2025
 *      Author: vovchik
 */

#ifndef INC_SWITCH_H_
#define INC_SWITCH_H_

#include "main.h"

#define KEY_ALL   (KEY_UP_Pin|KEY_LEFT_Pin|KEY_RIGHT_Pin|KEY_DOWN_Pin)
#define KEY_POS_STEP  4
#define PRESS_DETECT_TIME   20

typedef struct{
  uint8_t press_timer;
  uint8_t key_state;
}KeyDataStruct;

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

uint8_t GetKeyStates(void);

#endif /* INC_SWITCH_H_ */
