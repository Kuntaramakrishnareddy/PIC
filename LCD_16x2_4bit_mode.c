#include<p18f4520.h>
#include "LCD.h"


void LCD_String(char *data)
{
    LCD_send_cmd ( CLEAR_DISPLAY );
    LCD_send_cmd ( RETURN_HOME );
    LCD_send_cmd ( DISP_ON_CURSOR_OFF );  
    LCD_send_cmd ( CURSOR_1ST_LINE );
    LCD_send_cmd ( INC_CURSOR );  
	while(*data!='\0')
    {
       LCD_send_char(*data);
       data++;               
    }
}
void LCD_Initialization( void ) 
 {
		LCD_delay (); /* wait enough time after Vdd rise */
		RS =0;
		RW =0;
		BK =1;
		LCD_send_cmd ( RETURN_HOME );
		LCD_send_cmd ( FUNCTION_SET ); 
		LCD_send_cmd ( DISP_ON_CURSOR_OFF ); 
		LCD_send_cmd ( CLEAR_DISPLAY ); 
		LCD_send_cmd ( INC_CURSOR ); 
		LCD_send_cmd ( CURSOR_1ST_LINE ); 
 }
void LCD_SetPosition ( unsigned char position )
 {
 /* this subroutine works specifically for 4-bit Port A */
	LCD_send_cmd(position | CURSOR_1ST_LINE);
 }
void LCD_send_cmd ( unsigned char data )
 {
 /* this subroutine works specifically for 4-bit Port A */
 Nibble_MSB ( data ); /* send high nibble */
 LCD_Enable_pulse();
 Nibble_LSB ( data ); /* send low nibble */
 LCD_Enable_pulse();
 }
void LCD_Enable_pulse ( void )
 {
	EN = 1;
	LCD_delay(); 
    EN =0;
    LCD_delay(); 
 }
void LCD_delay(void)
{
  unsigned int i;
	for(i = 1; i < 100; i++);
}

void Nibble_MSB (unsigned char data)
{
    LCD &= 0xF0;
	LCD |= (data >>4) & 0x0F;
}

void Nibble_LSB(unsigned char data)
{
    LCD &= 0xF0;
	LCD |= data & 0x0F;
}
void LCD_send_char ( unsigned char data )
 {
	/* this subroutine works specifically for 4-bit */
 RS =1;
 Nibble_MSB ( data ); /* send high nibble */
 LCD_Enable_pulse();
 Nibble_LSB ( data ); /* send low nibble */
 LCD_Enable_pulse();
 RS =0;

 }