#include<p18f4520.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <xc.h> 
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

#define _XTAL_FREQ 20000000
#define OUTPUT_PORT 0x00
#define INPUT_PORT  0xFF
#define DQ_DIR TRISDbits.RD6
#define FAN PORTDbits.RD5
#define DQ PORTDbits.RD6
#define LED PORTBbits.RB1

unsigned char Init_DS18B20(void);
void WriteOneChar(unsigned char dat);
unsigned char ReadOneChar(void);
void  ReadTemperature(void);
unsigned int ADCRead(unsigned char );
void Serial_Init(void);
void Delay_ms(unsigned int time);

unsigned char readdata[2];

int main(void)
{
    static unsigned char String[]="Temperature:    ";
    static unsigned char String1[]="Sensor Connected";
    static unsigned char String2[]="Sensor Failed";

    unsigned char temp[10];
    unsigned int TEMPERATURE =0;   
    
		TRISD = OUTPUT_PORT;
        TRISB = INPUT_PORT & 0xF0 ;
        TRISC = OUTPUT_PORT;
		ADCON1 = 0x8E;
		ADCON2 = 0x8A;
        FAN = 0;
        Init_DS18B20();        
		Serial_Init();
        putrsUSART("Temperature monitoring \n\r ");
        LCD_Initialization();
        if(Init_DS18B20())
        {            	
            putrsUSART(" Sensor connection failed \n\r ");
            LCD_String(&String2);
            Delay_ms(2000);	
        }
        else
        {            	
            putrsUSART(" Sensor connected \n\r ");
            LCD_String(&String1);
            Delay_ms(2000);	
        }        
		while(1)
        {   
            if(Init_DS18B20())
            {
                LCD_String(&String2);
                Delay_ms(2000);		
                putrsUSART(" Sensor connection failed \n\r ");
                FAN = 0;
            }
            else
            {
                LCD_String(&String);                 
                ReadTemperature();
                TEMPERATURE = 0;
                TEMPERATURE |=readdata[0];
                TEMPERATURE |=(readdata[1] << 8);
                TEMPERATURE = TEMPERATURE*0.0625;
                sprintf(temp,"%d", TEMPERATURE);
                putsUSART(temp);
                putrsUSART("\n\r ");
                String[12]=temp[0];
                String[13]=temp[1];
                String[14]=temp[2];
                if(TEMPERATURE > 45)
                    FAN = 1;
                else
                    FAN = 0;                  
                Delay_ms(1000);	                
            }  
        }
        return 0;
}

void Delay_ms(unsigned int time)
{
    unsigned int i;
    for(i = 0; i < time; i++)
    {
        __delay_ms(1);
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
      SPBRG = 129;                        // 9600 baud @ 4MHz
      TXSTA = 0x26;                    // setup USART transmit
      RCSTA = 0x90;                    // setup USART receive
}

unsigned char Init_DS18B20(void)
{
    unsigned char status = 1; 
    DQ_DIR = 0;	
    DQ = 0;
    __delay_us(500);
    DQ = 1;
    DQ_DIR = 1;
    __delay_us(100);
    status = DQ;
    __delay_us(400);
    
    return status;    
}


unsigned char ReadOneChar(void)
{
    unsigned char i=0;
    unsigned char dat = 0;
    
	DQ_DIR = 1;
    //putrsUSART("Read:");
    for (i=0;i < 8; i++)
    {
	  DQ_DIR = 0;
      DQ = 0; // To the pulse signal
	  __delay_us(4);
	  DQ_DIR = 1;
	  __delay_us(10);
	  if(DQ != 0 )
	  {
		  dat |= 1 << i;
          //putrsUSART("1");
	  }
      else
      {
          //putrsUSART("0");
      }
	  __delay_us(44);
        
    }
    //putrsUSART("\n\r");
    return(dat);
}

void WriteOneChar(unsigned char dat)
{
    unsigned char i;
    DQ_DIR = 1;      
    for (i=0; i < 8; i++)
    {
		if((dat>>i)&0x01)
		{  
			DQ_DIR = 0;
			DQ =0;
			__delay_us(15);
			DQ_DIR = 1;
			__delay_us(45);
           
		}
		else
		{
			DQ_DIR = 0;
			DQ = 0;
			__delay_us(60); 
			DQ_DIR = 1;
            
		}
		__delay_us(2);       
    }
    
}
 
//Read temperature
void  ReadTemperature(void)
{
    Init_DS18B20();
    WriteOneChar(0xCC); // Skip read serial number column number of operations
    WriteOneChar(0x44); // Start temperature conversion
	Init_DS18B20();
    WriteOneChar(0xCC); //Skip read serial number column number of operations
    WriteOneChar(0xBE); //Read the temperature register, etc. (a total of 9 registers readable) is the temperature of the first two
    readdata[0]=ReadOneChar();
    readdata[1]=ReadOneChar();
   
}
