/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize infile outfile\n");
        return 1;
    }

    // remember filenames and factor
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (n < 0 || n > 100)
    {
        fprintf(stderr, "n, the resize factor, must satisfy 0 < n <= 100.\n");
        return 2;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //update output file width and height
    bi.biWidth = n * bi.biWidth;
    bi.biHeight = n * bi.biHeight;

    // determine padding for scanlines
    int paddingin =  (4 - ((bi.biWidth/n) * sizeof(RGBTRIPLE)) % 4) % 4; //infile
    int paddingout = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;// outfile


    bi.biSizeImage = abs(bi.biHeight) * ((bi.biWidth * sizeof (RGBTRIPLE)) + paddingout);
    bf.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);


    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);



    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight/n); i < biHeight; i++)
    {
        for(int repeat = 0; repeat < n; repeat++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < (bi.biWidth/n); j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int k = 0; k < n; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding, if any
            fseek(inptr, paddingin, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int l = 0; l < paddingout; l++)
            {
                fputc(0x00, outptr);
            }
            fseek(inptr, - ((bi.biWidth / n) * 3 + paddingin) , SEEK_CUR);
        }
        fseek(inptr, (bi.biWidth / n) * 3 + paddingin, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}