/*
 * File:   LCD.c
 * Author: mbeddedc.com
 *
 * Created on 14 October, 2017, 5:59 PM
 */


//#include <xc.h>
#include "LCD.h"
#include "config.c"

void WriteCommandToLCD(unsigned char Command)  
{
	while(LCDIsBusy());       // Wait for the LCD to be free

	LCD_RW = 0;               // We are writing to LCD
	LCD_RS = 0;               // It is a command

#ifdef DataBus4Bit
	Write4BitValueToDataBus(Command>>4);  // Write Upper Nibble First
#else                                     // DataBus8Bit
	LCD_Data_Bus = Command;               // Write Command on data bus
#endif

	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

#ifdef DataBus4Bit

	Write4BitValueToDataBus(Command);    //Write Lower Nibble Now

	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

#endif
}

void WriteDataToLCD(char LCDChar)  
{
	while(LCDIsBusy());       // Wait for the LCD to be free

	LCD_RW = 0;               // We are writing to LCD
	LCD_RS = 1;               // It is data

#ifdef DataBus4Bit
	Write4BitValueToDataBus(LCDChar>>4);  // Write Upper Nibble First
#else                                     // DataBus8Bit
	LCD_Data_Bus = LCDChar;               // Write Data on data bus
#endif

	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

#ifdef DataBus4Bit

	Write4BitValueToDataBus(LCDChar);    //Write Lower Nibble Now

	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

#endif
}

void InitLCD(void)
{ 
	// Firstly Make RS, E, RW pins low
	LCD_RS   = 0;
	LCD_RW   = 0;
	LCD_E    = 0;

    // Then make them output, because when pins are made output they
    // become randomly high or low, untill a particular value is written
    // on them. So to avoid that write a value on them before TRIS.
	LCD_RS_Dir = 0;      // Make Output
	LCD_RW_Dir = 0;      // Make Output
	LCD_E_Dir  = 0;      // Make Output

#ifdef DataBus4Bit
   Write4BitValueToDataBus(0x0);    // Write 0 value
   Write4BitValueToDataDirBus(0x0); // Configure Data Bus as output
#else                               // DataBus8Bit
   LCD_Data_Bus     = 0x00;         // Write 0 value
   LCD_Data_Bus_Dir = 0x00;         // Configure Data Bus as output
#endif

  ///////////////// Reset process from datasheet //////////////
  // [ We can't check busy Flag Before the reset process  ] ///
  // [ And In 4 bit mode only upper nibble must be written] ///
  // [ To the LCD during reset process !                  ] ///
  /////////////////////////////////////////////////////////////

	LCD_RW = 0;               // We are writing to LCD
	LCD_RS = 0;               // It is a command

   __delay_ms(40);

#ifdef DataBus4Bit
    Write4BitValueToDataBus(0x3);
#else                         // DataBus8Bit
    LCD_Data_Bus = 0x35;      // Reset Command
#endif
	
   	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

   __delay_ms(6);

#ifdef DataBus4Bit
    Write4BitValueToDataBus(0x3);
#else                         // DataBus8Bit
    LCD_Data_Bus = 0x30;      // Reset Command
#endif
	
   	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

   __delay_us(300);

#ifdef DataBus4Bit
    Write4BitValueToDataBus(0x3);
#else                         // DataBus8Bit
    LCD_Data_Bus = 0x30;      // Reset Command
#endif
	
   	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

   __delay_ms(2);

#ifdef DataBus4Bit

    Write4BitValueToDataBus(0x2);    // Declare 4 bit mode

   	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);      // so that LCD can latch the
	LCD_E = 0;                // data from data bus
	__delay_us(E_Delay); 

#endif

  /////////////// Reset Process End ////////////////
   
#ifdef DataBus4Bit
    #ifdef CharacterFont5x10
	   WriteCommandToLCD(0x2c);    //function set
    #else                          //CharacterFont5x8
	   WriteCommandToLCD(0x28);    //function set
    #endif
#else                              // DataBus8Bit
    #ifdef CharacterFont5x10
	   WriteCommandToLCD(0x3c);    //function set
    #else                          //CharacterFont5x8
	   WriteCommandToLCD(0x38);    //function set
    #endif
#endif

#ifdef CursorOn
  #ifdef CursorBlinkOn
   WriteCommandToLCD(0x0f);    //display on,cursor on,blink on
  #else
   WriteCommandToLCD(0x0e);    //display on,cursor on,blink off
  #endif
#else
   WriteCommandToLCD(0x0c);    //display on,cursor off,blink off
#endif

   WriteCommandToLCD(0x01);    //clear display

#ifdef EntryModeSetDecrement
   WriteCommandToLCD(0x04);    //entry mode, set decrement
#else                          //EntryModeSetIncrement
   WriteCommandToLCD(0x06);    //entry mode, set increment
#endif

#ifdef LCDCustomCharacters
   LCD_Char_build(0, CustomCharacter1);                  // Build Character1 at position 0 
   LCD_Char_build(1, CustomCharacter2);                  // Build Character2 at position 1 
   LCD_Char_build(2, CustomCharacter3);                  // Build Character3 at position 2  
   LCD_Char_build(3, CustomCharacter4);                  // Build Character4 at position 3  
   LCD_Char_build(4, CustomCharacter5);                  // Build Character5 at position 4  
   LCD_Char_build(5, CustomCharacter6);                  // Build Character6 at position 5  
   LCD_Char_build(6, CustomCharacter7);                  // Build Character5 at position 4  
   LCD_Char_build(7, CustomCharacter8);                  // Build Character6 at position 5  
#endif
}

void WriteStringToLCD(const char *s)
{	
	unsigned LCD_DDRAM_Address = 0;

	while(*s)
	{
		LCD_DDRAM_Address = ReadLCDAddressCounter();
		
        #ifdef WrapAround
           #ifdef LCD16x2
		        #ifdef EntryModeSetDecrement
					switch(LCD_DDRAM_Address)
					{
					case 127:    GotoYXPositionOnLCD(LCD_LINE2, 15);   break;   // Wraparound after 16th character
					case 63:     GotoYXPositionOnLCD(LCD_LINE1, 15);   break;   // Wraparound after 16th character
					}
				#else  //EntryModeSetIncrement
					switch(LCD_DDRAM_Address)
					{
					case 16:    GotoYXPositionOnLCD(LCD_LINE2, 0);   break;   // Wraparound after 16th character
					case 80:    GotoYXPositionOnLCD(LCD_LINE1, 0);   break;   // Wraparound after 16th character
					}
				#endif
           #else   // LCD20x2
		        #ifdef EntryModeSetDecrement
					switch(LCD_DDRAM_Address)
					{
					case 127:    GotoYXPositionOnLCD(LCD_LINE2, 19);   break;   // Wraparound after 20th character
					case 63:     GotoYXPositionOnLCD(LCD_LINE1, 19);   break;   // Wraparound after 20th character
					}
				#else    //EntryModeSetIncrement
					switch(LCD_DDRAM_Address)
					{
					case 20:    GotoYXPositionOnLCD(LCD_LINE2, 0);   break;   // Wraparound after 20th character
					case 84:    GotoYXPositionOnLCD(LCD_LINE1, 0);   break;   // Wraparound after 20th character
					}
                #endif
           #endif
        #else    // No Wraparound
	        #ifdef EntryModeSetDecrement
				switch(LCD_DDRAM_Address)
				{
				case 127:    GotoYXPositionOnLCD(LCD_LINE2, 39);    break;   // Wraparound after 20th character
				case 63:     GotoYXPositionOnLCD(LCD_LINE1, 39);    break;   // Wraparound after 20th character
				}
			#else    //EntryModeSetIncrement
				switch(LCD_DDRAM_Address)
				{
				case 40:     GotoYXPositionOnLCD(LCD_LINE2, 0);   break;
				case 104:    GotoYXPositionOnLCD(LCD_LINE1, 0);   break;
				}
			#endif
        #endif
		
		WriteDataToLCD(*s++); 
	}
}


void ClearLCDScreen(void)       // Clear the Screen and return cursor to zero position
{
	WriteCommandToLCD(0x01);    // Clear the screen
	__delay_ms(2);              // Delay for cursor to return at zero position
}



void GotoYXPositionOnLCD(unsigned char LineNumber, unsigned char OFFSET)   // LineNumber can have two values, LCD_LINE1 and LCD_LINE2
{                                                                          // OFFSET can have a value from 0 to 39 --> LCD DDRAM Range
	WriteCommandToLCD(0x80|((LineNumber*0x40)+OFFSET));                    // Line1 is from 0x0 to 0x27 and Line2 is from 0x40 to 0x67
}



void UpdateCharOnLCD(char CharToUpdate, unsigned char LineNumber, unsigned char OFFSET)
{
	GotoYXPositionOnLCD(LineNumber, OFFSET);
	WriteDataToLCD(CharToUpdate);
}


void UpdateStringOnLCD(const char *s, unsigned char LineNumber, unsigned char OFFSET)
{
	GotoYXPositionOnLCD(LineNumber, OFFSET);
	WriteStringToLCD(s);
}


void WriteLCDLine1(const char *s)    // length of s should be 16 for 16x2 LCD and 20 for 20x2 LCD
{
	GotoYXPositionOnLCD(LCD_LINE1, 0);     // Goto 0,0 location
	WriteStringToLCD(s);
}


void WriteLCDLine2(const char *s)    // length of s should be 16 for 16x2 LCD and 20 for 20x2 LCD
{
	GotoYXPositionOnLCD(LCD_LINE2, 0);     // Goto 1,0 location
	WriteStringToLCD(s);
}


unsigned char LCDIsBusy(void)  // Return 1 if LCD is busy, else return 0
{
#ifdef DataBus4Bit
    Write4BitValueToDataDirBus(0xF); // Configure Data Bus as input
#else                                // DataBus8Bit
    LCD_Data_Bus_Dir = 0xFF;         // Configure Data Bus as input
#endif

	LCD_RW = 1;               // We are reading from LCD
	LCD_RS = 0;               // It is a command

	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);           

	if( LCD_Data_D7 )           // if DB7 is set
    {
    	LCD_E = 0;                 
    	__delay_us(E_Delay);

	#ifdef DataBus4Bit
		LCD_E = 1;              // Read The Second nibble
		__delay_us(E_Delay);           

		LCD_E = 0;                
		__delay_us(E_Delay);
	#endif 
		return 1;               // yes busy
    }

	LCD_E = 0;                
	__delay_us(E_Delay);

#ifdef DataBus4Bit

	LCD_E = 1;              // Read The Second nibble
	__delay_us(E_Delay);           

	LCD_E = 0;                
	__delay_us(E_Delay);

	// Restore Data Bus direction
    Write4BitValueToDataDirBus(0x0); // Configure Data Bus as output
#else                                // DataBus8Bit
	// Restore Data Bus direction
    LCD_Data_Bus_Dir = 0x00;         // Configure Data Bus as output
#endif

	// Restore RW
	LCD_RW = 0;  

	return 0;                  // not busy
}




#ifdef DataBus4Bit
void Write4BitValueToDataBus(unsigned char Data)
{
	// Write 4bit of Data to DataBus
	LCD_Data_D4 = (((Data>>0)&0x1)!=0);
	LCD_Data_D5 = (((Data>>1)&0x1)!=0);
	LCD_Data_D6 = (((Data>>2)&0x1)!=0);
	LCD_Data_D7 = (((Data>>3)&0x1)!=0);
}
#endif


#ifdef DataBus4Bit
void Write4BitValueToDataDirBus(unsigned char DataDir)
{
	// Write 4bit of DataDir to DataBusDir
	LCD_Data_Dir_D4 = (((DataDir>>0)&0x1)!=0);
	LCD_Data_Dir_D5 = (((DataDir>>1)&0x1)!=0);
	LCD_Data_Dir_D6 = (((DataDir>>2)&0x1)!=0);
	LCD_Data_Dir_D7 = (((DataDir>>3)&0x1)!=0);
}
#endif



// ~~~~~~~~~~~~~~~~~~~~~~  LCD Custom Characters  ~~~~~~~~~~~~~~~~~~~~~~ //
/* --------------------------------------------------------------------- */
/* Tutorial available at ...                                             */
/* http://www.8051projects.net/lcd-interfacing/lcd-custom-character.php  */
/*-----------------------------------------------------------------------*/

/*  -=Description=-
  CGRAM adress command starts from 0x40, where the address of CGRAM (Acg) 
  starts from 0x00. CGRAM has a total of 64 Bytes. When you are using LCD
  as 5x8 dots in function set then you can define a total of 8 user defined
  patterns (1 Byte for each row and 8 rows for each pattern), where as when
  LCD is working in 5x10 dots, you can define 4 user defined patterns. */

#ifdef LCDCustomCharacters
// ********************************************************* //
//Function = LCD_Char_build
//Input:
//     location: location where you want to store  0,1,2,....7          
//     ptr: Pointer to pattern data
//Usage:
//     LCD_Char_build(1,pattern);
// ********************************************************* //

void LCD_Char_build(unsigned char location, unsigned char *ptr)
{
     unsigned char i;                          //For indexing

     if(location<8)                            //If valid address
	 {
         WriteCommandToLCD(0x40+(location*8)); //Write to CGRAM
         for(i=0;i<8;i++)
            WriteDataToLCD(ptr[i]);            //Write the character pattern to CGRAM
     }

	 WriteCommandToLCD(0x80);                  //shift back to DDRAM location 0
}
#endif





unsigned char ReadLCDAddressCounter(void)  // Return 7 bit Address Counter value, with MSB zero.
{
	unsigned char AddressCounter = 0;

#ifdef DataBus4Bit
    Write4BitValueToDataDirBus(0xF); // Configure Data Bus as input
#else                                // DataBus8Bit
    LCD_Data_Bus_Dir = 0xFF;         // Configure Data Bus as input
#endif

	LCD_RW = 1;               // We are reading from LCD
	LCD_RS = 0;               // It is a command

	LCD_E = 1;                // Give a pulse on E pin
	__delay_us(E_Delay);           

#ifdef DataBus4Bit   // Read Upper Nibble
	AddressCounter = (LCD_Data_D7 << 7) | (LCD_Data_D6 << 6) | (LCD_Data_D5 << 5) | (LCD_Data_D4 << 4);
#else                // DataBus8Bit
    AddressCounter = LCD_Data_Bus;
#endif

	LCD_E = 0;                
	__delay_us(E_Delay);

#ifdef DataBus4Bit

	LCD_E = 1;              // Read Lower Nibble
	__delay_us(E_Delay);           

	AddressCounter |= ( (LCD_Data_D7 << 3) | (LCD_Data_D6 << 2) | (LCD_Data_D5 << 1) | (LCD_Data_D4 << 0) );

	LCD_E = 0;                
	__delay_us(E_Delay);

	// Restore Data Bus direction
    Write4BitValueToDataDirBus(0x0); // Configure Data Bus as output
#else                         // DataBus8Bit
	// Restore Data Bus direction
    LCD_Data_Bus_Dir = 0x00;  // Configure Data Bus as output
#endif

	AddressCounter &= 0x7F;   // Make MSB zero, which is BF, we only need AC

	// Restore RW
	LCD_RW = 0;  

	return AddressCounter;                
}




void ScrollLCD(unsigned char ScreenOrCursor, unsigned char RightOrLeft, unsigned char ByHowMuch)     // Scroll LCD Screen
{
	for(unsigned char i=0; i<ByHowMuch; i++)
		WriteCommandToLCD(0x10|(ScreenOrCursor<<3)|(RightOrLeft<<2));
}
