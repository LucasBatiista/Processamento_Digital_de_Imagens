#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define IMAGEM1 "esferaRuido.bmp"
//#define IMAGEM2 "esferaRuidoCinza.bmp"
//#define IMAGEM3 "esferaRuidoMedia.bmp"

#define IMAGEM1 "flor.bmp"
#define IMAGEM2 "florCinza.bmp"
#define IMAGEM3 "florMedia.bmp"

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
} RegImagem;
//#pragma pack(pop)


  char ordenacao (char v1, char v2, char v3, char v4, char v5, char v6, char v7, char v8, char v9){
     
  return(v5);
   }

int main(){
    FILE *arq1, *arq2, *arq3;
    unsigned char **matr, **matg, **matb, **mata, **mat2, **mat3;
    unsigned char dado;
    int lar, alt, inicio, i, j;
    float L1, L2, L3;
    
    if((arq1 = fopen(IMAGEM1,"rb")) == NULL){
        printf("Arquivo nao encontrado\n");
        getche();
        exit(0);
    }

    arq2= fopen(IMAGEM2,"wb");
    arq3= fopen(IMAGEM3,"wb");

    fread(&RegImagem,sizeof(RegImagem),1,arq1);

    alt    = (int) (RegImagem.biheight);
    lar    = (int) (RegImagem.biwidth);
    inicio = (int) (RegImagem.begin_img);

    matr = malloc(sizeof(unsigned char*[alt]));
    matg = malloc(sizeof(unsigned char*[alt]));
    matb = malloc(sizeof(unsigned char*[alt]));
    mata = malloc(sizeof(unsigned char*[alt]));

    mat2 = malloc(sizeof(unsigned char*[alt]));
    mat3 = malloc(sizeof(unsigned char*[alt]));
  
    for(j = 0; j < alt; j++){
        matr[j] = malloc(sizeof(unsigned char[lar]));
        matg[j] = malloc(sizeof(unsigned char[lar]));
        matb[j] = malloc(sizeof(unsigned char[lar]));
        mata[j] = malloc(sizeof(unsigned char[lar]));

        mat2[j] = malloc(sizeof(unsigned char[lar]));
        mat3[j] = malloc(sizeof(unsigned char[lar]));
    }

    printf("Matrizes Alocadas\n");

//=== Atribuicao dos valores p/as Matrizes
    fseek (arq1, inicio, SEEK_SET);

    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++){
            fread(&dado,sizeof(unsigned char),1,arq1);
            mata[i][j] = dado;
            fread(&dado,sizeof(unsigned char),1,arq1);
            matg[i][j] = dado;
            fread(&dado,sizeof(unsigned char),1,arq1);
            matr[i][j] = dado;
            fread(&dado,sizeof(unsigned char),1,arq1);
            matb[i][j] = dado;
        }


//=== A T E N Ç Ã O: Operações nas Matrizes
//Cinza
    
    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++)
            mat2[i][j] = (matr[i][j] * 0.3) + (matg[i][j] * 0.59) + (matb[i][j] * 0.11);

//Preto e Branco        
 /*   for(i=0; i<alt; i++)
        for(j=0; j<lar; j++)
            if(mat2[i][j] <= 127)
               mat3[i][j] = 0; 
            else
               mat3[i][j] = 255;
*/            
            


//Filtro da Média
    /*for(i=0; i<alt; i++)
        for(j=0; j<lar; j++)
            mat3[i][j] = mat2[i][j];*/
           

    for(i=1; i<alt-1; i++)
        for(j=1; j<lar-1; j++){
            L1 = mat2[i-1][j-1] + mat2[i-1][j] + mat2[i-1][j+1];
            L2 =   mat2[i][j-1] +   mat2[i][j] +   mat2[i][j+1];
            L3 = mat2[i+1][j-1] + mat2[i+1][j] + mat2[i+1][j+1];          
            
          //mat3[i][j] = (ceil((L1 + L2 + L3)/9));
            mat3[i][j] = (ceil((L2)/3));
             }
          
        
//Filtro da Mediana
   for(i=1; i<alt-1; i++)
        for(j=1; j<lar-1; j++)
            mat3[i][j] = ordenacao(mat2[i-1][j-1], mat2[i-1][j], mat2[i-1][j+1],
                                  mat2[i][j-1],   mat2[i][j],   mat2[i][j+1],
                                  mat2[i+1][j-1], mat2[i+1][j], mat2[i+1][j+1]);


//=== Gravacao da nova Imagem
    fwrite(&RegImagem, sizeof(RegImagem), 1, arq2);
    fwrite(&RegImagem, sizeof(RegImagem), 1, arq3);

    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++){
            fwrite (&mat2[i][j], sizeof(unsigned char), 1, arq2);
            fwrite (&mat2[i][j], sizeof(unsigned char), 1, arq2);
            fwrite (&mat2[i][j], sizeof(unsigned char), 1, arq2);
            fwrite (&mat2[i][j], sizeof(unsigned char), 1, arq2);
        }

    for(i=0; i<alt; i++)
        for(j=0; j<lar; j++){
            fwrite (&mat3[i][j], sizeof(unsigned char), 1, arq3);
            fwrite (&mat3[i][j], sizeof(unsigned char), 1, arq3);
            fwrite (&mat3[i][j], sizeof(unsigned char), 1, arq3);
            fwrite (&mat3[i][j], sizeof(unsigned char), 1, arq3);
        }

    fclose(arq1);
    fclose(arq2);
    fclose(arq3);

    for(i=0; i<alt; i++){
        free(matr[i]);
        free(matg[i]);
        free(matb[i]);
        free(mata[i]);
        
        free(mat2[i]);
        free(mat3[i]);
    }

    free(matr);
    free(matg);
    free(matb);
    free(mata);
    
    free(mat2);
    free(mat3);

    printf("\nFim");
    getch();
    return 0;
}
