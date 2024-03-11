#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  F_first        1   /* This is the first call to the function. */
#define  F_last         2   /* This is the last call to the function. Free the memory area. */
#define  F_data_int     3   /* Void * argument points to integer data. */
#define  F_data_char    4   /* Void * argument points to character string. */
#define  F_data_float   5   /* Void * argument points to a float data value. */
#define  F_print        6   /* Print the accumulated values. */

void * f (int code, void * mem, void * data);

int main()
{
    int     i_a;
    float   f_a;

    void  * m; 
    int   * ip;
    float * fp;
    
 
    fp = & f_a;
    ip = & i_a;

    m = f (F_first, 0, (void *)200);           /* we really don't know how much it will take */

    //used to check the 0 byte allocation case
    //m = f (F_first, 0, (void *)0);           /* we really don't know how much it will take */

    m = f (F_data_char, m,  (void *)"Ethan gives gorkem a grade of ");

    f_a = 24.7;
    //tests fp
    m = f (F_data_float,  m,  (void *)fp);

    m = f (F_data_char, m,  (void *)" out");
    //test only a space
    m = f (F_data_char, m,  (void *)" ");

    m = f (F_data_char, m,  (void *)"of ");

    i_a = 100;
    //tests int
    m = f (F_data_int,  m,  (void *)ip);
    //tests special characters
    m = f (F_data_char, m,  (void *)"\n");

    m = f (F_print, m, 0);
    //tests freeing mem using f_last
    m = f(F_last, m, 0);
    //tests freeing mem this way
    //free(m);
}

