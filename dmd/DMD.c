/*--------------------------------------------------------------------------------------
 Setup and instantiation of DMD library
 Note this currently uses the SPI port for the fastest performance to the DMD, be
 careful of possible conflicts with other SPI port devices
--------------------------------------------------------------------------------------*/
#include "DMD.h"
#include "eltop.h"

extern SET set;

uint8_t DisplaysWide;
uint8_t DisplaysHigh;
uint8_t DisplaysTotal;
int row1, row2, row3;
volatile unsigned char bDMDByte;
//uint8_t RAMI[2000];
char *bDMDScreenRAM;
char *bDMDScreenGAM;//*bufgreen;
char *bDMDScreenBAM;//*bufblue;
const uint8_t *Font;
//char marqueeText[1512];
//# define len_mt 512
char *marqueeText;
uint8_t marqueeLength ;
uint8_t marqueeTop;
int marqueeWidth;
int marqueeHeight;
int marqueeOffsetX;
int marqueeOffsetY;
int marqueeOffset;
uint8_t britnes;
uint8_t polaris,oe;

uint32_t test;
/*--------------------------------------------------------------------------------------
 Clear the screen in DMD RAM
--------------------------------------------------------------------------------------*/
void clearScreen(unsigned char bNormal){
	  unsigned char b;
    if (bNormal)
	    b = 0xFF;		// clear all pixels
    else
	    b = 0x00;		// set all pixels
    //initialise the DMD RAM 
    //memset(bDMDScreenRAM,b,DMD_RAM_SIZE_BYTES*DisplaysTotal);
		osal_memset(bDMDScreenRAM,b,DMD_RAM_SIZE_BYTES*DisplaysTotal);
	//osal_memset(bDMDScreenGAM,b,DMD_RAM_SIZE_BYTES*DisplaysTotal);
	//osal_memset(bDMDScreenBAM,b,DMD_RAM_SIZE_BYTES*DisplaysTotal);
    //if (bNormal) // clear all pixels
   //     memset(bDMDScreenRAM,0xFF,DMD_RAM_SIZE_BYTES*DisplaysTotal);
    //else // set all pixels
    //    memset(bDMDScreenRAM,0x00,DMD_RAM_SIZE_BYTES*DisplaysTotal);
}

void DMD(unsigned char panelsWide, unsigned char panelsHigh)
{
   
    DisplaysWide=panelsWide;
    DisplaysHigh=panelsHigh;
    DisplaysTotal=DisplaysWide*DisplaysHigh;
    row1 = DisplaysTotal<<4;
    row2 = DisplaysTotal<<5;
    row3 = ((DisplaysTotal<<2)*3)<<2;
    //bDMDScreenRAM =  (char *) malloc(DisplaysTotal*DMD_RAM_SIZE_BYTES);
  	bDMDScreenRAM =  (char *)osal_mem_alloc( DisplaysTotal*DMD_RAM_SIZE_BYTES);
	//bDMDScreenGAM =  (char *)osal_mem_alloc( DisplaysTotal*DMD_RAM_SIZE_BYTES);
	//bDMDScreenBAM =  (char *)osal_mem_alloc( DisplaysTotal*DMD_RAM_SIZE_BYTES);
	marqueeText =  (char *)osal_mem_alloc(256);
    //bDMDScreenRAM = ( char *)RAMI;
    PIN_DMD_A_LOW;	// 
    PIN_DMD_B_LOW;	// 
    //PIN_DMD_CLK= LOW;	// 
    PIN_DMD_SCLK_LOW;	// 
    
    bDMDByte = 0;
}

void selectFont(const uint8 *font)
{ //unsigned char flash *hr;
     Font = font;
}

/*--------------------------------------------------------------------------------------
 Set or clear a pixel at the x and y location (0,0 is the top left corner)
-------------------------------------------------------------------------------------*/
void writePixel ( unsigned int bX , unsigned int bY , unsigned char bGraphicsMode , unsigned char bPixel )  {
    unsigned int uiDMDRAMPointer;
    unsigned char panel,lookup ;

    if ( (bX >= (DMD_PIXELS_ACROSS*DisplaysWide)) || (bY >= (DMD_PIXELS_DOWN * DisplaysHigh)) ) { return; }   
    
    panel=(bX/DMD_PIXELS_ACROSS) + (DisplaysWide*(bY/DMD_PIXELS_DOWN));
    bX=(bX % DMD_PIXELS_ACROSS) + (panel<<5);
    bY=(bY % DMD_PIXELS_DOWN); // + ((bY/DMD_PIXELS_DOWN)*16);
    //set pointer to DMD RAM byte to be modified
    uiDMDRAMPointer = bX/8 + bY*(DisplaysTotal<<2);

    lookup = bPixelLookupTable[bX & 0x07];

    switch (bGraphicsMode) {
    case GRAPHICS_NORMAL:
	    if (bPixel) bDMDScreenRAM[uiDMDRAMPointer] &= ~lookup;	// zero bit is pixel on
	        else bDMDScreenRAM[uiDMDRAMPointer] |= lookup;	// one bit is pixel off
	    break;
    case GRAPHICS_INVERSE:
	    if (bPixel == 0) bDMDScreenRAM[uiDMDRAMPointer] &= ~lookup;	// zero bit is pixel on
	    else bDMDScreenRAM[uiDMDRAMPointer] |= lookup;	// one bit is pixel off
	    break;
    
    } ;

}

/*--------------------------------------------------------------------------------------
 Draw or clear a line from x1,y1 to x2,y2
--------------------------------------------------------------------------------------*/
void drawLine(int x1, int y1, int x2, int y2, unsigned char bGraphicsMode)
{
    int dy = y2 - y1;
    int dx = x2 - x1;
    int stepx, stepy;

    if (dy < 0) {
              dy = -dy;
              stepy = -1;
    } else {
              stepy = 1;
    }
    if (dx < 0) {
              dx = -dx;
              stepx = -1;
    } else {
              stepx = 1;
    }
    dy <<= 1;                              // dy is now 2*dy
    dx <<= 1;                              // dx is now 2*dx

    writePixel(x1, y1, bGraphicsMode,1);
    if (dx > dy) {
              int fraction = dy - (dx >> 1);          // same as 2*dy - dx
              while (x1 != x2) {
                  if (fraction >= 0) {
                            y1 += stepy;
                            fraction -= dx;          // same as fraction -= 2*dx
                  }
                  x1 += stepx;
                  fraction += dy;          // same as fraction -= 2*dy
                  writePixel(x1, y1, bGraphicsMode, 1);
              }
    } else {
              int fraction = dx - (dy >> 1);
              while (y1 != y2) {
                  if (fraction >= 0) {
                            x1 += stepx;
                            fraction -= dy;
                  }
                  y1 += stepy;
                  fraction += dx;
                  writePixel(x1, y1, bGraphicsMode, 1);
              }
    }
}

int charWidth(const char letter)  {
  unsigned char width,firstChar,charCount;  //unsigned int index;
  char c;
     c = letter;
    // Space is often not included in font so use width of 'n'
    if (c == ' ') c = 'n';
     width = 0;

     firstChar = *(Font + FONT_FIRST_CHAR);
     charCount = *(Font + FONT_CHAR_COUNT);

     //index = 0;

    if ( (c < firstChar) || (c >= (firstChar + charCount)) ) {
              return 0;
    }
    c -= firstChar;

    if ( (*(Font + FONT_LENGTH) == 0 ) && (*(Font + FONT_LENGTH + 1) == 0) ) {
              // zero length is flag indicating fixed width font (array does not contain width data entries)
              width =*(Font + FONT_FIXED_WIDTH);
    } else {
              // variable width font, read width data
              width = *(Font + FONT_WIDTH_TABLE + c);
    };
    return width;
}

void drawFilledBox (int x1, int y1, int x2, int y2, unsigned char bGraphicsMode)
{
int b;
    for (b = x1; b <= x2; b++) {
              drawLine (b, y1, b, y2, bGraphicsMode);
    };
}

int drawChar( int bX, int bY, char letter, unsigned char bGraphicsMode)
{
   char c ;  
   unsigned char height,width,bytes,firstChar,charCount;  
   int charWide,offset; 
   unsigned char i, j,k,data; 
   unsigned int index;
    // writePixel (5,5,GRAPHICS_NORMAL,1);  return 0;
    if ( (bX > (DMD_PIXELS_ACROSS*DisplaysWide)) || (bY > (DMD_PIXELS_DOWN*DisplaysHigh) ) ) return -1;
     c = letter;  
     height =*(Font + FONT_HEIGHT); 
	 // if(height==0x03) { writePixel (5,5,GRAPHICS_NORMAL,1);  return 0; };
    if (c == ' ') { charWide = charWidth(' ');
		drawFilledBox(bX, bY, bX + charWide, bY + height, GRAPHICS_INVERSE); return charWide;
	};
     width = 0;
     bytes = (height + 7) / 8;

    firstChar = *(Font + FONT_FIRST_CHAR);
    charCount = *(Font + FONT_CHAR_COUNT);

    index = 0;

    if ((c < firstChar) || (c >= (firstChar + charCount))) return 0;
    c -= firstChar;

    if ( (*(Font + FONT_LENGTH) == 0) && (*(Font + FONT_LENGTH + 1) == 0) ) {
              // zero length is flag indicating fixed width font (array does not contain width data entries)
              width = *(Font + FONT_FIXED_WIDTH);
              index = c * bytes * width + FONT_WIDTH_TABLE;
    } else {
              // variable width font, read width data, to get the index
              for (i = 0; i < c; i++) { index += *(Font + FONT_WIDTH_TABLE + i); };
              index = index * bytes + charCount + FONT_WIDTH_TABLE;
              width = *(Font + FONT_WIDTH_TABLE + c);
    }
   if ( (bX <-width) || (bY <-height) ) return width;
    //writePixel (5,5,GRAPHICS_NORMAL,1);  return 0; 
    // last but not least, draw the character
    for (j = 0; j < width; j++) { // Width
              for ( i = bytes - 1; i < 254; i--) { // Vertical Bytes
                   data = *(Font + index + j + (i * width));
                   offset = (i * 8);
                   if ( (i == bytes - 1) && (bytes > 1) ) { offset = height - 8; };
                        
                  for ( k = 0; k < 8; k++) { // Vertical bits
                            if ( (offset+k >= (i*8)) && (offset+k <= height)) {
                                if (data & (1 << k)) {
                                          writePixel(bX + j, bY + offset + k, bGraphicsMode, 1);
                                } else {
                                          writePixel(bX + j, bY + offset + k, bGraphicsMode, 0);
                                };
                            };
                  };
              };
    };
    return width;
}



void drawString(int bX, int bY,  char *bChars, unsigned char length, unsigned char bGraphicsMode)
{
unsigned char height;
int charWide,strWidth;
unsigned int i;

    if ( (bX >= (DMD_PIXELS_ACROSS*DisplaysWide)) || (bY >= (DMD_PIXELS_DOWN * DisplaysHigh)) )  return;
    height = *(Font + FONT_HEIGHT);
    if (bY+height<0) return;

    strWidth = 0;
	drawLine(bX -1 , bY, bX -1 , bY + height, GRAPHICS_INVERSE);

    for (i = 0; i < length; i++) {
         charWide = drawChar(bX+strWidth, bY, bChars[i], bGraphicsMode);  //charWide =5;
         if (charWide > 0) {
             strWidth += charWide ;
             drawLine(bX + strWidth , bY, bX + strWidth , bY + height, GRAPHICS_INVERSE);
             strWidth++;  }
         else if (charWide < 0) { return; };
        
        if ( ((bX + strWidth) >= DMD_PIXELS_ACROSS * DisplaysWide) || (bY >= (DMD_PIXELS_DOWN * DisplaysHigh))  ) return;
    };
}

void drawMarquee(const char *bChars, unsigned char length, unsigned char top)
{
	  int i;
    marqueeWidth = 0;
    for ( i = 0; i < length; i++) {
	marqueeText[i] = bChars[i];
	marqueeWidth += charWidth(bChars[i]) + 1;
    }
		//LOG("\nMtext == %s MTlen=%d with=%d \n",marqueeText, osal_strlen(marqueeText),marqueeWidth); 
    marqueeHeight=*(Font + FONT_HEIGHT); 
    marqueeText[length] = '\0';
    marqueeOffsetY = top;
    marqueeOffsetX = DMD_PIXELS_ACROSS * DisplaysWide ;
    marqueeLength = length;
    drawString(marqueeOffsetX, marqueeOffsetY, marqueeText, marqueeLength,
     GRAPHICS_NORMAL);
}

_Bool stepMarquee(unsigned char amount)
{
    _Bool ret=0;
    marqueeOffsetX -= amount;
    if (marqueeOffsetX < -(marqueeWidth)) {
      marqueeOffsetX = DMD_PIXELS_ACROSS * DisplaysWide ;//64;//(DMD_PIXELS_ACROSS * 2);
      //clearScreen(1);
        ret=1;
    }
    drawString(marqueeOffsetX, marqueeOffsetY, marqueeText, marqueeLength,GRAPHICS_NORMAL);
    return ret;;
}

_Bool stepMarquees(int amountX, int amountY)
{
	int i;
	int strWidth;
	//u8 colom;
    _Bool ret=0;
    marqueeOffsetX += amountX;
    marqueeOffsetY += amountY;
	  //colom = marqueeOffsetY;
    if (marqueeOffsetX < -marqueeWidth) {
      marqueeOffsetX = DMD_PIXELS_ACROSS * DisplaysWide;
      //clearScreen(true);
        ret=1;
    } else if (marqueeOffsetX > DMD_PIXELS_ACROSS * DisplaysWide) {
      marqueeOffsetX = -marqueeWidth;
      //clearScreen(true);
        ret=1;
    }
    
        
    if (marqueeOffsetY < -marqueeHeight) {
      marqueeOffsetY = DMD_PIXELS_DOWN * DisplaysHigh;
      //clearScreen(true);
        ret=1;
    } else if (marqueeOffsetY > DMD_PIXELS_DOWN * DisplaysHigh) {
      marqueeOffsetY = -marqueeHeight;
      //clearScreen(true);
        ret=1;
    }

    // Special case horizontal scrolling to improve speed
    if (amountY==0 && amountX==-1) {
        // Shift entire screen one bit
        for (i=(marqueeOffsetY*(DisplaysWide*4)); i<DMD_RAM_SIZE_BYTES*DisplaysTotal;i++) {
            if ((i%(DisplaysWide*4)) == (DisplaysWide*4) -1) {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1)+1;
            } else {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1) + ((bDMDScreenRAM[i+1] & 0x80) >>7);
            }
        }
      /*          
			 do {
				 for (i=(colom*(DisplaysWide*4)); i<(colom*(DisplaysWide*4))+(DisplaysWide*4);i++) {
            //if ((i%(DisplaysWide*4)) == (DisplaysWide*4) -1) {
					  bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1) + ((bDMDScreenRAM[i+1] & 0x80) >>7); };
            bDMDScreenRAM[i+1]=(bDMDScreenRAM[i]<<1)+1;
            ///} else {
               // bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1) + ((bDMDScreenRAM[i+1] & 0x80) >>7);
            ///}
           colom++;
			  } while (colom<16); colom=marqueeOffsetY;
				*/
        // Redraw last char on screen
        strWidth=marqueeOffsetX;
        for (i=0; i < marqueeLength; i++) {
            int wide = charWidth(marqueeText[i]);
            if (strWidth+wide >= DisplaysWide*DMD_PIXELS_ACROSS) {
                drawChar(strWidth, marqueeOffsetY,marqueeText[i],GRAPHICS_NORMAL);
                return ret;
            }
            strWidth += wide+1;
        }
    } else if (amountY==0 && amountX==1) {
        // Shift entire screen one bit
        for (i=(DMD_RAM_SIZE_BYTES*DisplaysTotal)-1; i>=0;i--) {
            if ((i%(DisplaysWide*4)) == 0) {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]>>1)+128;
            } else {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]>>1) + ((bDMDScreenRAM[i-1] & 1) <<7);
            }
        }

        // Redraw last char on screen
        strWidth=marqueeOffsetX;
        for ( i=0; i < marqueeLength; i++) {
            int wide = charWidth(marqueeText[i]);
            if (strWidth+wide >= 0) {
                drawChar(strWidth, marqueeOffsetY,marqueeText[i],GRAPHICS_NORMAL);
                return ret;
            }
            strWidth += wide+1;
        }
    } else {
      
        drawString(marqueeOffsetX, marqueeOffsetY, marqueeText, marqueeLength,
         GRAPHICS_NORMAL);
    }

    return ret;
}

_Bool stepMarqueePanel(int amountX,int amountY )
{
	int i;
	int strWidth;
	int colom;
    _Bool ret=0;
    marqueeOffsetX += amountX;
    marqueeOffsetY += amountY;
	  colom = marqueeOffsetY;
    if (marqueeOffsetX < (-marqueeWidth+32) ) {  // ditambah lebarnya jam
      marqueeOffsetX = DMD_PIXELS_ACROSS * DisplaysWide;
      //clearScreen(true);
        ret=1;
    } else if (marqueeOffsetX > DMD_PIXELS_ACROSS * DisplaysWide) {
      marqueeOffsetX = -marqueeWidth;
      //clearScreen(true);
        ret=1;
    }
    
    /*    
    if (marqueeOffsetY < -marqueeHeight) {
      marqueeOffsetY = DMD_PIXELS_DOWN * DisplaysHigh;
      //clearScreen(true);
        ret=1;
    } else if (marqueeOffsetY > DMD_PIXELS_DOWN * DisplaysHigh) {
      marqueeOffsetY = -marqueeHeight;
      //clearScreen(true);
        ret=1;
    }
*/
    // Special case horizontal scrolling to improve speed
    if (amountY==0 && amountX==-1) {
        // Shift entire screen one bit
        /*for (i=(marqueeOffsetY*(DisplaysWide*4)); i<DMD_RAM_SIZE_BYTES*DisplaysTotal;i++) {
            if ((i%(DisplaysWide*4)) == (DisplaysWide*4) -1) {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1)+1;
            } else {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1) + ((bDMDScreenRAM[i+1] & 0x80) >>7);
            }
        }
                */
			 do {
				 for (i=(colom*(DisplaysWide*4))+4; i<(colom*(DisplaysWide*4))+(DisplaysWide*4);i++) {
            if ((i%(DisplaysWide*4)) == (DisplaysWide*4) -1) {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1)+1;
            } else {
					  bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1) + ((bDMDScreenRAM[i+1] & 0x80) >>7); };
					};
            //bDMDScreenRAM[i+1]=(bDMDScreenRAM[i]<<1)+1;
            ///} else {
               // bDMDScreenRAM[i]=(bDMDScreenRAM[i]<<1) + ((bDMDScreenRAM[i+1] & 0x80) >>7);
            ///}
           colom++;
			  } while (colom<16); colom=marqueeOffsetY;
        // Redraw last char on screen
        strWidth=marqueeOffsetX;
        for (i=0; i < marqueeLength; i++) {
            int wide = charWidth(marqueeText[i]);
            if (strWidth+wide >= DisplaysWide*DMD_PIXELS_ACROSS) {
                drawChar(strWidth, marqueeOffsetY,marqueeText[i],GRAPHICS_NORMAL);
                return ret;
            }
            strWidth += wide+1;
        }
    } 
		/*else if (amountY==0 && amountX==1) {
        // Shift entire screen one bit
        for (i=(DMD_RAM_SIZE_BYTES*DisplaysTotal)-1; i>=0;i--) {
            if ((i%(DisplaysWide*4)) == 0) {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]>>1)+128;
            } else {
                bDMDScreenRAM[i]=(bDMDScreenRAM[i]>>1) + ((bDMDScreenRAM[i-1] & 1) <<7);
            }
        }

        // Redraw last char on screen
        strWidth=marqueeOffsetX;
        for ( i=0; i < marqueeLength; i++) {
            int wide = charWidth(marqueeText[i]);
            if (strWidth+wide >= 0) {
                drawChar(strWidth, marqueeOffsetY,marqueeText[i],GRAPHICS_NORMAL);
                return ret;
            }
            strWidth += wide+1;
        }
    } else {
      
        drawString(marqueeOffsetX, marqueeOffsetY, marqueeText, marqueeLength,
         GRAPHICS_NORMAL);
    }
  */
    return ret;
}

/*--------------------------------------------------------------------------------------
 Scan the dot matrix LED panel display, from the RAM mirror out to the display hardware.
 Call 4 times to scan the whole display which is made up of 4 interleaved rows within the 16 total rows.
 Insert the calls to this function into the main loop for the highest call rate, or from a timer interrupt
--------------------------------------------------------------------------------------*/
void spi(unsigned char data) {
	//uint8_t h;
	for (u8 h = 0; h < 8; h++)
  {
	if((data & 0x80) != 0){ AP_GPIO->swporta_dr |= BIT(P14); } else {  AP_GPIO->swporta_dr &= ~BIT(P14);   }
	//if((data & 0x80) != 0){  GPIOA_ResetBits(GPIO_Pin_1);; } else {  GPIOA_SetBits(GPIO_Pin_1);   }
	//data = (data & 0x80)  ? (AP_GPIO->swporta_dr |= BIT(24)) : (AP_GPIO->swporta_dr &= ~BIT(24))
		data <<= 1; 
	//if((data1 & 0x80) != 0) {  AP_GPIO->swporta_dr &= ~BIT(24); } else {  AP_GPIO->swporta_dr |= BIT(24);   }
	//	data1 <<= 1;
	AP_GPIO->swporta_dr |= BIT(P7);   AP_GPIO->swporta_dr &= ~BIT(P7);
	//GPIOA->BRR  = GPIO_Pin_15;   GPIOC->BRR = GPIO_Pin_11;
 };
}

uint8_t flag_sw=1;
void scanDisplayBySPI(void)
{ 
  //int hub2=DisplaysTotal*64;
  int rowsize=DisplaysTotal<<2 ; //,ion;
  int offset=rowsize * bDMDByte;
 //  polaris=2;
   switch (flag_sw) {
   			case 1:
   				for (int i = 0; i < 4; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   				flag_sw=2;	break;
   			case 2:
   				for (int i = 4; i < 8; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=3;	break;
   			case 3:
   				for (int i = 8; i < 12; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=4;	break;
   			case 4:
   				for (int i = 12; i < 16; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=5;	break;
				case 5:
   				for (int i = 16; i < 20; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=6;	break;
				case 6:
   				for (int i = 20; i < 24; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=7;	break;
			 case 7:
   				for (int i = 24; i < 28; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=8;	break;
				case 8:
   				for (int i = 28; i < 32; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=9;	break;
				case 9:
   				for (int i = 32; i < 36; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=10;	break;
				case 10:
   				for (int i = 36; i < 40; i++) {
   					spi(bDMDScreenRAM[offset + i + row3]);
   					spi(bDMDScreenRAM[offset + i + row2]);
   					spi(bDMDScreenRAM[offset + i + row1]);
   					spi(bDMDScreenRAM[offset + i]);
   					}
   					flag_sw=11;	break;
   		}; //break;

   	if (flag_sw == (DisplaysTotal-1)) { PWM_SET_CMP_VAL(PWM_CH0,0); };

   	if (flag_sw > DisplaysTotal) {
   		flag_sw = 1; //PWM_SET_CMP_VAL(PWM_CH0,0);
          switch (bDMDByte) {
           case 0:                              // row 1, 5, 9, 13 were clocked out
               LIGHT_DMD_ROW_01_05_09_13;
               bDMDByte=1;
               break;
           case 1:                              // row 2, 6, 10, 14 were clocked out
               LIGHT_DMD_ROW_02_06_10_14;
               bDMDByte=2;
               break;
           case 2:                              // row 3, 7, 11, 15 were clocked out
               LIGHT_DMD_ROW_03_07_11_15;
               bDMDByte=3;
               break;
           case 3:                              // row 4, 8, 12, 16 were clocked out
               LIGHT_DMD_ROW_04_08_12_16;
               bDMDByte=0;
               break;
           }
          LATCH_DMD_SHIFT_REG_TO_OUTPUT;
          PWM_SET_CMP_VAL(PWM_CH0,(set.britnes*10));//R8_PWM_OUT_EN |= (CH_PWM6);
          //if(++test>0xffffffff) { test=0;}
     }
   }







