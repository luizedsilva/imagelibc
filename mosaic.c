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

typedef struct {
    int nrows, ncols, maxl;
    char *filename;
    image icon;
} imgtype;

typedef imgtype *ptimg;

int main(void)
{
    FILE *arq;
    int tone, ntones, nrows, ncols, nfiles, i, j, y, x;
    char name[102], command[100];
    image *imgtone, In, Out;
    ptimg imginfo;

   /*-------------------------------------------
    * Read data from config.txt
    *-------------------------------------------*/
    if ((arq = fopen("config.txt", "rt")) == NULL)
        error("File <config.txt> not found!");
    fscanf(arq, "%d %d %d", &ntones, &nrows, &ncols);
    fscanf(arq, "%d ", &nfiles);

    if (nfiles > 20)
        error("Max. files number exceeded!");

    imginfo = malloc (nfiles * sizeof(imgtype));
    if (!imginfo)
        error("Image info structure creation error!");

    for (i = 0; i < nfiles; i++)
    {
        fscanf(arq, "%s", name);
        imginfo[i].filename = malloc(strlen(name) * sizeof(char));
        strcpy(imginfo[i].filename, name);
        //puts(name);
    }
    fclose(arq);

   /*-------------------------------------------
    * Iconize images
    *-------------------------------------------*/

    for (i = 0; i < nfiles; i++)
    {
        In = img_readpgm(imginfo[i].filename, &imginfo[i].nrows, &imginfo[i].ncols, &imginfo[i].maxl);
        //puts (imginfo[i].filenames);
        if (!In)
            error("Image read pgm error!");
        imginfo[i].icon = img_alloc(nrows, ncols);
        if (!imginfo[i].icon)
            error("Icon image creation error!");
        iconize(In, imginfo[i].icon, imginfo[i].nrows, imginfo[i].ncols, imginfo[i].maxl, nrows, ncols);
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
        int isorted = rand() % (nfiles-1) + 1; // skip de fisrt image
        //printf ("Icon sorted = %d\n", iconsorted);
        int tonecalculated = calctone(imginfo[isorted].icon, nrows, ncols, imginfo[isorted].maxl, ntones+1);
        int value = (tone - tonecalculated) * imginfo[isorted].maxl/ (ntones+1);
        imgtone[tone] = img_alloc(nrows, ncols);
        if (!imgtone[tone])
            error("Tone image creation error!");
        createimgtone(imginfo[isorted].icon, imgtone[tone], nrows, ncols, imginfo[isorted].maxl, value);
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
            tone = (float)imginfo[0].icon[i * ncols + j]/ imginfo[0].maxl * (ntones-1); // pixels of first image listed
            //printf ("%2d", tone);
            if (tone < 0 || tone >= ntones)
                error ("Range tones image error!");
            for (y = 0; y < nrows; y++)
                for (x = 0; x < ncols; x++) 
                    Out[(i * nrows + y) * ncols * ncols + j * ncols + x] = imgtone[tone][y * ncols + x];
        }
    sprintf(name, "%s-mosaic.pgm", imginfo[0].filename);
    img_writepgm(Out, name, nrows * nrows, ncols * ncols, imginfo[0].maxl);
    sprintf(command, "eog %s &", name);
    system(command);
    return 0;
}
