/*+-------------------------------------------------------------------------+
  | PROCESSAMENTO DE IMAGEM -                                               |
  | -----------------------                                                 |
  | Programa que calcula o HISTOGRAMA de uma imagem em niveis de cinza.     |
  |                                                                         |
  | Por Luiz Eduardo da Silva.                                              |
  +-------------------------------------------------------------------------+*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0
#define CREATOR "# CREATOR: Luiz Eduardo V:1.00 R:MAR/2015\n"

typedef int * imagem;
typedef int * histograma;

/*+-------------------------------------------------------------------------+
  | Rotinas para ALOCAR e DESALOCAR dinamicamente espaco de memoria para um |
  | vetor monodimensional que armazenara' a imagem.                         |
  | PARAMETROS:                                                             |
  | I  = Endereco do vetor (ponteiro de inteiros).                          |
  | nl = Numero de linhas.                                                  |
  | nc = Numero de colunas.                                                 |
  +-------------------------------------------------------------------------+*/
int aloca_memo(imagem *I, int tam) {
    *I = (int *) malloc(tam * sizeof (int));
    if (*I) return TRUE;
    else return FALSE;
}

int desaloca_memo(imagem *I) {
    free(*I);
}

/*+-------------------------------------------------------------------------+
  | Apresenta informacoes sobre um arquivo de imagem.                       |
  | Parametros:                                                             |
  |   nome = nome fisico do arquivo de imagem.                              |
  |   nl   = numero de linhas da imagem.                                    |
  |   nc   = numero de colunas da imagem.                                   |
  |   mn   = maximo nivel de cinza da imagem.                               |
  +-------------------------------------------------------------------------+*/
void info_imagem(char *nome, int nl, int nc, int mn) {
    printf("\nINFORMACOES SOBRE A IMAGEM:");
    printf("\n--------------------------\n");
    printf("Nome do arquivo.............: %s \n", nome);
    printf("Numero de linhas............: %d \n", nl);
    printf("Numero de colunas...........: %d \n", nc);
    printf("Maximo nivel-de-cinza/cores.: %d \n\n", mn);
}

/*+-------------------------------------------------------------------------+
  | Rotina que faz a leitura de uma imagem em formato .PGM ASCII e armazena |
  | num vetor monodimensional. Um exemplo de imagem .PGM ASCII gravada neste|
  | formato:                                                                |
  |                                                                         |
  | P2                                                                      |
  | # CREATOR: XV Version 3.10a  Rev: 12/29/94                              |
  | 124 122                                                                 |
  | 255                                                                     |
  | 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255     |
  | 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255     |
  | (...)                                                                   |
  |                                                                         |
  | Lin 1: contem P2, o que identifica este arquivo como PGM ASCII.         |
  | Lin 2: contem um comentario qualquer iniciado com #.                    |
  | Lin 3: numero de colunas e numero de linhas da imagem.                  |
  | Lin 4: maximo nivel de cinza na imagem (255 normalmente).               |
  | Lin 5 em diante: valores de cinza da imagem.                            |
  |                                                                         |
  | PARAMETROS:                                                             |
  | nome = nome do arquivo (entra).                                         |
  | I    = ponteiro para o vetor imagem (retorna).                          |
  | nl   = numero de linhas da imagem (retorna).                            |
  | nc   = numero de colunas da imagem (retorna).                           |
  | mn   = maximo nivel de cinza (retorna).                                 |
  +-------------------------------------------------------------------------+*/
int le_imagem_pgm(char *nome, imagem *I, int *nl, int *nc, int *mn) {
    int i, j, k, LIMX, LIMY, MAX_NIVEL;
    char c, LINHA[100];
    FILE *arq;
    if ((arq = fopen(nome, "r")) == NULL) {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- PGM = "P2" -----------*/
    fgets(LINHA, 80, arq);
    if (!strstr(LINHA, "P2")) {
        printf("Erro no FORMATO do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- Dimensoes da imagem --*/
    fgets(LINHA, 80, arq);
    do {
        fgets(LINHA, 80, arq);
    } while (strchr(LINHA, '#'));
    sscanf(LINHA, "%d %d", &LIMX, &LIMY);
    fscanf(arq, "%d", &MAX_NIVEL);

    if (LIMX == 0 || LIMY == 0 || MAX_NIVEL == 0) {
        printf("Erro nas DIMENSOES do arquivo <%s>\n\n", nome);
        return FALSE;
    }

    if (aloca_memo(I, LIMY * LIMX)) {
        for (i = 0; i < LIMY; i++) {
            for (j = 0; j < LIMX; j++) {
                fscanf(arq, "%d", &k);
                if (k > MAX_NIVEL) {
                    printf("Erro nos DADOS do arquivo <%s>\n", nome);
                    printf("Valor lido: %d   Max Nivel: %d\n\n", k, MAX_NIVEL);
                    return FALSE;
                }
                *(*(I) + i * LIMX + j) = k;
            }
        }
        fclose(arq);
    } else {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n", nome);
        printf("Rotina: le_imagem_pgm\n\n");
        fclose(arq);
        return FALSE;
    }
    *nc = LIMX;
    *nl = LIMY;
    *mn = MAX_NIVEL;
    return TRUE;
}

/*+-------------------------------------------------------------------------+
  | Rotina que grava o arquivo da imagem em formato PGM ASCII.              |
  | PARAMETROS:                                                             |
  | I    = ponteiro para o vetor imagem (entra).                            |
  | nome = nome do arquivo (entra).                                         |
  | nl   = numero de linhas (entra).                                        |
  | nc   = numero de colunas (entra).                                       |
  | mn   = maximo nivel de cinza (entra).                                   |
  +-------------------------------------------------------------------------+*/
void grava_imagem_pgm(imagem I, char *nome, int nl, int nc, int mn) {
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    } else {
        fputs("P2\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d  %d\n", nc, nl);
        fprintf(arq, "%d\n", mn);
        valores_por_linha = 16;
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++) {
                x = *(I + i * nc + j);
                fprintf(arq, "%3d ", x);
                k++;
                if (k > valores_por_linha) {
                    fprintf(arq, "\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}

void calculaHistograma(imagem I, histograma H, int nl, int nc, int mn) {
    int i, j;
    for (i = 0; i <= mn; i++)
        H[i] = 0;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++) {
            int nivel = I[i * nc + j];
            H[nivel]++;
        }
}

void mostraHistograma(histograma H, int mn) {
    int i, soma = 0;
    for (i = 0; i <= mn; i++) {
        printf("%3d->%3d\n", i, H[i]);
        soma += H[i];
    }
    printf("\nSoma = %d\n", soma);
}

/*
 * Duas regras de tres:
 *   MaiorHisto - 30% de nl
 *   H[i] - h
 * 
 *   mn - 20% de nc
 *   i   - x
 */
void desenhaHistograma(imagem I, imagem O, histograma H, int nl, int nc, int mn) {
    int i, j,  maior = H[0];
    int x, y, h;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
            O[i*nc+j] = I[i*nc+j];
    for (i = 1; i < mn + 1; i++)
        if (maior < H[i])
            maior = H[i];
    for (y = nl - 10; y > 0.7 * nl - 10; y--)
        for (x = 0; x < 0.2 * nc; x++)
            if (O[y * nc + x + 10] >= 30)
                O[y * nc + x + 10] -= 30;
            else
                O[y * nc + x + 10] += 0;
    for (i = 0; i < mn + 1; i++) {
        h = (float) (0.3 * nl * H[i]) / maior;
        x = (float) (0.2 * nc * i) / mn;
        //printf ("[%d %d]", h, x);
        for (y = nl - 10; y > nl - (h + 10); y--) {
            //printf ("%d,%d ", y, x);
            O[y * nc + x + 10] = 255;
        }
        //puts("");
    }
}

void equalizaHistograma(imagem I, imagem O, histograma H, int nl, int nc, int mn) {
    float PF[mn+1];
    float GK[mn+1];
    int GKArred[mn+1];
    int i, j;
    for (i = 0; i < mn+1; i++)
        PF[i] = (float)H[i] / (nl * nc);
    GK[0] = PF[0];
    for (i = 1; i < mn+1; i++)
        GK[i] = GK[i-1] + PF[i];
    for (i = 0; i < mn+1; i++) {
        GKArred[i] = GK[i] * mn;
        // printf("GKArred[%d]= %d\n", i, GKArred[i]);
    }
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++)
            O[i*nc+j] = GKArred[I[i*nc+j]];
}

void msg(char *s) {
    printf("\nHISTOGRAMA de uma imagem qualquer");
    printf("\n---------------------------------");
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
    histograma Histo;
    if (argc < 2)
        msg(argv[0]);
    else {
        OK = le_imagem_pgm(argv[1], &In, &nl, &nc, &mn);
        if (OK) {
            printf("\nHISTOGRAMA DE IMAGEM");
            info_imagem(argv[1], nl, nc, mn);
            aloca_memo(&Out, nl * nc);
            aloca_memo(&Histo, mn);
            calculaHistograma(In, Histo, nl, nc, mn);
            // mostraHistograma (Histo, mn);
            desenhaHistograma(In, Out, Histo, nl, nc, mn);
            strcpy(nome, argv[1]);
            strcat(nome, "-histograma-1.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            //sprintf(comando, "eog %s &", nome);
            //system(comando);
            
            equalizaHistograma (In, Out, Histo, nl, nc, mn);
            calculaHistograma (Out, Histo, nl, nc, mn);
            desenhaHistograma(Out, In, Histo, nl, nc, mn);
            strcpy(nome, argv[1]);
            strcat(nome, "-histograma-2.pgm");
            grava_imagem_pgm(In, nome, nl, nc, mn);
            sprintf(comando, "eog %s &", nome);
            system(comando);
            
            desaloca_memo(&In);
            desaloca_memo(&Out);
            desaloca_memo(&Histo);
        }
    }
    return 0;
}
