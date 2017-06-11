#include<p18f4520.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include"usart.h"
#include"LCD.h"

#pragma config OSC = HS
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = OFF
#pragma config PBADEN = OFF
#pragma config STVREN = OFF
#pragma config LVP = OFF

#define OUTPUT_PORT 0x00
#define INPUT_PORT  0xFF
#define FAN PORTDbits.RD5

unsigned char String[]="Temperature:    ";

unsigned int ADCRead(unsigned char );
void print_bits(int );
void Delay_ms(unsigned char );
void Serial_Init(void);
int Dig;

void main()
{
        unsigned char *p, temp[10], i;
		TRISD = OUTPUT_PORT;
        TRISB = OUTPUT_PORT;
        TRISC = OUTPUT_PORT;
		ADCON1 = 0x8E;
		ADCON2 = 0x8A;
		Serial_Init();
		putrsUSART(" \n\r Temperature monitoring  \n\r ");
        LCD_Initialization();
		while(1)
        {       
            while(String[i]!='\0')
            {
               LCD_send_char(String[i]);
               i++;               
            }
		    Dig = ADCRead(0);
			sprintf(temp,"%d", Dig);
		    putsUSART(temp);
			String[12]=temp[0];
			String[13]=temp[1];
			String[14]=temp[2];
            if(Dig>25)
                FAN = 1;
            else
                FAN = 0;
			putrsUSART(" \n\r");
            LCD_SetPosition(12);
			Delay_ms(100);						
			//LCD_send_cmd(CLEAR_DISPLAY);            
            i = 12;			
		}
}

unsigned int ADCRead(unsigned char ch)
{
   if(ch>13) return 0;  	//Invalid Channel
   ADCON0=0x00;
   ADCON0=(ch<<2);   		//Select ADC Channel
   ADON=1;  				//switch on the adc module
   GODONE=1;  				//Start conversion
   while(GODONE); 			//wait for the conversion to finish
   ADON=0;  				//switch off adc
   return ADRES;
}

void Serial_Init(void)
{
      SPBRG = 25;                        // 19200 baud @ 20MHz
      TXSTA = 0x26;                    // setup USART transmit
      RCSTA = 0x90;                    // setup USART receive
}

void Delay_ms(unsigned char time)
{
        unsigned char i,j;
        for(i=0;i<time;i++)
       {
               for(j=0;j<200;j++);
       }
} 
