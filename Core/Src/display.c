/*
 * display.c
 *
 *  Created on: Nov 13, 2025
 *      Author: vovchik
 */

#include "lcd.h"
#include "display.h"

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

void WriteNextSequenceArrow(uint8_t arrow,uint8_t number){
  lcd_update_buffer(&arrow,1,20+STRATAGEM_COL_START+number);
}

void DisplayWaitForStartScreen(void){
  lcd_clear_buffer();
  lcd_update_buffer("  Press any button  ",20,0);
  lcd_update_buffer("  to start the game ",20,20);
}
