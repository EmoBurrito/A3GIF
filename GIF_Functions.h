/*
 * gif_header.h
 *
 *  Created on: Apr 3, 2017
 *      Author: cst221 & cst235
 */

#ifndef GIF_FUNCTIONS_H_
#define GIF_FUNCTIONS_H_
#include <stdio.h>

#define BOOL unsigned char
#define BYTE unsigned char
#define WORD unsigned short
#define GIFHDRSIZE 13 //measured in bytes
#define COLORTABLESIZE 256 //measured in pixels

#define TRUE 1
#define FALSE 0

//We must set the byte alignment on our structures to align on
//1 byte boundaries.  We do this so that we can read  from
//a file and directly place the information into a struct.

#pragma pack (1)
typedef struct
{
	//Header
	BYTE Type[6]; //signature (GIF) and version (either 87a or 89a)
	//Logical Screen Descriptor
	WORD Width; //image width in pixels
	WORD Height; //image height in pixels
	BYTE GCTFollows; //1 if a color table exists, 0 otherwise.
	BYTE BackGroundColor; //index of the color in GCT used as the boarder and background color
	BYTE PixelAspectRatio; //don't use this
}GIF_HEADER;

typedef struct
{
	BYTE bRed, bGrn, bBlu;
} PIXEL;
#pragma pack ()

typedef struct
{
	GIF_HEADER * ghHeader; //13 bytes
	PIXEL * pColorTable; //768 bytes total
	BYTE * bData;
	int iDataSize;
}IMAGE;


//Get handle to a gif file
FILE * GetFile(char * cPrompt, char * cMode);
//Functions to read and write image data to/from files
void ReadImage(IMAGE * img, FILE * infile);
void ReadHeader(IMAGE * img, FILE * infile);
void ReadColorTable(IMAGE * img, FILE * infile);
void ReadData(IMAGE * img, FILE * infile);

void WriteImage(IMAGE * img, FILE * outfile);
void WriteHeader(IMAGE * img, FILE ** outfile);
void WriteColorTable(IMAGE * img, FILE ** outfile);
void WriteData(IMAGE * img, FILE * outfile);

//Functions to manipulate pixels
//We a need a function pointer that will point to functions that can manipulate
//a PIXEL
typedef void (*GIF_FUNC_PTR) (PIXEL *);
BOOL ManipImage(IMAGE * img, GIF_FUNC_PTR pixelFunc);

//Free the memory that was dynamically allocated
void FreeImage(IMAGE * img);




#endif /* GIF_FUNCTIONS_H_ */
