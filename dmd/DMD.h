
/*--------------------------------------------------------------------------------------
 Setup and instantiation of DMD library
 Note this currently uses the SPI port for the fastest performance to the DMD, be
 careful of possible conflicts with other SPI port devices
--------------------------------------------------------------------------------------*/
//#warning CHANGE THESE TO SEMI-ADJUSTABLE PIN DEFS!
//Arduino pins used for the display connection
//#define PIN_DMD_nOE       PORTB.0    // D9 active low Output Enable, setting this low lights all the LEDs in the selected rows. Can pwm it at very high frequency for brightness control.
//#define PIN_DMD_A         PORTB.2    // D6
//#define PIN_DMD_B         PORTB.1    // D7
//#define PIN_DMD_CLK       PORTB.7   // D13_SCK  is SPI Clock if SPI is used
//#define PIN_DMD_SCLK      PORTB.3    // D8
//#define PIN_DMD_R_DATA    PORTB.5   // D11_MOSI is SPI Master Out if SPI is used
//Define this chip select pin that the Ethernet W5100 IC or other SPI device uses
//if it is in use during a DMD scan request then scanDisplayBySPI() will exit without conflict! (and skip that scan)
//#define PIN_OTHER_SPI_nCS PORTB.4
#ifndef __DMD_H
#define __DMD_H

/* Includes ------------------------------------------------------------------*/
//#include "definisi.h"
#include "OSAL.h"
#include "OSAL_Memory.h"
#include "log.h"
#include "gpio.h"
#include "pwm.h"
#include "clock.h"
#include "timer.h"
#include "definisi.h"
#include "Systemfont5x7.h"

//static unsigned int brt[16] = {3000,5000,8000,10000,11000,13000,15000,17000,19000,20000,
//                                21000,23000,25000,27000,30000,40000};
#ifndef UINT8
typedef unsigned char           u8;
#endif

#define fs_jws   		0x0002
#define fs_lt_bjr  		0x0003
#define fs_iqomah	  	0x0004
#define fs_bios  		0x0005
#define fs_text00  		0x0006
#define fs_text01  		0x0007
#define fs_text02  		0x0008
#define fs_text03  		0x0009
#define fs_text04  		0x000A
#define fs_text05  		0x000B
#define fs_text06  		0x000C
#define fs_text07  		0x000D
#define fs_text08  		0x000E
#define fs_text09  		0x000F
#define fs_panel  		0x0010

static unsigned char bPixelLookupTable[8] =
{
   0x80,   //0, bit 7
   0x40,   //1, bit 6
   0x20,   //2. bit 5
   0x10,   //3, bit 4
   0x08,   //4, bit 3
   0x04,   //5, bit 2
   0x02,   //6, bit 1
   0x01    //7, bit 0
};

#define LOW                     0
#define HIGH                    1
#define PIN_DMD_A_LOW           AP_GPIO->swporta_dr &= ~BIT(P2); //GPIOA_ResetBits(GPIO_Pin_15)
#define PIN_DMD_A_HIGH          AP_GPIO->swporta_dr |= BIT(P2); //GPIOA_SetBits(GPIO_Pin_15)
#define PIN_DMD_B_LOW  			AP_GPIO->swporta_dr &= ~BIT(P3); //GPIOA_ResetBits(GPIO_Pin_14)
#define PIN_DMD_B_HIGH 			AP_GPIO->swporta_dr |= BIT(P3); //GPIOA_SetBits(GPIO_Pin_14)
#define PIN_DMD_SCLK_LOW  		AP_GPIO->swporta_dr &= ~BIT(P11); //GPIOA_ResetBits(GPIO_Pin_3)
#define PIN_DMD_SCLK_HIGH 		AP_GPIO->swporta_dr |= BIT(P11); //GPIOA_SetBits(GPIO_Pin_3)

//DMD I/O pin macros
#define LIGHT_DMD_ROW_01_05_09_13       { PIN_DMD_B_LOW ; PIN_DMD_A_LOW ; }
#define LIGHT_DMD_ROW_02_06_10_14       { PIN_DMD_B_LOW ; PIN_DMD_A_HIGH ; }
#define LIGHT_DMD_ROW_03_07_11_15       { PIN_DMD_B_HIGH ; PIN_DMD_A_LOW ; }
#define LIGHT_DMD_ROW_04_08_12_16       { PIN_DMD_B_HIGH ; PIN_DMD_A_HIGH ; }
#define LATCH_DMD_SHIFT_REG_TO_OUTPUT   { PIN_DMD_SCLK_HIGH ; PIN_DMD_SCLK_LOW ; }
//#define OE_DMD_ROWS_OFF                 { PIN_DMD_nOE_LOW  ; }
//#define OE_DMD_ROWS_ON                  { PIN_DMD_nOE_HIGH ; }

//Pixel/graphics writing modes (bGraphicsMode)
#define GRAPHICS_NORMAL    0
#define GRAPHICS_INVERSE   1
#define GRAPHICS_TOGGLE    2
#define GRAPHICS_OR        3
#define GRAPHICS_NOR       4

//drawTestPattern Patterns
#define PATTERN_ALT_0     0
#define PATTERN_ALT_1     1
#define PATTERN_STRIPE_0  2
#define PATTERN_STRIPE_1  3

//display screen (and subscreen) sizing
#define DMD_PIXELS_ACROSS         32      //pixels across x axis (base 2 size expected)
#define DMD_PIXELS_DOWN           16      //pixels down y axis
#define DMD_BITSPERPIXEL           1      //1 bit per pixel, use more bits to allow for pwm screen brightness control
#define DMD_RAM_SIZE_BYTES        ((DMD_PIXELS_ACROSS*DMD_BITSPERPIXEL/8)*DMD_PIXELS_DOWN)
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
#define DISPLAYS_BPP 1
// Font Indices
#define FONT_LENGTH             0
#define FONT_FIXED_WIDTH        2
#define FONT_HEIGHT             3
#define FONT_FIRST_CHAR         4
#define FONT_CHAR_COUNT         5
#define FONT_WIDTH_TABLE        6

//#define tr
//unsigned int ui;

void clearScreen(unsigned char bNormal);
void DMD(unsigned char panelsWide, unsigned char panelsHigh);
void selectFont(const uint8 *font);
void writePixel ( unsigned int bX , unsigned int bY , unsigned char bGraphicsMode , unsigned char bPixel );
int charWidth(const char letter) ;
int drawChar( int bX, int bY, char letter, unsigned char bGraphicsMode);
void drawString(int bX, int bY,  char *bChars, unsigned char length, unsigned char bGraphicsMode);
void drawMarquee(const char *bChars, unsigned char length, unsigned char top);
_Bool stepMarquee(unsigned char amount);
_Bool stepMarquees(int amountX, int amountY);
_Bool stepMarqueePanel(int amountX,int amountY );
//void drawCircleSub(int cx, int cy, int x, int y, unsigned char bGraphicsMode);
//void drawCircle(int xCenter, int yCenter, int radius, unsigned char bGraphicsMode);
//void drawBox(int x1, int y1, int x2, int y2, unsigned char bGraphicsMode);
//void DMD::drawTestPattern(byte bPattern);
void spi(unsigned char data);
void scanDisplayBySPI(void);

#endif



