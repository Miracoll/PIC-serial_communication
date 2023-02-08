/* 
 * File:   LCD.h
 * Author: mbeddedc.com
 *
 * Created on 14 October, 2017, 5:57 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

// Define CPU Frequency
// This must be defined, if __delay_ms() or 
// __delay_us() functions are used in the code
//#define _XTAL_FREQ 8000000  
    
// ------ Selectables ------ //
#define DataBus4Bit
//#define DataBus8Bit
//#define CursorOn
//#define CursorBlinkOn
#define EntryModeSetIncrement
//#define EntryModeSetDecrement
#define LCD16x2
//#define LCD20x2
//#define WrapAround
#define CharacterFont5x8
//#define CharacterFont5x10
#define LCDCustomCharacters
// ------------------------- //


// Define Control Pins used for LCD
#define LCD_RS            RB0
#define LCD_RW            RB1
#define LCD_E             RB2

// Define Control Pins directions used for LCD
#define LCD_RS_Dir        TRISB0
#define LCD_RW_Dir        TRISB1
#define LCD_E_Dir         TRISB2

#ifdef DataBus4Bit
	#define LCD_Data_D7       RB7
	#define LCD_Data_D6       RB6
	#define LCD_Data_D5       RB5
	#define LCD_Data_D4       RB4
	#define LCD_Data_Dir_D7   TRISB7
	#define LCD_Data_Dir_D6   TRISB6
	#define LCD_Data_Dir_D5   TRISB5
	#define LCD_Data_Dir_D4   TRISB4
#else //DataBus8Bit
	#define LCD_Data_Bus      PORTB
	#define LCD_Data_Bus_Dir  TRISB
    #define LCD_Data_D7       LCD_Data_Bus&0x80
#endif

// Constants
#define E_Delay       1   // micro seconds
#define LCD_LINE1     0
#define LCD_LINE2     1

// Scroll Command parameters
#define ScrollScreen  1
#define ScrollCursor  0
#define ToRight       1
#define ToLeft        0

// Link to design new custom characters->  http://www.8051projects.net/lcd-interfacing/lcd-custom-character.php 

// LCD Custom Characters
#ifdef LCDCustomCharacters   // Font should be 5x8 for 8 custom characters
							 // For 5x10 font only 4 custom characters can be made
	unsigned char CustomCharacter1[8] = { 0b00000,0b01010,0b00000,0b00100,0b00000,0b01010,0b00100,0b00000};//Small smiley
	unsigned char CustomCharacter2[8] = { 0b10111,0b10100,0b10100,0b11111,0b00101,0b00101,0b00101,0b11101};//custom 1
	unsigned char CustomCharacter3[8] = { 0b00111,0b00100,0b00100,0b00100,0b00100,0b00100,0b00100,0b11100};//custom 2
	unsigned char CustomCharacter4[8] = { 0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};//all
	unsigned char CustomCharacter5[8] = { 0b00000,0b01010,0b00000,0b00100,0b00000,0b00100,0b01010,0b00000};//sad smiley
	unsigned char CustomCharacter6[8] = { 0b00100,0b01110,0b11111,0b01110,0b01110,0b01110,0b11111,0b00100};//youtube bell
	unsigned char CustomCharacter7[8] = { 0b00000,0b11011,0b11011,0b00100,0b00100,0b10001,0b01110,0b01110};//smiley
	unsigned char CustomCharacter8[8] = { 0b00011,0b00011,0b00011,0b11111,0b01111,0b11111,0b01111,0b11111};//like
#endif

// Function Declarations
void WriteCommandToLCD(unsigned char);
void WriteDataToLCD(char);
void InitLCD(void);
void WriteStringToLCD(const char*);
void ClearLCDScreen(void);
unsigned char LCDIsBusy(void);
unsigned char ReadLCDAddressCounter(void);
void GotoYXPositionOnLCD(unsigned char,unsigned char);
void UpdateCharOnLCD(char,unsigned char,unsigned char);
void UpdateStringOnLCD(const char*,unsigned char,unsigned char);
void WriteLCDLine1(const char*);
void WriteLCDLine2(const char*);
void ScrollLCD(unsigned char,unsigned char,unsigned char);

#ifdef LCDCustomCharacters
void LCD_Char_build(unsigned char,unsigned char*); 
#endif

#ifdef DataBus4Bit
void Write4BitValueToDataBus(unsigned char);
void Write4BitValueToDataDirBus(unsigned char);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

