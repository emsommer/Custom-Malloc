#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  F_first        1   /* This is the first call to the function. */
#define  F_last         2   /* This is the last call to the function. Free the memory area. */
#define  F_data_int     3   /* Void * argument points to integer data. */
#define  F_data_char    4   /* Void * argument points to character string. */
#define  F_data_float   5   /* Void * argument points to a float data value. */
#define  F_print        6   /* Print the accumulated values. */

//THIS FUNCTION SHOULD STORE DATA IN THE FOLLOWING WAY:

//the first two bytes of mem should contain the int value of offset (offset should = the begining of free memory in mem)
//the next byte should contain the identifier for the type of data you are about to store, i.e. 3,4 or 5 as illustrated by the #define statements above
//following that identifier should be the data itself (ints are 4 bytes and so are floats, the char will be an unknown size)

//we should be able to add as much data to mem as we want and print its contents at any time (should only be freed once F_LAST is called)

void * f (int code, void * mem, void * data)
{   

    long long int z_data = (long long int)data;

    //invalid code
    if((code > 6) | (code < 1))
    {
        return 0;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODE FIRST 
    if (code == F_first)
    {
        //mem must equal null for first call
        if((mem != NULL) | (z_data < 1))
        {
            return 0;
        }

        //initialize the memory 
        mem = (void *) malloc(z_data);

        //set the first two bytes to be the offset which should default to 2
        short *offset;
        offset = (short *)mem;

        //default
        *offset = 2;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODE LAST
    if (code == F_last)
    {
        //mem must not equal null for calls other than first 
        if(mem == NULL)
        {
            return 0;
        }

        //free all of mem
        free(mem);
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODE STORE INT
    if (code == F_data_int) {

        //mem must not equal null for calls other than first 
        if(mem == NULL)
        {
            return 0;
        }

        //set offset = to first 2 bytes of mem (i.e offset = current offset)
        short *offset;
        offset = (short *)mem;

        //copy of mem pointing to the same memory location as mem
        char* curr_mem = (char *)mem;

        //set curr mem = begin of free memory 
        curr_mem = curr_mem + *offset;

        //store the data type and increment past it to next free mem
        *curr_mem = F_data_int;
        curr_mem++;

        //store the data into mem and increment to next free mem
        *((int *)curr_mem) = *((int *)data);
        *offset = *offset + 5;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODE STORE FLOAT
    if (code == F_data_float) {
        
        //mem must not equal null for calls other than first 
        if(mem == NULL)
        {
            return 0;
        }

        //set offset = to first 2 bytes of mem (i.e offset = current offset)
        short *offset;
        offset = (short *)mem;

         //copy of mem pointing to the same memory location as mem
        char *curr_mem = (char *)mem;

        //set curr mem = begin of free memory
        curr_mem = curr_mem + *offset;

        //store the data type and increment past it to next free mem
        *curr_mem = F_data_float;
        curr_mem++;

        //store the data of the float and increment to next free mem
        *((float *)curr_mem) = *((float *)data);
        *offset = *offset + 5;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODE STORE CHAR
    if (code == F_data_char) {

        //mem must not equal null for calls other than first 
        if(mem == NULL)
        {
            return 0;
        }

        //set offset = to first 2 bytes of mem (i.e offset = current offset)
        short *offset;
        offset = (short *)mem;

         //copy of mem with pointing to the same memory location as mem
        char *curr_mem = (char *)mem;

        //set curr mem = begin of free memory
        curr_mem = curr_mem + *offset;

        //store the data type and increment past it to next free mem
        *curr_mem = F_data_char;
        curr_mem++;
        *offset = *offset + 1;

        //while the current char in the read in string is not equal to the null terminator
        while(*((char *)data) != '\0')
        {
            //store the char and increment to next free mem
            *((char *)curr_mem) = *((char *)data);
            curr_mem++;

            //increment offset
            *offset = *offset + 1;

            //increment to next char in the passed in data
            data++;
        }
        //add null terminator
        *((char *)curr_mem) = '\0';

        //skip null terminator for the next check of data type
        *offset = *offset + 1;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////// CODE PRINT 
    if (code == F_print) {

        //mem must not equal null for calls other than first 
        if(mem == NULL)
        {
            return 0;
        }

        //copy of mem pointing to the same memory as mem
        char *curr_mem = (char *)mem;

        //point curr_mem to the first data type
        curr_mem = curr_mem + 2;
    
        //size = size of current mem in bytes (so we can check each byte)
        int size = sizeof(mem);

        int i = 0;
        
        //i think this works?? (for(int i = 0 ; i < size ; i ++))
        while(i <= size)
        {
            //prints an int
            if(*curr_mem == F_data_int)
            {
                //iterate past data type
                curr_mem++;

                //print the int 
                printf("%d", *((int *)curr_mem));

                //point to end of actual data (i.e the next data type that needs to be printed)
                curr_mem = curr_mem + 4;
            }        

            //prints a float 
            if(*curr_mem == F_data_float)
            {    
                //iterate past the data type
                curr_mem++;

                //print the float
                printf("%.2f", *((float *)curr_mem));

                //point to end of actual data (i.e the next data type that needs to be printed)
                curr_mem = curr_mem + 4;
            }

            //prints a string
            if(*curr_mem == F_data_char)
            {
                //iterate past data type 
                curr_mem++;

                //while the current byte of curr_mem != the null terminator
                while(*((char *)curr_mem) != '\0')
                {
                    //print the current character
                    printf("%c", *((char *)curr_mem));

                    curr_mem++;
                    //increment to next byte of curr_mem
                }

                //skip the null terminator for next type comparison
                curr_mem++;
            }
        //iterate through the size of the current memory
        i++;
        }
    }

    return mem;
}