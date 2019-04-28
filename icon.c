/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Iconize an image
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "imagelib.h"

void iconize(image In, image Out, int nl, int nc, int mn, int ny, int nx)
{
    int i, j, y, x;
    for (i = 0, y = 0; i < ny; i++, y += (float)nl / (ny+1))
        for (j = 0, x = 0; j < nx; j++, x += (float)nc / (nx+1))
            Out[i * nx + j] = In[y * nc + x];
}

void msg(char *s)
{
    printf("\nIconize image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  <IMG.PGM> yDim xDim\n\n", s);
    printf("    <IMG.PGM> is pgm image file \n");
    printf("    yDim is icon's number of rows \n");
    printf("    xDim is icon's number of columns \n");
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int OK, nc, nr, ml, ny, nx;
    char name[100];
    char command[100];
    image In, Out;
    if (argc < 4)
        msg(argv[0]);
    else
    {
        In = img_readpgm(argv[1], &nr, &nc, &ml);
        ny = atoi(argv[2]);
        nx = atoi(argv[3]);
        if (In)
        {
            printf("\nImage negative");
            img_info(argv[1], nr, nc, ml);
            Out = img_alloc(ny, nx);

            /*-- transformation --*/
            iconize(In, Out, nr, nc, ml, ny, nx);

            sprintf(name, "%s-%s", argv[1], "icon.pgm");
            img_writepgm(Out, name, ny, nx, ml);
            sprintf(command, "eog %s &", name);
            system(command);
            img_free(&In);
            img_free(&Out);
        }
    }
    return 0;
}
