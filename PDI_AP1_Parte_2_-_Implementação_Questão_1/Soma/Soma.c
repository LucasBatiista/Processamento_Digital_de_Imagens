#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IMAGEM1 "somaP_1.bmp"
#define IMAGEM2 "somaP_2.bmp"
#define IMAGEM3 "SOMAS.bmp"
#pragma pack(push, 1)

struct{
    char      assinatura[2];
    int       sizefile;
    short int reserved1;
    short int reserved2;
    int       begin_img;

    int       sizeheader; //40bytes
    int       biwidth;
    int       biheight;
    short int planes; //default=1
    short int bpp;
    int       compression;
    int       size;
    int       biXPelsPerMeter; //resolucao horizontal
    int       biYPelsPerMeter; //resolucao vertical
    int       biClrUsed;
    int       biClrImportant;

    //Paleta
    unsigned char r[256];
    unsigned char g[256];
    unsigned char b[256];
    unsigned char alpha[256];
} RegImagem;
//#pragma pack(pop)


int main(){
    FILE *arq1, *arq2, *arq3;
    unsigned char **mat1, **mat2, **mat3;
    unsigned char dado;
    int lar, alt, inicio, i, j, soma;

    if((arq1 = fopen(IMAGEM1,"rb")) == NULL){
        printf("Arquivo nao encontrado\n");
        getche();
        exit(0);
    }

    if((arq2 = fopen(IMAGEM2,"rb")) == NULL){
        printf("Arquivo nao encontrado\n");
        getche();
        exit(0);
    }

    arq3 = fopen(IMAGEM3,"wb");

    fread(&RegImagem,sizeof(RegImagem),1,arq1);

    alt    = (int) (RegImagem.biheight);
    lar    = (int) (RegImagem.biwidth);
    inicio = (int) (RegImagem.begin_img);

    mat1 = malloc(sizeof(unsigned char*[alt]));
    mat2 = malloc(sizeof(unsigned char*[alt]));
    mat3 = malloc(sizeof(unsigned char*[alt]));

    for(j = 0; j < alt; j++){
        mat1[j] = malloc(sizeof(unsigned char[lar]));
        mat2[j] = malloc(sizeof(unsigned char[lar]));
        mat3[j] = malloc(sizeof(unsigned char[lar]));
    }

    printf("Matrizes Alocadas\n");

//=== Atribuicao dos valores p/as Matrizes
    fseek (arq1, inicio, SEEK_SET);
    fseek (arq2, inicio, SEEK_SET);

    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++){
            fread(&dado,sizeof(unsigned char),1,arq1);
            mat1[i][j] = dado;
        }

    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++){
            fread(&dado,sizeof(unsigned char),1,arq2);
            mat2[i][j] = dado;
        }

//=== A T E N Ç Ã O: Operações nas Matrizes
    for(i=1; i<alt-1; i++)
        for(j=1; j<lar-1; j++){
           // if( (mat1[i][j] > 100) || (mat2[i][j] > 100) ) 
               mat3[i][j] = (mat1[i][j]+mat2[i][j]);
        }

//=== FIM

//=== Gravacao da nova Imagem
    fwrite(&RegImagem, sizeof(RegImagem), 1, arq3);

    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++)
            fwrite (&mat3[i][j], sizeof(unsigned char), 1, arq3);

    fclose(arq1);
    fclose(arq2);
    fclose(arq3);

    for(i=0; i<alt; i++){
        free(mat1[i]);
        free(mat2[i]);
        free(mat3[i]);
    }

    free(mat1);
    free(mat2);
    free(mat3);

    printf("\nFim");
    getch();
    return 0;
}
