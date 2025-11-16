/*
 * display.c
 *
 *  Created on: Nov 13, 2025
 *      Author: vovchik
 */

#include "lcd.h"
#include "stdio.h"
#include "display.h"
#include "game_logic.h"

uint8_t arrow_high[]={0x00,0x04,0xe0,0x15,0x4,0x4,0x4,0x4};
uint8_t arrow_left[]={0x00,0x4,0x4,0x4,0x4,0x15,0xe0,0x04};

uint8_t arrows[]={0x00,0x04,0x0e,0x15,0x04,0x04,0x00,0x00
                  ,0x00,0x04,0x04,0x15,0x0e,0x04,0x00,0x00};


void DisplayConfigCustomChars(void){
  lcd_set_custom_symbol(arrows,2);
}

void ClearStratagemOnDisplay(void){
  lcd_update_buffer("        ", MAX_STRATAGEM_SIZE, 20+STRATAGEM_COL_START);
}

void DisplayNextSequenceArrow(uint8_t arrow,uint8_t number){
  lcd_update_buffer(&arrow,1,20+STRATAGEM_COL_START+number);
}

void DisplayWaitForStartScreen(void){
  lcd_clear_buffer();
  lcd_update_buffer("  Press any button  ",20,0);
  lcd_update_buffer("  to start the game ",20,20);
}

void DisplayStartCountDownScreen(uint8_t time_s){
  lcd_clear_buffer();
  lcd_update_buffer("  Starting game in  ",20,0);
  uint8_t buf[9];
  sprintf(buf,"%d sec ",time_s);
  lcd_update_buffer(buf,6,26);
}

void DisplayActiveGameScreen(uint32_t sequence){
  //lcd_clear_buffer();
  lcd_update_buffer("Game started: ",14,0);
  uint8_t act_sequence[8];
  for(uint8_t i=0;i<=32;i++){
    act_sequence[i]=ParseKeysToLcdArrows((sequence>>(i*4))&0xF);
  }
  lcd_update_buffer(&act_sequence,8,40+6);
}

void DisplayAfterRoundInfo(uint8_t time_left,uint16_t points){
  lcd_clear_buffer();
  lcd_update_buffer("   Round Complete   ",20,0);
  uint8_t buf[20];
 // sprintf(buf,"  your score: %d",points);
  //lcd_update_buffer(buf,20,20);
  lcd_update_buffer("  Press any button  ",20,40);
  lcd_update_buffer("     to continue    ",20,60);
}

void DisplayFinalRoundInfo(uint16_t points){
  lcd_clear_buffer();
  lcd_update_buffer("    Game Complete   ",20,0);
  uint8_t buf[20];
  //sprintf(buf,"  your score: %d     ",points);
  //lcd_update_buffer(buf,20,20);
  lcd_update_buffer("  Press any button  ",20,40);
  lcd_update_buffer("     to restart     ",20,60);
}
