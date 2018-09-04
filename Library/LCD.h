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