/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Create photomosaic from images
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "imagelib.h"

#define DEBUG(x) x

void iconize(image In, image Out, int nl, int nc, int mn, int ny, int nx)
{
    int i, j;
    int py = nl / (ny - 1);
    int px = nc / (nx - 1);
    for (i = 0; i < ny; i++)
        for (j = 0; j < nx; j++)
        {
            Out[i * nx + j] = In[i * py * nc + j * px];
        }
}

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

void error(char *s)
{
    puts(s);
    exit(10);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(void)
{
    FILE *arq;
    int ntones, nrows, ncols, nfiles;
    char *filenames[20];
    char name[102], command[100];
    int i, j, y, x, nr, nc, ml;
    image *imgtone, In, imgpx, Out;
    if ((arq = fopen("dados.txt", "rt")) == NULL)
        error("File <dados.txt> not found!");
    fscanf(arq, "%d %d %d", &ntones, &nrows, &ncols);
    fscanf(arq, "%d ", &nfiles);
    DEBUG(printf("t=%d tr=%d tc=%d tf=%d\n", ntones, nrows, ncols, nfiles);)
    if (nfiles > 20)
        error("Max. files number exceeded!");
    for (i = 0; i < nfiles; i++)
    {
        fscanf(arq, "%s", name);
        filenames[i] = malloc(strlen(name) * sizeof(char));
        strcpy(filenames[i], name);
        DEBUG(printf("file %d = %s\n", i, name);)
    }
    fclose(arq);
    imgtone = malloc(nfiles * sizeof(image));
    for (i = 0; i < nfiles; i++)
    {
        In = img_readpgm(filenames[i], &nr, &nc, &ml);
        if (!In)
            error("Image read pgm error!");

        imgtone[i] = img_alloc(nrows, ncols);
        if (!imgtone[i])
            error("Image tone creation error!");

        /*-- transformation --*/
        iconize(In, imgtone[i], nr, nc, ml, nrows, ncols);

        img_free(&In);
    }

    srand((unsigned)time(NULL));

    Out = img_alloc(nrows * nrows, ncols * ncols);
    if (!Out)
        error("Image out creation error!");

    imgpx = img_alloc(nrows, ncols);
    if (!imgpx)
        error("Image px creation error!");

    for (i = 0; i < nrows; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            int tone = imgtone[0][i * ncols + j]; // pixels of readed image first
            int scaled = calcscaled(imgtone[0], nrows, ncols, ml, ntones);
            int value = (tone - scaled) * ml / (ntones + 1);
            createtone(imgtone[(rand() % 10)], imgpx, nrows, ncols, ml, value);
            for (y = 0; y < nrows; y++)
                for (x = 0; x < ncols; x++)
                    Out[(i * nrows + y) * ncols * ncols + j * ncols + x] = imgpx[y * ncols + x];
        }
        //DEBUG( printf ("\n"); )
    }
    sprintf(name, "%s-mosaic.pgm", filenames[0]);
    img_writepgm(Out, name, nrows * nrows, ncols * ncols, ml);
    sprintf(command, "eog %s &", name);
    system(command);
    return 0;
}
