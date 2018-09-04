void LCD_GotoXY(uint8_t const ROW, uint8_t const COLUMN)
{
  uint8_t address;
  switch (ROW)
  {
    case 1: address = 0x00; break;
    case 2: address = 0x40; break;
    case 3: address = 0x14; break;
    case 4: address = 0x54; break;
    default: address = 0x00; break;
  }
  LCD_RS = LOW;
  LCD_RW = LOW;
  LCD_Pulse_Char(0x80 | (address + COLUMN));
  
}

void LCD_Init(void)
{
  __delay_ms(100);
  LCD_RS = LOW;
  LCD_Pulse_Nibble(0x03); // wake up
  __delay_us(80);
  LCD_Pulse_Nibble(0x03); // no really, wake up
  __delay_us(80);
  LCD_Pulse_Nibble(0x03); // I mean it, WAKE UP!
  __delay_us(80);
  LCD_Pulse_Nibble(0x02); // 4-bit interface
  __delay_us(80);
  LCD_Pulse_Char(0x28); // 4bit, 2line, 5x8 dots
  LCD_Pulse_Char(0x04); 
  LCD_Pulse_Char(0x85);
  LCD_Pulse_Char(0x06);
  LCD_Pulse_Char(0x02);
  __delay_ms(3);
  LCD_Pulse_Char(0x0C);
  LCD_Pulse_Char(0x01); // clear display
  __delay_ms(6);
  
}

void LCD_Pulse_Char(uint8_t const CHARACTER)
{
  LCD_Pulse_Nibble((CHARACTER & 0xF0) >> 4);
  LCD_Pulse_Nibble(CHARACTER & 0x0F);
  __delay_us(50);
}

void LCD_Pulse_Nibble(uint8_t const NIBBLE)
{
  LCD_DB7 = (NIBBLE & 0x08) > 0;
  LCD_DB6 = (NIBBLE & 0x04) > 0;
  LCD_DB5 = (NIBBLE & 0x02) > 0;
  LCD_DB4 = (NIBBLE & 0x01) > 0;
  LCD_EN = 1;
  __delay_us(50);
  LCD_EN = 0;
  __delay_us(50);
}

void LCD_Write_String (char const * TEXT)
{
  uint16_t loop = 0;
  LCD_RS = HIGH;
  LCD_RW = LOW;

  for (loop = 0; TEXT[loop] != '\0'; loop++)
  {
    LCD_Pulse_Char(TEXT[loop]);
  }
}
  
void LCD_Write_XY (uint8_t const ROW, uint8_t const COLUMN, char const * TEXT)
{
  LCD_GotoXY(ROW, COLUMN);
  LCD_Write_String(TEXT);
}