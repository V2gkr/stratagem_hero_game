/*
 * display.h
 *
 *  Created on: Nov 13, 2025
 *      Author: vovchik
 */
#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


#define ARROW_UP    0x00  // custom symbol code arrow up
#define ARROW_DOWN  0x01  // custom symbol code arrow down
#define ARROW_LEFT  0x7F  // prefedined symbol code arrow left
#define ARROW_RIGHT 0x7E  // predefined symbol code arrow right


#define DISPLAY_ROWS          4
#define DISPLAY_COLS          20
#define MAX_STRATAGEM_SIZE    8 /* max is hellbomb up down left up down right up down*/
#define STRATAGEM_COL_START   ((DISPLAY_COLS-MAX_STRATAGEM_SIZE)/2)

/* custom stuff for 20x4 lcd display used in this project*/
void DisplayConfigCustomChars(void);


void ClearStratagemOnDisplay(void);


void DisplayNextSequenceArrow(uint8_t arrow,uint8_t number);


void DisplayWaitForStartScreen(void);

void DisplayActiveGameScreen(uint32_t sequence);

void DisplayStartCountDownScreen(uint8_t time_s);

void DisplayAfterRoundInfo(uint8_t time_left,uint16_t points);

void DisplayFinalRoundInfo(uint16_t points);
#endif /* INC_DISPLAY_H_*/

