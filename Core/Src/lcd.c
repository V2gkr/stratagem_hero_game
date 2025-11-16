
/** Put this in the src folder **/

#include "lcd.h"
#include "string.h"

/** @brief: handler to HAL i2c structure*/
extern I2C_HandleTypeDef hi2c1;

#define SLAVE_ADDRESS_LCD 0x4E 

/** @brief: buffer to store all data to display*/
char LCD_80buf[80]={0};

/** 
	* @brief: static functions
*/

/** @brief: sends command to an lcd*/
void lcd_send_cmd (char cmd);

/** @brief: sends data to an lcd*/
void lcd_send_data (char data);

/** @brief: sends full string on one line of lcd */
void lcd_send_string (char *str,uint8_t size);  

/** @brief: moving a cursor to the start of a specific line */
void lcd_set_line(uint8_t line);

/** @brief: initialization of lcd to work with i2c module*/
void lcd_init (void){
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(10);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(20);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(20);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(5);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(5);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(5);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(5);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	lcd_clear_buffer();
}

/** @brief: sends command to an lcd*/
void lcd_send_cmd (char cmd){
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

/** @brief: sends data to an lcd*/
void lcd_send_data (char data){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

/** @brief: moving a cursor to the start of a specific line */
void lcd_set_line(uint8_t line){
	uint8_t frame=0x80;
	switch(line){
		case 0:
			frame|=0x00;
			break;
		case 1:
			frame|=0x40;
			break;
		case 2:
			frame|=0x14;
			break;
		case 3:
			frame|=0x54;
			break;
	}
	lcd_send_cmd(frame);
}

/** @brief: clears whole screen */
void lcd_clear(void){
	lcd_send_cmd (0x01);
}

/** @brief: sends full string on one line of lcd */
void lcd_send_string (char *str,uint8_t size){
	while (size--) lcd_send_data (*str++);
}

/** @brief: updates a buffer with our data */
void lcd_update_buffer(uint8_t * data , uint8_t size,uint8_t offset){
	memcpy(LCD_80buf+offset,data,size);
}

void lcd_clear_buffer(void){
  memset(LCD_80buf,' ',80);
}

/** @brief: sends full data to the lcd to update whole screen */
void lcd_update_screen(void){
	lcd_set_line(0);
	lcd_send_string(LCD_80buf,20);
	lcd_set_line(1);
	lcd_send_string(LCD_80buf+20,20);
	lcd_set_line(2);
	lcd_send_string(LCD_80buf+40,20);
	lcd_set_line(3);
	lcd_send_string(LCD_80buf+60,20);
}

void lcd_set_custom_symbol(uint8_t *buf,uint8_t symbol_count){
  lcd_send_cmd(0x40);
  for(uint8_t i=0;i<symbol_count*8;i++)
    lcd_send_data(buf[i]);
}
