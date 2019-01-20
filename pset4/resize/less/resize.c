// Enlarges infile n times

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy positive integer infile outfile\n");
        return 1;
    }

    // we are enlarging the file n times, so we need n
    // we also need to convert it from string(char *) to int
    int n = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
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

    // calculate the padding for infile
    int paddingOld = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate the new width and height
    bi.biWidth *= n;
    bi.biHeight *= n;


    // calculate the padding for scanlines for outfile
    int paddingNew = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate the new value for biSizeImage
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + paddingNew) * abs(bi.biHeight);

    // calculate the new bfSize
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // counter used for resizing vertically (duplicating the scanlines)
    int count = 1;

    // iterate over infile's scanlines, but keeping in mind that we repeat each scanline n times
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth / n; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // duplicate the pixel n times for horizontal resizing
            for (int k = 0; k < n; k++)
            {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // if count equals to n, means we are ready to proceed to the next scanline
        if (count == n)
        {
            // skip over padding, if any, to proceed to the next scanline
            fseek(inptr, paddingOld, SEEK_CUR);
            // reset the counter
            count = 1;
        }
        // otherwise we need to duplicate each scanline n times
        else
        {
            // go back to the begining of the scanline
            fseek(inptr, -bi.biWidth / n * sizeof(RGBTRIPLE), SEEK_CUR);
            // increase the counter
            count++;
        }

        // add paddings in the outfile
        for (int k = 0; k < paddingNew; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
