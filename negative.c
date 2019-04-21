/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Negative from grayscale image
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "imagelib.h"

void negative(image In, image Out, int nl, int nc, int mn) {
    int i, j;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++) {
            Out[i * nc + j] = 255 - In[i * nc + j];
        }
}

void msg(char *s) {
    printf("\nImage negative");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  <IMG.PGM>\n\n", s);
    printf("    <IMG.PGM> is pgm image file \n\n");
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    int OK, nc, nr, ml;
    char name[100];
    char command[100];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);
    else {
        In = img_readpgm (argv[1], &nr, &nc, &ml);
        if (In) {
            printf ("\nImage negative");
            img_info (argv[1], nr, nc, ml);
            Out = img_alloc (nr, nc);

            /*-- transformation --*/
            negative (In, Out, nr, nc, ml);

            sprintf (name, "%s-%s", argv[1], "-negative.pgm");
            img_writepgm (Out, name, nr, nc, ml);
            sprintf (command, "eog %s &", name);
            system(command);
            img_free (&In);
            img_free (&Out);
        }
    }
    return 0;
}

