#include "i2clcd.h"
//--------------------------------------------------------------
unsigned char RS, i2c_add, BackLight_State = LCD_BACKLIGHT;

void LCD_Init(unsigned char I2C_Add) 
{
  i2c_add = I2C_Add;
  IO_Expander_Write(0x00);
  __delay_ms(30);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_CMD(0x20 | (LCD_TYPE << 2));
  __delay_ms(10);
  LCD_CMD(LCD_TURN_ON);
  __delay_ms(10);
  LCD_CMD(LCD_CLEAR);
  __delay_ms(10);
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(10);
  //LCD_CMD(LCD_BLINK_CURSOR_ON);
}

void IO_Expander_Write(unsigned char Data) 
{
  I2C_StartCondition();
  I2C_Write_Byte(i2c_add);
  I2C_Write_Byte(Data | BackLight_State);
  I2C_StopCondition();
}

void LCD_Write_4Bit(unsigned char Nibble) 
{
  // Get The RS Value To LSB OF Data  
  Nibble |= RS;
  IO_Expander_Write(Nibble | 0x04);
  IO_Expander_Write(Nibble & 0xFB);
  __delay_us(5);
}

void LCD_CMD(unsigned char CMD) 
{
  RS = 0; // Command Register Select
  LCD_Write_4Bit(CMD & 0xF0);
  LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Write_Char(char Data)
{
  RS = 1;  // Data Register Select
  LCD_Write_4Bit(Data & 0xF0);
  LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Write_String(char* Str)
{
    for(unsigned char i=0; Str[i]!='\0'; ++i)
    {
       LCD_Write_Char(Str[i]); 
    }
}

void LCD_Set_Cursor(unsigned char ROW, unsigned char COL) 
{    
  switch(ROW) 
  {
    case 2:
      LCD_CMD(0xC0 + COL-1);
      break;
    case 3:
      LCD_CMD(0x94 + COL-1);
      break;
    case 4:
      LCD_CMD(0xD4 + COL-1);
      break;
    // Case 1  
    default:
      LCD_CMD(0x80 + COL-1);
  }
}

inline void LCD_Backlight() 
{
  BackLight_State = LCD_BACKLIGHT;
  IO_Expander_Write(0);
}

inline void LCD_noBacklight() 
{
  BackLight_State = LCD_NOBACKLIGHT;
  IO_Expander_Write(0);
}

/*void LCD_SL()
{
  LCD_CMD(0x18);
  __delay_us(40);
}

void LCD_SR()
{
  LCD_CMD(0x1C);
  __delay_us(40);
}

void LCD_Clear()
{
  LCD_CMD(LCD_CLEAR); 
  __delay_us(40);
}*/
void LCD_Clear_Row(unsigned char ROW)
{
    LCD_Set_Cursor(ROW,1);
    for(unsigned char i=1; i<21; ++i)
    {
       LCD_Write_Char(0x20); 
    }
}