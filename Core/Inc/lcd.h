#ifndef __LCD_H
#define __LCD_H

#include "stm32g4xx_hal.h"


extern char LCD_80buf[80];//buffer to store data to be displayed

/** @brief: initialization of lcd to work with i2c module*/
void lcd_init (void);

/** @brief: clears whole screen */
void lcd_clear (void);

/** @brief: updates a buffer with our data */
void lcd_update_buffer(uint8_t * data , uint8_t size,uint8_t offset);

/** @brief: sends full data to the lcd to update whole screen */
void lcd_update_screen(void);

void lcd_set_custom_symbol(uint8_t *buf,uint8_t symbol_count);
#endif
