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

int calctone(image In, int nl, int nc, int mn, int ntones)
{
    int i, j, median, sum, value, scaled;
    for (i = 0, sum = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
        {
            sum += In[i * nc + j];
        }
    median = sum / (nl * nc);
    return ((float)median / mn) * ntones;
}

void createimgtone(image In, image Out, int nl, int nc, int mn, int value)
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
    int ntones, nrows, ncols, nfiles, i, j, y, x, nr, nc, ml, tone;
    char *filenames[20], name[102], command[100];
    image *icon, *imgtone, In, Out;

   /*-------------------------------------------
    * Read data from config.txt
    *-------------------------------------------*/
    if ((arq = fopen("config.txt", "rt")) == NULL)
        error("File <config.txt> not found!");
    fscanf(arq, "%d %d %d", &ntones, &nrows, &ncols);
    fscanf(arq, "%d ", &nfiles);

    if (nfiles > 20)
        error("Max. files number exceeded!");
    for (i = 0; i < nfiles; i++)
    {
        fscanf(arq, "%s", name);
        filenames[i] = malloc(strlen(name) * sizeof(char));
        strcpy(filenames[i], name);
        //puts(name);
    }
    fclose(arq);

   /*-------------------------------------------
    * Iconize images
    *-------------------------------------------*/
    icon = malloc(nfiles * sizeof(image));
    if (!icon)
        error("Icon vector creation error!");
    for (i = 0; i < nfiles; i++)
    {
        In = img_readpgm(filenames[i], &nr, &nc, &ml);
        //puts (filenames[i]);
        if (!In)
            error("Image read pgm error!");
        icon[i] = img_alloc(nrows, ncols);
        if (!icon[i])
            error("Icon image creation error!");
        iconize(In, icon[i], nr, nc, ml, nrows, ncols);
        img_free(&In);
    }

   /*-------------------------------------------
    * Create images of tones
    *-------------------------------------------*/
    srand((unsigned)time(NULL));
    imgtone = malloc(ntones * sizeof(image));
    if (!imgtone)
        error("Tone image creation error!");
    for (tone = 0; tone < ntones; tone++)
    {
        int iconsorted = rand() % (nfiles-1) + 1; // skip de fisrt image
        //printf ("Icon sorted = %d\n", iconsorted);
        int tonecalculated = calctone(icon[iconsorted], nrows, ncols, ml, ntones+1);
        int value = (tone - tonecalculated) * ml / (ntones+1);
        imgtone[tone] = img_alloc(nrows, ncols);
        if (!imgtone[tone])
            error("Tone image creation error!");
        createimgtone(icon[iconsorted], imgtone[tone], nrows, ncols, ml, value);
    }

   /*-------------------------------------------
    * Create mosaic
    *-------------------------------------------*/
    Out = img_alloc(nrows * nrows, ncols * ncols);
    if (!Out)
        error("Out image creation error!");
    for (i = 0; i < nrows; i++)
        for (j = 0; j < ncols; j++)
        {
            tone = (float)icon[0][i * ncols + j] / ml * (ntones-1); // pixels of first image listed
            //printf ("%2d", tone);
            if (tone < 0 || tone >= ntones)
                error ("Range tones image error!");
            for (y = 0; y < nrows; y++)
                for (x = 0; x < ncols; x++) 
                    Out[(i * nrows + y) * ncols * ncols + j * ncols + x] = imgtone[tone][y * ncols + x];
        }
    sprintf(name, "%s-mosaic.pgm", filenames[0]);
    img_writepgm(Out, name, nrows * nrows, ncols * ncols, ml);
    sprintf(command, "eog %s &", name);
    system(command);
    return 0;
}
