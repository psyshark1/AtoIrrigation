#ifndef MAIN_H
#define	MAIN_H
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 20000000

#include <stdio.h>
#include "i2clcd.h"

char dat[4]={'\0'};
unsigned char tmp = 0;
unsigned char sec = 0;
unsigned char min = 0;
unsigned char hrs = 0;
unsigned char pump_check_t = 20;//20 sec
unsigned char irrigation_t = 15;//15 min
unsigned char wait_t = 23;//23 hrs
unsigned short states = 0;
/* states bit15 - irrigation cycle pause time state
 * states bit14 - pump state
 * states bit13 - light state
 * states bit12 - pump control water state
 * states bit11 - error pump control water state
 * states bit10 - soil control state
 * states bit9 - start pump enable emergency control time state
 * states bit8 - calculate start pump emergency disable control time state
 * states bit7 - start control irrigation time state
 * states bit6 - calculate control irrigation time state
 * states bit5 - calculate cycle irrigation time state
 * states bit0-4 - buttons bits*/

void CheckButtons(void);
inline void ReadTemPress(void);
void SetLight(void);
void SetWater(void);
void SetSoil(void);
void ConvertTime(unsigned char clock);
void PrintContrtolPanel(void);
void PrintError(void);
#endif	/* MAIN_H */

