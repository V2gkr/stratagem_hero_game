/*
 * switch.c
 *
 *  Created on: Nov 13, 2025
 *      Author: vovchik
 */

#ifndef SRC_SWITCH_C_
#define SRC_SWITCH_C_

#include "switch.h"

KeyDataStruct keyUpStruct;
KeyDataStruct keyDownStruct;
KeyDataStruct keyRightStruct;
KeyDataStruct keyLeftStruct;

uint8_t GetKeyStates(void){
  uint8_t states=(~GPIOC->IDR)&0xF;
  uint8_t firstDetectedKey=0;
  if(states&KEY_UP_Pin){
    keyUpStruct.press_timer++;
    if(keyUpStruct.press_timer>=PRESS_DETECT_TIME){
      if(keyUpStruct.key_state!=1){
        keyUpStruct.key_state=1;
        firstDetectedKey=KEY_UP_Pin;
      }
    }
  }
  else{
    keyUpStruct.key_state=0;
    keyUpStruct.press_timer=0;
  }

  if(states&KEY_DOWN_Pin){
    keyDownStruct.press_timer++;
    if(keyDownStruct.press_timer>=PRESS_DETECT_TIME){
      if(keyDownStruct.key_state!=1){
        keyDownStruct.key_state=1;
        firstDetectedKey=KEY_DOWN_Pin;
      }
    }
  }
  else{
    keyDownStruct.key_state=0;
    keyDownStruct.press_timer=0;
  }

  if(states&KEY_RIGHT_Pin){
    keyRightStruct.press_timer++;
    if(keyRightStruct.press_timer>=PRESS_DETECT_TIME){
      if(keyRightStruct.key_state!=1){
        keyRightStruct.key_state=1;
        firstDetectedKey=KEY_RIGHT_Pin;
      }
    }
  }
  else{
    keyRightStruct.key_state=0;
    keyRightStruct.press_timer=0;
  }

  if(states&KEY_LEFT_Pin){
    keyLeftStruct.press_timer++;
    if(keyLeftStruct.press_timer>=PRESS_DETECT_TIME){
      if(keyLeftStruct.key_state!=1){
        keyLeftStruct.key_state=1;
        firstDetectedKey=KEY_LEFT_Pin;
      }
    }
  }
  else{
    keyLeftStruct.key_state=0;
    keyLeftStruct.press_timer=0;
  }
  return firstDetectedKey;
}

void DetectSequence(uint8_t result){

}

#endif /* SRC_SWITCH_C_ */
