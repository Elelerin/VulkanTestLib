#include "../include/bmp.h"
#include <stdio.h>
#include <stdlib.h>
/*
C - BMP LOAD
*/

#define iSIZE sizeof(int)
#define sSIZE sizeof(short)

//PRINT HEADER DATA
void printBMPHeaderData(BITMAPINFOHEADER* toPrint){
    printf("== BITMAP HEADER INFO ==\n");
    printf("HEIGHT: %d\n", toPrint->height);
    printf("WIDTH: %d\n", toPrint->width);
    printf("IMAGE SIZE: %d\n", toPrint->imagesize);
    printf("'FILE SIZE': %d\n", toPrint->size);
    printf("'FILE SIZE LESS HEADERS': %d\n", toPrint->size - 54);
    printf("BITS PER PIXEL: %d\n", toPrint->bits);
    printf("COMPRESSION TYPE: %d\n", toPrint->compression);
    printf("NUMBER OF COLORS: %d\n", toPrint->ncolours);
}

//GET HEADER DATA
static inline const BITMAPINFOHEADER* loadBmpHeader (const char* fileName){
    BITMAPINFOHEADER* toReturn = malloc(sizeof(BITMAPINFOHEADER));
    FILE* fptr = fopen(fileName, "r");

    if(fptr == NULL){
        printf("Error, failed to read: %s", fileName);
        free(toReturn);
        return NULL;
    }

    //Allocate Temporary Memory for copying
    int* _temp = malloc(iSIZE);
    *_temp = 0;
    fread(_temp, sizeof(short), 1, fptr);
    if(*_temp != 0x4D42){
        printf("The file %s is not a bitmap file (FILE DID NOT START WITH 'BM').\n", fileName);
        free(_temp);
        free(toReturn);
        return NULL;
    }

    fseek(fptr, 10, SEEK_SET);
    fread(&toReturn->size, iSIZE, 1, fptr);
    fseek(fptr, 18, SEEK_SET);
    fread(&toReturn->width, iSIZE, 1, fptr);
    fread(&toReturn->height, iSIZE, 1, fptr);
    fread(&toReturn->planes, sSIZE, 1, fptr);
    fread(&toReturn->bits, sSIZE, 1, fptr);
    fread(&toReturn->compression, iSIZE, 1, fptr);
    fread(&toReturn->imagesize, iSIZE, 1, fptr);
    fread(&toReturn->xresolution, iSIZE, 1, fptr);
    fread(&toReturn->yresolution, iSIZE, 1, fptr);
    fread(&toReturn->ncolours, iSIZE, 1, fptr);
    fread(&toReturn->importantcolours, iSIZE, 1, fptr);

    if(&toReturn->bits < 8){
        printf("ERROR: COLOR TABLE OFFSET");
    }

    fclose(fptr);
    free(_temp);
    return toReturn;

    BADCOMPRESSIONBI:
        fclose(fptr);
        free(_temp);
        free(toReturn);
        printf("Error: Bad Bitmap Compression. Unsupported.\n");
        return NULL;
}

//LOAD BITMAP INFO AS 'BITMAPINFO'
BITMAPINFO* loadBITMAPINFO(const char* fileName){
    BITMAPINFOHEADER* header = loadBmpHeader(fileName);
    BITMAPINFO toReturn;
    toReturn.bmiHeader = header;
    toReturn.BMDATA = malloc(header->imagesize);

    #ifdef VERBOSE
    printf("== LOADING BMP ==\n\n");
    printf("Allocated Memory: %d Bytes\n", header->imagesize);
    #endif

    FILE* fptr = fopen(fileName, "r");
    fseek(fptr, header->size - 1, SEEK_SET);
    fread(toReturn.BMDATA, toReturn.bmiHeader->imagesize, 1, fptr);

    #ifdef VERBOSE
    printf("DATASIZE, LOADING BMP: %d\n", header->imagesize);
    for(int i = header->imagesize; i > 0 ; i -= 3){
        char b = -(char)toReturn.BMDATA[i];
        char g = -(char)toReturn.BMDATA[i-1];
        char r = -(char)toReturn.BMDATA[i-2];

        if(i/3 >= 10){
            printf("PIXEL %d ----- R: %d, G: %d, B: %d\n", i/3, r, g, b);
        }else{
            printf("PIXEL %d ------ R: %d, G: %d, B: %d\n", i/3, r, g, b);
        }

    }
    #endif

    free(header);
    fclose(fptr);
    return &toReturn;
}

bmpPixelData getBMPData(const char * fileName, int *x, int *y, int *channels, int *imgSize){
    BITMAPINFOHEADER* header = loadBmpHeader(fileName);
    bmpPixelData toReturn = malloc(header->imagesize);

    FILE* fptr = fopen(fileName, "r");
    fseek(fptr, header->size, SEEK_SET);
    fread(toReturn, header->imagesize, 1, fptr);

    *x = header->width;
    *y = header->height;
    *channels = header->compression;
    *imgSize = header->imagesize;

    free(header);
    fclose(fptr);
    return toReturn;
}
