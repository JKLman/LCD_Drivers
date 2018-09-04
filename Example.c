#define FOSC (7370000ULL)
#define FCY (FOSC/4)
#define TMR2PRESCALE 1

// DSPIC30F3013 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = FRC             // Oscillator (Internal Fast RC)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF             // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection

// FICD
#pragma config ICS = ICS_PGD  //Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libpic30.h>
#include <string.h>



/******************************************************************************
 * PIN DEFINIITIONS
 *****************************************************************************/
#define LCD_RS _LATF4
#define LCD_RW _LATF6
#define LCD_EN _LATF5
#define LCD_DB4 _LATB9
#define LCD_DB5 _LATB8
#define LCD_DB6 _LATB7
#define LCD_DB7 _LATB6

/******************************************************************************
 * TYPEDEFS and ENUMS
 *****************************************************************************/
typedef enum
{
  OUTPUT = 0,
  INPUT = 1
} direction_t;

typedef enum
{
  LOW = 0,
  HIGH = 1
} output_state_t;



/******************************************************************************
 * GLOBAL CONSTANTS
 ******************************************************************************/
uint16_t const GENERIC_TIMEOUT = 200; // 200 * 0.005sec = 1 second


/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/


uint16_t gu16_generic_timer = 0;

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

void LCD_GotoXY(uint8_t const ROW, uint8_t const COLUMN);
void LCD_Init(void);
void LCD_Pulse_Nibble(uint8_t const NIBBLE);
void LCD_Pulse_Char(uint8_t const CHARACTER);
void LCD_Write_String (char const * TEXT);
void LCD_Write_XY (uint8_t const ROW, uint8_t const COLUMN, char const * TEXT);
void PIC_Configure(void);

/*
 * 
 */
int main(void)
{
  PIC_Configure();
  char text[21];
  
  sprintf(text, "Hello World!");
  LCD_Write_XY(1, 0, text);
   
  
  while(1)
  {
    ; //Infinite loop
  }
  return (1);
}


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

void PIC_Configure(void)
{

  _TRISB2 = OUTPUT; // Pin 4:
  _TRISB3 = OUTPUT; // Pin 5: AN3/CN5/RB3 - LCD_RW
  _TRISB4 = OUTPUT; // Pin 6: AN4/CN6/RB4 - LCD_EN
  _TRISB5 = OUTPUT; // Pin 7: AN5/CN7/RB5 - LCD_DB4
  // Pin 8: VSS - connect to VSS (GND)
  // Pin 9: OSC1/CLK1 - not used
  _TRISC15 = OUTPUT; // Pin 10: OSC2/CLKO/RC15 - can not use
  _TRISC13 = OUTPUT; // Pin 11: EMUD1/SOSCI/T2CK/U1ATX/CN1/RC13 - LCD_DB5
  _TRISC14 = OUTPUT; // Pin 12: EMUC1/SOSCO/T1CK/U1ARX/CN0/RC14 - LCD_DB6
  // Pin 13: VDD - connect to VCC (5V)
  _TRISD9 = OUTPUT; // Pin 14: IC2/INT2/RD9 - LCD_DB7
  _TRISD8 = OUTPUT; // Pin 15: EMUC2/IC1/INT1/RD8 - Button_1 with pullup
  _TRISF6 = OUTPUT; // Pin 16: LCD RW
  // Pin 17: PDG/EMUD/U1TX/SDO1/SCL/RF3 - PGD - used by pin 3 of ICD3
  // Pin 18: PGC/EMUC/U1RX/SDI1/SDA/RF2 - PGC - used by pin 2 of ICD3
  // Pin 19: VSS - connect to VSS (GND)
  // Pin 20: VDD - connect to VCC (5V)
  _TRISF5 = OUTPUT; // Pin 21: LCD EN
  LCD_EN = LOW;
  _TRISF4 = OUTPUT; // Pin 22: LCD RS
  _TRISB9 = OUTPUT; // Pin 23: - LCD DB4
  _TRISB8 = OUTPUT; // Pin 24: - LCD DB5
  _TRISB7 = OUTPUT; // Pin 25: - LCD DB6
  _TRISB6 = OUTPUT; // Pin 26: - LCD DB7
  // Pin 27: AVSS - connect to VSS (GND)
  // Pin 28: AVDD - connect to VCC (5V)


  /*LCD pinout
   * Pin 1: GND
   * Pin 2: 5V
   * Pin 3: pot for setting contrast
   * Pin 4: RS
   * Pin 5: RW
   * Pin 6: E
   * Pin 7: DB0 - not used
   * Pin 8: DB1 - not used
   * Pin 9: DB2 - not used
   * Pin 10: DB3 - not used
   * Pin 11: DB4
   * Pin 12: DB5
   * Pin 13: DB6
   * Pin 14: DB7
   * Pin 15: backlight +
   * Pin 16: backlight -
   */
  LCD_Init();
}


