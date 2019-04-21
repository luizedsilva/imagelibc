#include <stdio.h>

#define CREATOR "# CREATOR: Image Processing usind C-Ansi\n"

typedef int *image;

/*-------------------------------------------------------------------------
 * Image allocation and free routines
 *   nr = number of rows
 *   nc = number of columns
 *-------------------------------------------------------------------------*/
image img_alloc(int nr, int nc)
{
    return (image)malloc(nr * nc * sizeof(int));
}

int img_free(image *Im)
{
    free(*Im);
}

/*-------------------------------------------------------------------------
 * Display image information
 *   name = file name image
 *   nr = number of rows
 *   nc = number os columns
 *   ml = max grayscale level
 *-------------------------------------------------------------------------*/
void img_info(char *name, int nr, int nc, int ml)
{
    printf("\nImage Informations:");
    printf("\n--------------------------\n");
    printf("Image file name.............: %s \n", name);
    printf("Number of rows..............: %d \n", nr);
    printf("Number of columns...........: %d \n", nc);
    printf("Max grayscale level.........: %d \n\n", ml);
}

/*-------------------------------------------------------------------------
 * Read pgm image
 *   PGM format:
 * P2
 * # CREATOR: Image Processing usind C-Ansi
 * 124 122
 * 255
 * 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255
 * 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255
 *  (...)
 * Line 1: P2 identify PGM ASCII file
 * Line 2: Lines begins with # are comment
 * Line 3: Numbers of columns and rows
 * Line 4: Max grayscale value
 * Line 5...: Pixel value of image
 *-------------------------------------------------------------------------+*/
image img_readpgm(char *name, int *nr, int *nc, int *ml)
{
    int i, j, k;
    char c, lines[100];
    image img;
    FILE *fimg;

    if ((fimg = fopen(name, "r")) == NULL)
    {
        printf("File open error: <%s>\n\n", name);
        return NULL;
    }

    /*--- PGM = "P2" ---*/
    fgets(lines, 80, fimg);
    if (!strstr(lines, "P2"))
    {
        printf("File image format error: <%s>\n\n", name);
        return NULL;
    }

    /*--- Comment lines ---*/
    fgets(lines, 80, fimg);
    do
    {
        fgets(lines, 80, fimg);
    } while (strchr(lines, '#'));
    sscanf(lines, "%d %d", nc, nr);
    fscanf(fimg, "%d", ml);

    if (*nc == 0 || *nr == 0 || *ml == 0)
    {
        printf("Image dimensions error: <%s>\n\n", name);
        return NULL;
    }

    img = img_alloc(*nr, *nc);
    if (!img)
        printf("Image allocation error: %s\n\n img_readpgm routine\n\n", name);
    else
    {
        for (i = 0; i < *nr; i++)
        {
            for (j = 0; j < *nc; j++)
            {
                fscanf(fimg, "%d", &k);
                if (k > *ml)
                {
                    printf("Max grayscale image error: <%s>\n", name);
                    return NULL;
                }
                img[i * (*nc) + j] = k;
            }
        }
    }
    fclose (fimg);
    return img;
}

/*-------------------------------------------------------------------------
 * Write pgm image
 * Params:
 *   img = image
 *   name = image file name
 *   nr = number of rows
 *   nc = number of columns
 *   ml = max grayscale level
 *-------------------------------------------------------------------------*/
void img_pgmwrite (image img, char *name, int nr, int nc, int ml)
{
    int i, j, x, k, values_per_lines = 16;
    FILE *fimg;
    if ((fimg = fopen(name, "wt")) == NULL)
    {
        printf("Image creation error: <%s>\n\n", name);
    }
    else
    {
        fputs("P2\n", fimg);
        fputs(CREATOR, fimg);
        fprintf(fimg, "%d  %d\n", nc, nl);
        fprintf(fimg, "%d\n", mn);
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++)
            {
                x = *(I + i * nc + j);
                fprintf(arq, "%3d ", x);
                k++;
                if (k > valores_por_linha)
                {
                    fprintf(arq, "\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}
