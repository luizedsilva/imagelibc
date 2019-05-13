/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Mean Filter
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "imagelib.h"

void selectionSort (int *v, int n) {
    int i, j, less, temp;
    for (i = 0; i < n - 1; i++) {
        less = i;
        for (j = i + 1; j < n; j++)
            if (v[j] < v[less])
                less = j;
        temp = v[i];
        v[i] = v[less];
        v[less] = temp;
    }
}

void mean (image In, image Out, int nr, int nc, int ml) {
    int i, j, vet[9];
    for (i = 1; i < nr - 1; i++)
        for (j = 1; j < nc - 1; j++) {
            vet[0] = In[(i - 1) * nc + j - 1];
            vet[1] = In[(i - 1) * nc + j];
            vet[2] = In[(i - 1) * nc + j + 1];
            vet[3] = In[i * nc + j - 1];            
            vet[4] = In[i * nc + j];
            vet[5] = In[i * nc + j + 1];            
            vet[6] = In[(i + 1) * nc + j - 1];
            vet[7] = In[(i + 1) * nc + j];
            vet[8] = In[(i + 1) * nc + j + 1];
            selectionSort (vet, 9);
            Out[i * nc + j] = vet[4];
        }
}

void msg(char *s) {
    printf("\nMean filter");
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
            printf ("\nMean filter");
            img_info (argv[1], nr, nc, ml);
            Out = img_alloc (nr, nc);

            /*-- transformation --*/
            mean (In, Out, nr, nc, ml);

            sprintf (name, "%s-%s", argv[1], "filtered.pgm");
            img_writepgm (Out, name, nr, nc, ml);
            sprintf (command, "eog %s &", name);
            system(command);
            img_free (&In);
            img_free (&Out);
        }
    }
    return 0;
}