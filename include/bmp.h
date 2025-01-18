#ifndef BMP_H
#define BMP_H

#pragma once
#include <stdio.h>

#include "../include/KEYVALS.h"


#define BMP_NO_COMPRESSION 0
#define BMP_8BIT_ENCODING 1
#define BMP_4BIT_ENCODING 2
#define BMP_RGB_MASK 3


//MEMORY LEAK FREE (?)
//TESTED BY LOADING 1000 IMAGES INTO MEMORY, THEN FREEING THEM
//REMEMBER TO UNLOAD YOUR IMAGES!!! (¬_¬")

typedef char* bmpPixelData;

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */


typedef struct {
   unsigned short int type;                 /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data, bytes */
} BITMAPHEADER;

typedef struct {
   unsigned int size;               /* Header size in bytes      */
   int width,height;                /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
   unsigned int compression;        /* Compression type          */
   unsigned int imagesize;          /* Image size in bytes       */
   int xresolution,yresolution;     /* Pixels per meter          */
   unsigned int ncolours;           /* Number of colours         */
   unsigned int importantcolours;   /* Important colours         */
} BITMAPINFOHEADER;

typedef struct{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} RGBQUAD;

typedef struct{
    BITMAPINFOHEADER* bmiHeader;
    bmpPixelData* BMDATA;
} BITMAPINFO;

void printBMPHeaderData(BITMAPINFOHEADER* toPrint);

static inline const BITMAPINFOHEADER* loadBmpHeader(const char* fileName);

BITMAPINFO* loadBITMAPINFO(const char* fileName);

bmpPixelData getBMPData(const char* fileName, int *x, int *y, int *channels, int *imgSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // BMP_H
