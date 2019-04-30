/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Change tone by scale
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "imagelib.h"

int calcscaled(image In, int nl, int nc, int mn, int scale)
{
    int i, j, median, sum, value, scaled;
    for (i = 0, sum = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
        {
            sum += In[i * nc + j];
        }
    median = sum / (nl * nc);
    return ((float)median / mn) * scale;
}

void createtone(image In, image Out, int nl, int nc, int mn, int value)
{
    int i, j, sum;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
        {
            int sum = value + In[i * nc + j];
            if (sum > 255)
                sum = 255;
            if (sum < 0)
                sum = 0;
            Out[i * nc + j] = sum;
        }
}

void msg(char *s)
{
    printf("\nChange tone for grayscale image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  <IMG.PGM> <SCALE>\n\n", s);
    printf("    <IMG.PGM> is pgm image file \n");
    printf("    <SCALE>   grayscale \n\n");
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int OK, nc, nr, ml, tone, scale, scaled;
    char name[100];
    char command[100];
    image In, Out;
    if (argc < 3)
        msg(argv[0]);
    else
    {
        In = img_readpgm(argv[1], &nr, &nc, &ml);
        scale = atoi(argv[2]);
        if (In)
        {
            printf("\nCreate tones grayscale images");
            img_info(argv[1], nr, nc, ml);
            Out = img_alloc(nr, nc);

            /*-- transformation --*/
            scaled = calcscaled (In, nr, nc, ml, scale);
            for (tone = 0; tone < scale; tone++)
            {
                int value = (tone - scaled) * ml / (scale + 1);
                createtone(In, Out, nr, nc, ml, value);
                sprintf(name, "%s-tone-%d%s", argv[1], tone, ".pgm");
                img_writepgm(Out, name, nr, nc, ml);
                //sprintf(command, "eog %s &", name);
                system(command);
            }
            img_free(&In);
            img_free(&Out);
        }
    }
    return 0;
}
