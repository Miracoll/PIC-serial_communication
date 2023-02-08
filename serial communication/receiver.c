/* 
 * File:   receiver.c
 * Author: Obiano Miracol
 *
 * Created on April 7, 2020, 5:15 PM
 */

//#include <pic16f877a.h>
#include "config.c"
#include "LCD.h"

char receiveChar(void);
//void interrupt changevalue(void);

/*
 * 
 */
void main(void) {
    
    SPBRG = 51;
    BRGH = 1;
    SYNC = 0;
    SPEN = 1;
    CREN = 1;
    RX9 = 0;
    
    TRISCbits.TRISC7 = 1;
    //TRISCbits.TRISC6 = 0;
    char receiveData;
    //char sendData;
    //char LEDStatus;
    
    TRISDbits.TRISD0 = 0;
    PORTDbits.RD0 = 0;
    
    InitLCD();
    LCDIsBusy();
    WriteCommandToLCD(0x0c);
    
    while(1){
        receiveData = receiveChar();
        LCDIsBusy();
        if(receiveData == '1'){
            ClearLCDScreen();
            RD0 = 1;
            WriteLCDLine1("LED TURNED ON");
        }
        if(receiveData == '2'){
            ClearLCDScreen();
            RD0 = 0;
            WriteLCDLine1("LED TURNED OFF");
        }
    }
}

char receiveChar(void)
{
    while(!RCIF);
    char ch = RCREG;
    if(OERR){
        CREN = 0;
        CREN = 1;
    }
    return ch;
}