/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t  CHAR; 
bool isJPEG(CHAR array[], int size);

int main(void)
{

    CHAR buffer[512]; 
    
    // Everytime close JPEG, increment x as new one
    int x = 0; 
    
    // Declares img pointer so can close any JPEGs at start
    FILE* img = NULL;  

    FILE* inptr = fopen("card.raw", "rb");
    
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", "card.raw");
        return 1;
    }
    
    
    while (512 == fread(buffer, sizeof(CHAR), 512, inptr)) 
    {
               
        // if first four bytes are...then create image file
        if (true == isJPEG(buffer, 512)) 
        { 
            // closes any open JPEG files 
            if (NULL != img)
            {
                fclose(img);
                img = NULL; 
            }
            
            char title [10];  
            sprintf(title, "%03d.jpg", x); 
            
            // title is stored as char array, now call with fopen function to create JPEG       
            img = fopen(title, "wb");               
            
            if (NULL == img)
            {
                printf("Could not open %s\n", title);
                return 2; 
            }
            
            // write bytes from mem card to image file 
            fwrite(buffer, sizeof(CHAR), 512, img);            
            x++;  
        }
        
        else
        {   
            if (NULL != img)
            {
                fwrite(buffer, sizeof(CHAR), 512, img);
            }
        }
       
    }
     
    if (NULL != img)
    {
        fclose(img);
    }
     
    if (NULL != inptr)
    {
        fclose(inptr);
    }
     
    return 0; 
}

bool isJPEG(CHAR array[], int size)
{
    if (*(array + 0) == 0xff && *(array + 1) == 0xd8 && *(array + 2) == 0xff && *(array + 3) == 0xe0)
    {
        return true;
    }    
    
    
    else if (*(array + 0) == 0xff && *(array + 1) == 0xd8 && *(array + 2) == 0xff && *(array + 3) == 0xe1)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

        
        
    




