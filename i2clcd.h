#ifndef LCD_H
#define LCD_H
//--------------------------------------------------------------
//#include <xc.h>
#include "RTC_1302.h"
//--------------------------------------------------------------
#define _XTAL_FREQ 20000000
#define LCD_BACKLIGHT          0x08
#define LCD_NOBACKLIGHT        0x00
#define LCD_FIRST_ROW          0x80
#define LCD_SECOND_ROW         0xC0
#define LCD_THIRD_ROW          0x94
#define LCD_FOURTH_ROW         0xD4
#define LCD_CLEAR              0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E
#define LCD_TYPE               2

//void LCD_PORT_init();
void LCD_Init(unsigned char I2C_Addr);
void IO_Expander_Write(unsigned char Data);
void LCD_Write_4Bit(unsigned char Nibble);
void LCD_CMD(unsigned char CMD);
void LCD_Write_Char(char Data);
void LCD_Write_String(char* Str);
void LCD_Set_Cursor(unsigned char ROW, unsigned char COL);
inline void LCD_Backlight(void);
inline void LCD_noBacklight(void);
/*void LCD_SL(void);
void LCD_SR(void);
void LCD_Clear(void);*/
void LCD_Clear_Row(unsigned char ROW);
//--------------------------------------------------------------
#endif /* LCD_H */