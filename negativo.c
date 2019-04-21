/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Negative from grayscale image
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imagelib.h"

void negativo(imagem In, imagem Out, int nl, int nc, int mn) {
    int i, j;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++) {
            Out[i * nc + j] = 255 - In[i * nc + j];
        }
}

void msg(char *s) {
    printf("\nNEGATIVO de uma imagem qualquer");
    printf("\n-------------------------------");
    printf("\nUSO.:  %s  <IMG.PGM>", s);
    printf("\nONDE:\n");
    printf("    <IMG.PGM>  Arquivo da imagem em formato PGM\n\n");
}

/*+------------------------------------------------------+
  |        P R O G R A M A    P R I N C I P A L          |
  +------------------------------------------------------+*/
int main(int argc, char *argv[]) {
    int OK, nc, nl, mn, tipo_img;
    char nome[100];
    char comando[100];
    imagem In, Out;
    if (argc < 2)
        msg(argv[0]);
    else {
        OK = le_imagem_pgm(argv[1], &In, &nl, &nc, &mn);
        if (OK) {
            printf("\nNegativo da imagem");
            info_imagem(argv[1], nl, nc, mn);
            aloca_memo(&Out, nl, nc);
            negativo(In, Out, nl, nc, mn);
            sprintf (nome, "%s-%s", argv[1], "-negativo.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            sprintf (comando, "eog %s &", nome);
            system(comando);
            desaloca_memo(&In);
            desaloca_memo(&Out);
        }
    }
    return 0;
}
