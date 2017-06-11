#ifndef LCD_H
#define LCD_H

/*
#define LCD_D0 PORTDbits.RD0
#define LCD_D1 PORTDbits.RD1
#define LCD_D2 PORTDbits.RD2
#define LCD_D3 PORTDbits.RD3
*/
#define LCD PORTD
/* LCD Controlling pins */
#define RS PORTBbits.RB3
#define RW PORTBbits.RB2
#define EN PORTDbits.RD4
#define BK PORTCbits.RC2

/* Command set for Hitachi 44780U LCD display controller  */
#define CLEAR_DISPLAY 			0x01  //Clear display screen
#define RETURN_HOME 			0x02  //Return home
#define DEC_CURSOR 				0x04  //Decrement cursor (shift cursor to left)
#define INC_CURSOR 				0x06  //Increment cursor (shift cursor to right)
#define SH_DIS_R   				0x05  //Shift display right
#define SH_DISP_L  				0x07  //Shift display left
#define DISP_CURSOR_OFF  		0x08  //Display OFF, cursor OFF
#define DISP_ON_CURSOR_OFF      0x0C  //Display ON, cursor OFF
#define DIS_ON_CUR_BLNK 		0x0E  //Display ON, cursor blinking
#define LCD_CURSOR_ON 			0x0F  //LCD ON, cursor ON
#define LCD_CURSOR_BACK     	0x10
#define LCD_CURSOR_FWD      	0x14
#define LCD_PAN_LEFT        	0x18
#define LCD_PAN_RIGHT       	0x1C
#define FUNCTION_SET        	0x28  // 4 bit interface, 2 lines, 5x8 font
#define TWO_LINES_5x7			0x38  //2 lines and 5×7 matrix
#define ACTIVATE_LINE2			0x3C  //Activate second line
#define CURSOR_1ST_LINE 		0x80  //Force cursor to beginning of first line
#define CURSOR_2ND_LINE 		0xC0  //Force cursor to beginning of second line
#define LINE2_POS1				0xC1  //Jump to second line, position 1



void LCD_Initialization ( void );
void LCD_SetPosition ( unsigned char c );
void LCD_send_cmd ( unsigned char c );
void LCD_Enable_pulse ( void );
void LCD_delay(void);
void Nibble_MSB (unsigned char c);
void Nibble_LSB(unsigned char c);
void LCD_send_char ( unsigned char c );

#endif