/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes 24-bit uncompressed BMPs by a factor of n.
 * 
 * Usage: ./copy n infile outfile
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    
    if (n < 1 || n > 100)
    {
        printf ("Please provide a valid n\n");
        return 2;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
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
    
    // Store dimensions from old bitmap 
    
    long old_biWidth = bi.biWidth;
    long old_biHeight = bi.biHeight;
    int old_padding = (4 - (old_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Store dimensions of new bitmap
    
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n; 
    int new_padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
       
    bi.biSizeImage = (bi.biWidth * abs(bi.biHeight)) * 3 + (new_padding * abs(bi.biHeight));  
    bf.bfSize = bi.biSizeImage + 54; 

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }    
    
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    // int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // for each scanline being read from old file
    for (int h = 0; h < abs(old_biHeight); h++)
    {  
        
        for (int i = 0; i < n; i++)
        { 
        
            // iterate over pixels in scanline
            for (int j = 0; j < old_biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;            

                                            
                // read RGB triple from infile - using old dimensions. If use new dimensions then read into paddiing                   
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // Using pixels stored in triple, write n times
                for (int k = 0; k < n; k++) 
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }                          
            
            }
            // skip over padding, if any
            fseek(inptr, old_padding, SEEK_CUR);

            // Add in new padding to out-file
            for (int l = 0; l < new_padding; l++)
            {
                fputc(0x00, outptr);
            }
            
            // Sets pointer for in-file back to first pixel
            fseek(inptr, -(old_biWidth * 3 + old_padding), SEEK_CUR); 
            
        
        
        }
        // Moves pointer to end of scan line 
        fseek(inptr, old_biWidth * 3 + old_padding, SEEK_CUR); 
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
