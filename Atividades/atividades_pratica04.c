
#include <stdio.h>
//GABRIEL SARAIVA DE GOUVEIA RA: 129145
//20/07 ATIVIDADE PRATICA 04
#define MAX_BUFFER 100
int leia_reg(char *BUFFER, int TAM, FILE *ENTRADA) {
    int comp_reg;
    fread(&comp_reg, sizeof(int), 1, ENTRADA);
    if (feof(ENTRADA))
        return 0;

    if (comp_reg < TAM) {
        fread(BUFFER, sizeof(char), comp_reg, ENTRADA);
        BUFFER[comp_reg] = '\0';
        return comp_reg;
    } else {
        return 0;
    }
}

void le_registros(FILE *NOME_ARQ){

    FILE *arq;
    arq = fopen(NOME_ARQ, "rb");

    if(arq == NULL){
        printf("ERRO");
    }

    char campo[31];
    int comp_reg;
    char buffer[MAX_BUFFER];
    comp_reg = leia_reg(buffer, MAX_BUFFER, arq);

    while(comp_reg > 0){
        campo = strtok(buffer, "|");
        while(campo != NULL){
            printf("Campo: %s", campo);
            campo = strtok(NULL, "|");
        }
        comp_reg = leia_reg(buffer, MAX_BUFFER, arq);
    }
    fclose(arq);
}
int main(){
    char nome_arquivo[21];
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome_arquivo);
    le_registros(nome_arquivo);
    return 0;
}
