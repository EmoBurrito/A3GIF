/* Author: cst221 (Stephanie McCallum) & cst235 (Nathan Smith)
 * Date: Apr 6, 2017
 * Title:
 */
#include "GIF_Functions.h"
#include <stdio.h>
#include <stdlib.h>
//If you want true sepia, set this to TRUE. Else, colorized sepia.
#define FUSSY FALSE
//////////////
//	TESTS	//
//////////////

void GreyScale(PIXEL * p)
{
	BYTE Grey = 0;
	Grey = (BYTE)((int)(p->bBlu + p->bGrn + p->bRed) / 3);
	p->bRed = Grey;
	p->bGrn = Grey;
	p->bBlu = Grey;
}

void Negative(PIXEL * p)
{
	p->bRed = (BYTE)((int)(255 - p->bRed));
	p->bGrn = (BYTE)((int)(255 - p->bGrn));
	p->bBlu = (BYTE)((int)(255 - p->bBlu));
}

void Sepia(PIXEL * p)
{
	if(FUSSY)
	{
		GreyScale(p);
	}
	p->bRed = (BYTE)((p->bRed * .439) + (p->bGrn * .259) + (p->bBlu * .078));
	p->bGrn = (BYTE)((p->bRed * .439) + (p->bGrn * .259) + (p->bBlu * .078));
	p->bBlu = (BYTE)((p->bRed * .439) + (p->bGrn * .259) + (p->bBlu * .078));
}

void SwapNibbles(PIXEL * p)
{
	p->bRed = (p->bRed <<4) | (p->bRed >> 4);
	p->bGrn = (p->bGrn <<4) | (p->bGrn >> 4);
	p->bBlu = (p->bBlu <<4) | (p->bBlu >> 4);
}

void TestReadWriteImage()
{
	FILE * infile = GetFile("Enter file to open", "rb");
	FILE * outfile = GetFile("Enter output file name(and extension)", "w+b");
	IMAGE img = {NULL, NULL, NULL, 0};
	ReadImage(&img, infile);
	WriteImage(&img, outfile);
	fclose(outfile);
	FreeImage(&img);
}

void TestManipImage()
{
	FILE * infile = GetFile("Enter file to open", "rb");
	FILE * outfile = GetFile("Enter output file name(and extention)", "w+b");
	IMAGE img = {NULL, NULL};
	ReadImage(&img, infile);
	ManipImage(&img, Sepia);
	WriteImage(&img, outfile);
	fclose(outfile);
	FreeImage(&img);
}
//////////////
//	MAIN	//
//////////////
int main(void)
{
	setbuf(stdout, NULL);
	//TestReadWriteImage();
	TestManipImage();
	return EXIT_SUCCESS;
}
