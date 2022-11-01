#ifndef FIXEDNUMS8x160_H
#define FIXEDNUMS8x160_H

//#include <inttypes.h>
//#include <avr/pgmspace.h>


const unsigned char fixednums8x160[] = {
     0x04, 0x52, // size // size of zero indicates fixed width font
    6,		// width
    16,		// height
    '0',	// first char (48)
    12,		// char count

// char widths
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 
    0x04, 0x04, 


    0xfc, 0xfe, 0x07, 0x07, 0xfe, 0xfc, 0x3f, 0x7f, 0xe0, 0xe0, 0x7f, 0x3f,   // char '0'
    0x18, 0x1c, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xc0,   // char '1'
    0x3c, 0x3e, 0x07, 0x87, 0xfe, 0xfc, 0xf8, 0xfe, 0xcf, 0xc3, 0xc1, 0xc0,   // char '2'
    0x1c, 0x1e, 0x83, 0x83, 0xfe, 0x7c, 0x38, 0x78, 0xc1, 0xc1, 0x7f, 0x3e,   // char '3'
    0xc0, 0xf0, 0x3c, 0xff, 0xff, 0x00, 0x07, 0x07, 0x06, 0xff, 0xff, 0x06,   // char '4'
    0xff, 0xff, 0x63, 0x63, 0xe3, 0xc3, 0x38, 0x78, 0xe0, 0xe0, 0x7f, 0x3f,   // char '5'
    0xfc, 0xfe, 0x87, 0x87, 0x1e, 0x1c, 0x3f, 0x7f, 0xe1, 0xe1, 0x7f, 0x3e,   // char '6'
    0x07, 0x03, 0xc3, 0xe3, 0x7f, 0x3f, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,   // char '7'
    0x7c, 0xfe, 0x87, 0x87, 0xfe, 0x7c, 0x3e, 0x7f, 0xe1, 0xe1, 0x7f, 0x3e,   // char '8'
    0x7c, 0xfe, 0x87, 0x87, 0xfe, 0xfc, 0x38, 0x78, 0xe1, 0xe1, 0x7f, 0x3f,   // char '9'
    0x00, 0x70, 0x70, 0x00, 0x00, 0x0E, 0x0E, 0x00, // 58
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 59
   // 0x00, 0x38, 0x38, 0x00, 0x00, 0x07, 0x07, 0x00, // 60 // 60 "<"
    //0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 61 "="
};
#endif

