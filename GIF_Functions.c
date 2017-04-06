/*
 * GIF_Functions.c
 *
 *  Created on: Apr 3, 2017
 *      Author: cst221
 */
#include "GIF_Functions.h"
#include <malloc.h>
#define DEBUG TRUE

//Get handle to a gif file
FILE * GetFile(char * cPrompt, char * cMode)
{
	FILE * aFile = NULL;
	char cFileName[256];
	printf("%s: \n", cPrompt);
	scanf("%s", cFileName);
	//Open the file with the mode passed in
	aFile = fopen(cFileName, cMode);
	return aFile;
}

//Functions to read and write image data to/from files
void ReadImage(IMAGE * img, FILE * infile)
{
	if(img != NULL && infile != NULL)
	{
		printf("image wasn't null, infile wasn't null\n");
		//read in the header of the image first
		ReadHeader(img, infile);
		//If the header was read in
		if(img->ghHeader != NULL)
		{
			printf("header not null\n");
			//Read in the pixel array/data of the image
			ReadColorTable(img, infile);
			if(img->pColorTable != NULL)
			{
				printf("CT not null\n");
				ReadData(img, infile);
			}
		}
	}
}
void ReadHeader(IMAGE * img, FILE * infile)
{
	BOOL bSuccess = FALSE;
	//Allocate memory for the header
	if((img->ghHeader = (GIF_HEADER *)malloc(GIFHDRSIZE)) != NULL)
	{
		if(fread(img->ghHeader, GIFHDRSIZE, 1, infile) == 1)
		{
			printf("read the header\n");
			printf("gonna be %d wide and %d high\n", img->ghHeader->Width, img->ghHeader->Height);
			bSuccess = TRUE;
		}
	}

	//What happens if we failed to read
	if(!bSuccess && img->ghHeader != NULL)
	{
		printf("header didn't work");
		free(img->ghHeader);
		img->ghHeader = NULL;
	}
}
void ReadColorTable(IMAGE * img, FILE * infile)
{
	BOOL bSuccess = FALSE;
	//Allocate memory for pixel data
	if((img->pColorTable = (PIXEL *)malloc(sizeof(PIXEL) * COLORTABLESIZE)) != NULL)
	{
		//Read in the pixel array
		if((fread(img->pColorTable, sizeof(PIXEL) * COLORTABLESIZE, 1, infile) == 1))
		{
			//printf("read the color table\n");
			//printf("this color table is %d in size\n", (unsigned int)sizeof(img->pColorTable));
			bSuccess = TRUE;
		}
	}
	//If we failed to read
	if(!bSuccess)
	{
		//printf("didn't read the color table\n");
		//Free the header
		if(img->ghHeader != NULL)
		{
			free(img->ghHeader);
			img->ghHeader = NULL;
		}
		//Free the data if it exists
		if(img->bData != NULL)
		{
			free(img->bData );
			img->bData = NULL;
		}
	}
}
void ReadData(IMAGE * img, FILE * infile)
{
	BOOL bSuccess = FALSE;
	//how big is this whole image supposed to be??
	fseek(infile, 0, SEEK_END);
	int entire_file_size = ftell(infile);
	//printf("entire file size returned from fseek() is: %d\n", entire_file_size);
	//figure out the data size for the image
	img->iDataSize = entire_file_size - GIFHDRSIZE - (sizeof(PIXEL) * COLORTABLESIZE);
	//reset seek to beginning of file
	fseek(infile, 0, SEEK_SET);
	if(fseek(infile, (GIFHDRSIZE + (sizeof(PIXEL) * COLORTABLESIZE)),SEEK_SET) == 0)
	{
		//printf("I SEEKED the thing\n");
		//try mallocing
		if((img->bData = (BYTE *)malloc(img->iDataSize)) != NULL)
		{
			//printf("I could allocate space for data\n");
			//try reading
			if((fread(img->bData, img->iDataSize, 1, infile) == 1))
			{
				//printf("i thinks i read a data\n");
				//printf("data is %d in size\n", img->iDataSize);
				bSuccess = TRUE;
			}
		}
	}

	//If we failed to read
	if(!bSuccess)
	{
		//printf("I didn't read the data\n");
		//Free the header
		if(img->ghHeader != NULL)
		{
			free(img->ghHeader);
			img->ghHeader = NULL;
		}
		//Free the color table
		if(img->pColorTable != NULL)
		{
			free(img->pColorTable);
			img->pColorTable = NULL;
		}
		//Free the data if it exists
		if(img->bData != NULL)
		{
			free(img->bData );
			img->bData = NULL;
		}
	}
}

void WriteImage(IMAGE * img, FILE * outfile)
{
	if(img != NULL && outfile != NULL)
	{
		//printf("think i'll write an image now\n");
		//Write the header out
		WriteHeader(img, &outfile);
		//If the header was written correctly
		if(outfile != NULL)
		{
			//Write the pixel array
			WriteColorTable(img, &outfile);
			WriteData(img, outfile);
		}
	}
}
void WriteHeader(IMAGE * img, FILE ** outfile)
{
	BOOL bSuccess = FALSE;
	if(img->ghHeader != NULL)
	{
		//printf("hey i know the file is %d x %d before i write it\n", img->ghHeader->Width, img->ghHeader->Height);
		if(fwrite(img->ghHeader, GIFHDRSIZE, 1, *outfile) == 1)
		{
			//printf("i'm at %d now\n", (int)ftell(*outfile));
			//printf("hey i wrote the header\n");
			bSuccess = TRUE;
		}
	}
	//if we failed
	if(!bSuccess && outfile != NULL)
	{
		//printf("couldn't write header\n");
		fclose(*outfile);
		outfile = NULL;
	}
}
void WriteColorTable(IMAGE * img, FILE ** outfile)
{
	BOOL bSuccess = FALSE;
	if(img->ghHeader != NULL && img->pColorTable != NULL)
	{
		if(fwrite(img->pColorTable,sizeof(PIXEL) * COLORTABLESIZE, 1, *outfile) == 1)
		{
			//printf("i'm at %d now, should be 781\n", (int)ftell(*outfile));
			//printf("heck yes wrote color table\n");
			bSuccess = TRUE;
		}
	}
	if(!bSuccess && outfile != NULL)
	{
		if(DEBUG){printf("couldn't write color table\n");}
		fclose(*outfile);
		outfile = NULL;
	}
}
void WriteData(IMAGE * img, FILE * outfile)
{
	BOOL bSuccess = FALSE;
	if(img->ghHeader != NULL && img->pColorTable != NULL && img->bData != NULL)
	{
		//size = (img->ghHeader->Width * sizeof(PIXEL)) * (img->ghHeader->Height);

		if(fwrite(img->bData, img->iDataSize, 1, outfile) == 1)
		{
			//printf("i'm at %d now, should be 434119\n", (int)ftell(outfile));
			//printf("oh bb wrote data\n");
			bSuccess = TRUE;
		}
	}
	//if we failed to write at any point
	if(!bSuccess && outfile != NULL)
	{
		printf("couldn't write data\n");
		//Nuke 'n' NULL
		fclose(outfile);
		outfile = NULL;
	}
}

//Functions to manipulate pixels
//We a need a function pointer that will point to functions that can manipulate
//a PIXEL
//PIXEL * GetPixel(unsigned int row, unsigned int col, IMAGE * img)
//{
//	//Offset = size of a complete row, * num of complete rows, + num bytes
//	//	preceding desired pixel in the last row.
//	//unsigned int offset = (imgPtr->bmHDR->dwWidth * sizeof(PIXEL) + padding) * row + col * sizeof(PIXEL);
//	return (PIXEL *)((BYTE*)imgPtr->bmData + offset);
//}
BOOL ManipImage(IMAGE * img, GIF_FUNC_PTR pixelFunc)
{
	BOOL bSuccess = TRUE;
	int i = 0, j = 0;
	if(img != NULL && pixelFunc != NULL)
	{
		if(img->ghHeader != NULL && img->pColorTable != NULL && img->bData != NULL)
		{
			for (i = 0; i < img->ghHeader->Height; ++i)
			{
				for (j = 0; j < img->ghHeader->Width; ++j)
				{
					//pixelFunc(GetPixel(i,j,img));
				}
			}
		}
		else
		{
			bSuccess = FALSE;
		}
	}
	else
	{
		bSuccess = FALSE;
	}
	return bSuccess;
}

//Free the memory that was dynamically allocated
void FreeImage(IMAGE * img)
{
	free(img->ghHeader);
	img->ghHeader = NULL;
	free(img->pColorTable);
	img->ghHeader = NULL;
	free(img->bData);
	img->bData = NULL;
}
