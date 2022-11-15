//DONE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

 //Define datatype BYTE
 typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check commandline argument
    if (argc != 2)
    {
        printf("Usage: ./recover images\n");
        return 1;
    }

    //Open memory card file. "r" for readmode.
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Could not open your file");
        return 1;
    }

     //Buffer
     BYTE buffer[512];
     char filename[8];
     FILE *recovered_img;
     int JPEG_counter = 0;

    //Read files with fread, which returns number of items of size size were read.
     while (fread (buffer, 512, 1, f) == 1)
     {
         //Look for JPEG header: detect for bytes at beginning of 512 byte (0xff 0xd8 0xff 0xe.).
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (JPEG_counter == 0)
            {
            //making new JPEG file
            sprintf(filename, "%03i.jpg", JPEG_counter);
             
            //open new JPEG file
            recovered_img = fopen(filename, "w"); //"w" for writing
            
            //write
            fwrite(buffer, 512, 1, recovered_img);
             
            JPEG_counter++;
            }
            
            else if (JPEG_counter > 0) //next JPEG 
            {
                fclose(recovered_img);

                sprintf(filename, "%03i.jpg", JPEG_counter);

                recovered_img = fopen(filename, "w");

                fwrite(buffer, 512, 1, recovered_img);

                JPEG_counter++;
            }
        }

        //write files
        else if(recovered_img > 0)
        {
            fwrite(buffer, 512, 1, recovered_img);
        }
    
     }
     
     fclose(f);
     fclose(recovered_img);
     
     return 0;
}