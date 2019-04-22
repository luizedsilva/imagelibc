/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Darken ou Lighten grayscale image
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imagelib.h"

void graytone(image In, image Out, int nl, int nc, int mn, float factor)
{
    int i, j;
    float T[mn];
    for (i = 0; i < mn; i++)
        T[i] = (float)pow(i, factor) / pow(mn, factor) * mn;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
        {
            Out[i * nc + j] = T[In[i * nc + j]];
        }
}

void msg(char *s)
{
    printf("\nGrayscale Change");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  <IMG.PGM> FACTOR\n\n", s);
    printf("    <IMG.PGM> is pgm image file \n");
    printf("    FACTOR > 1.0 to darken\n");
    printf("    0.0 < FACTOR < 1.0 to lighten\n\n");
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int OK, nc, nr, ml;
    char name[100];
    char command[100];
    image In, Out;
    float fact;
    if (argc < 3)
        msg(argv[0]);
    else
    {
        In = img_readpgm(argv[1], &nr, &nc, &ml);
        fact = atof (argv[2]);
        if (In)
        {
            printf("\nGray tone modify");
            img_info(argv[1], nr, nc, ml);
            Out = img_alloc(nr, nc);

            /*-- transformation --*/
            graytone (In, Out, nr, nc, ml, fact);

            sprintf(name, "%s-%s", argv[1], "graytone.pgm");
            img_writepgm(Out, name, nr, nc, ml);
            sprintf(command, "eog %s &", name);
            system(command);
            img_free(&In);
            img_free(&Out);
        }
    }
    return 0;
}
