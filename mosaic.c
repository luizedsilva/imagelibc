/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program to create photomosaic from images
 * 
 * By Luiz Eduardo da Silva.
 * 
 * It use 'config.txt' file which may be organized as follows:
 *  + The first line of the file must contain three numbers that define
 *    the NTONES number of grayscale tiles, the NROWS number of rows
 *    and the NCOLS number of mosaic columns and images that will be
 *    used for this composition.
 *  + The second line contains the NFILES number of images that will be 
 *    used for mounting.
 *  + From the third line onwards the file contains the names of the
 *    NFILES images that will be used in the assembly. The first image of 
 *    the list is also used as base image, ie all images will be iconized 
 *    and darkened or cleared to be used as 'pixels' for mounting the first 
 *    image.
 * 
 * Example:
 * --------
 *   32 60 80
 *   5
 *   image1.pgm
 *   image2.pgm
 *   image3.pgm
 *   image4.pgm
 *   image5.pgm
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "imagelib.h"

/*-------------------------------------------------------------------------
 * Iconize image
 * Params:
 *   In = Input image
 *   Out= Output image (icon)
 *   nl = number of rows
 *   nc = number of columns
 *   mn = max grayscale level
 *   ny = number of icon's rows
 *   nx = number of icon's columns
 *-------------------------------------------------------------------------*/
void iconize(image In, image Out, int nl, int nc, int mn, int ny, int nx)
{
    int i, j, y, x;
    for (i = 0, y = 0; i < ny; i++, y += (float)nl / (ny + 1))
        for (j = 0, x = 0; j < nx; j++, x += (float)nc / (nx + 1))
            Out[i * nx + j] = In[y * nc + x];
}

/*-------------------------------------------------------------------------
 * calculates the average hue in the image
 * Params:
 *   In = Input image
 *   nl = number of rows
 *   nc = number of columns
 *   mn = max grayscale level
 *   ntones = scale of tones
 *-------------------------------------------------------------------------*/
int calctone(image In, int nl, int nc, int mn, int ntones)
{
    int i, sum;
    for (i = 0, sum = 0; i < nl * nc; i++)
        sum += In[i];
    return ((float)sum / (nl * nc) / mn) * ntones;
}

/*-------------------------------------------------------------------------
 * creates an image for a given hue value
 * Params:
 *   In = Input image
 *   Out= Output image
 *   nl = number of rows
 *   nc = number of columns
 *   mn = max grayscale level
 *   value = hue value
 *-------------------------------------------------------------------------*/
void createimgtone(image In, image Out, int nl, int nc, int mn, int value)
{
    int i;
    for (i = 0; i < nl * nc; i++)
    {
        int sum = value + In[i];
        sum = sum > 255 ? 255 : sum;
        sum = sum < 0 ? 0 : sum;
        Out[i] = sum;
    }
}

/*-------------------------------------------------------------------------
 * structure for images icons
 *-------------------------------------------------------------------------*/
typedef struct
{
    int nrows, ncols, maxl;
    char *filename;
    image icon;
} imgtype;

typedef imgtype *ptimg;

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
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
    ERROR((arq = fopen("config.txt", "rt")) == NULL, errormsg("File <config.txt> not found!"));
    fscanf(arq, "%d %d %d", &ntones, &nrows, &ncols);
    fscanf(arq, "%d ", &nfiles);
    ERROR(nfiles > 20, errormsg("Maximum files number exceeded!"));
    imginfo = malloc(nfiles * sizeof(imgtype));
    ERROR(!imginfo, errormsg("Image info structure creation error!"));
    for (i = 0; i < nfiles; i++)
    {
        fscanf(arq, "%s", name);
        imginfo[i].filename = malloc(strlen(name) * sizeof(char) + 1);
        strcpy(imginfo[i].filename, name);
    }
    fclose(arq);
    /*-------------------------------------------
    * Iconize images
    *-------------------------------------------*/
    for (i = 0; i < nfiles; i++)
    {
        In = img_readpgm(imginfo[i].filename, &imginfo[i].nrows, &imginfo[i].ncols, &imginfo[i].maxl);
        imginfo[i].icon = img_alloc(nrows, ncols);
        ERROR(!imginfo[i].icon, errormsg("Icon image creation error!"));
        iconize(In, imginfo[i].icon, imginfo[i].nrows, imginfo[i].ncols, imginfo[i].maxl, nrows, ncols);
        img_free(&In);
    }
    /*-------------------------------------------
    * Create images of tones
    *-------------------------------------------*/
    srand((unsigned)time(NULL));
    imgtone = malloc(ntones * sizeof(image));
    ERROR(!imgtone, errormsg("Tone's image vector creation error!"));
    for (tone = 0; tone < ntones; tone++)
    {
        int isorted = rand() % (nfiles - 1) + 1; // skip de first image
        int calculatedtone = calctone(imginfo[isorted].icon, nrows, ncols, imginfo[isorted].maxl, ntones + 1);
        int value = (tone - calculatedtone) * imginfo[isorted].maxl / (ntones + 1);
        imgtone[tone] = img_alloc(nrows, ncols);
        ERROR(!imgtone[tone], errormsg("Tone [%d] image creation error!", tone));
        createimgtone(imginfo[isorted].icon, imgtone[tone], nrows, ncols, imginfo[isorted].maxl, value);
    }
    /*-------------------------------------------
    * Create mosaic
    *-------------------------------------------*/
    Out = img_alloc(nrows * nrows, ncols * ncols);
    ERROR(!Out, errormsg("Out image creation error!"));
    for (i = 0; i < nrows; i++)
        for (j = 0; j < ncols; j++)
        {
            tone = (float)imginfo[0].icon[i * ncols + j] / imginfo[0].maxl * (ntones - 1); // pixels of first listed image
            ERROR(tone < 0 || tone >= ntones, errormsg("Tone [%d] range error!", tone));
            for (y = 0; y < nrows; y++)
                for (x = 0; x < ncols; x++)
                    Out[((i * nrows + y) * ncols + j) * ncols + x] = imgtone[tone][y * ncols + x];
        }
    sprintf(name, "%s-mosaic.pgm", imginfo[0].filename);
    img_writepgm(Out, name, nrows * nrows, ncols * ncols, imginfo[0].maxl);
    sprintf(command, "eog %s &", name);
    system(command);
    img_free(&Out);
    for (i = 0; i < nfiles; i++)
    {
        free(imginfo[i].filename);
        img_free(&imginfo[i].icon);
    }
    for (tone = 0; tone < ntones; tone++)
        img_free(&imgtone[tone]);
    return 0;
}