/* Author: cst221 (Stephanie McCallum) & cst235 (Nathan Smith)
 * Date: Apr 3, 2017
 * Title:
 */
#include "GIF_Functions.h"
#include <stdio.h>
#include <stdlib.h>

//////////////
//	TESTS	//
//////////////

void Negative(PIXEL * p)
{
	p->bBlu = ~p->bBlu;
	p->bRed = ~p->bRed;
	p->bGrn = ~p->bGrn;
}

void Sepia(PIXEL * p)
{
	p->bRed =(BYTE)((p->bRed * .439) + (p->bGrn * .259) + (p->bBlu * .078));
	p->bGrn = (BYTE)((p->bRed * .439) + (p->bGrn * .259) + (p->bBlu * .078));
	p->bBlu = (BYTE)((p->bRed * .439) + (p->bGrn * .259) + (p->bBlu * .078));
}

void SwapNibbles(PIXEL * p)
{
	p->bBlu = (p->bBlu <<4) | (p->bBlu >> 4);
	p->bRed = (p->bRed <<4) | (p->bRed >> 4);
	p->bGrn = (p->bGrn <<4) | (p->bGrn >> 4);
}

void TestManipulateImage()
{
	FILE * infile = GetFile("Enter file to open", "rb");
	FILE * outfile = GetFile("Enter output file name(and extension)", "w+b");
	IMAGE img = {NULL, NULL, NULL, 0};
	ReadImage(&img, infile);
	WriteImage(&img, outfile);
	fclose(outfile);
}
//////////////
//	MAIN	//
//////////////
int main(void)
{
	setbuf(stdout, NULL);
	TestManipulateImage();




	return EXIT_SUCCESS;
}
