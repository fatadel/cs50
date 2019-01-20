// Enlarges or shrinks infile by the factor of n

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy n infile outfile, where n is a floating-point value in (0.0, 100.0]\n");
        return 1;
    }

    // convert the factor from string (char *) to float
    float n = atof(argv[1]);

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


    // So now we check whether we need to enlarge image or shrink since the methods are different
    if (n >= 1)
    {
        // Let's enlarge!

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
    }
    else
    {
        // Let's shrink it!

        // iterate over outfile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                // skip over horizontal pixel duplicates in infile (horizontal skipping)
                fseek(inptr, ((1 / n) - 1) * sizeof(RGBTRIPLE), SEEK_CUR);
            }

            // skip over the padding in infile
            fseek(inptr, paddingOld, SEEK_CUR);

            // add paddings in the outfile
            for (int k = 0; k < paddingNew; k++)
            {
                fputc(0x00, outptr);
            }

            // skip over scanline duplicates in infile (vertical skipping)
            fseek(inptr, ((1 / n) - 1) * bi.biWidth / n * sizeof(RGBTRIPLE) + paddingOld, SEEK_CUR);

        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
