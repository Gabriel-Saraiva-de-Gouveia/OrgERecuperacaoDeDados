#include <stdio.h>
#include <string.h>
//GABRIEL SARAIVA DE GOUVEIA RA: 129145
//17/07 ATIVIDADE PRATICA 02
int leia_campo(char str[], int size, FILE *ENTRADA) {
    int i = 0;
    char CH = fgetc(ENTRADA); // Use fgetc() para ler um caractere do arquivo
    while (CH != EOF && CH != '|') {
        if (i < size - 1) {
            str[i] = CH;
            i++;
        }
        CH = fgetc(ENTRADA); // Use fgetc() novamente para ler o próximo caractere
    }
    str[i] = '\0';
    return i;
}


void le_campos(char NOME_ARQ[21]) {
    FILE *arq;
    arq = fopen(NOME_ARQ, "r");
    int i = 1;
    char CAMPO[31];
    int COMP_CAMPO = 1;

    if(arq == NULL){
        printf("ERRO");
    }else{
        printf("Arquivo aberto com sucesso!\n");
        while(COMP_CAMPO > 0){
            COMP_CAMPO = leia_campo(CAMPO, 30, arq);
            fgets(CAMPO,0,arq);
            printf("campo #%d: %s\n", i,CAMPO);
            i++;
        }

    }
    fclose(arq);
}

int main() {
    char nome_arq[21];
    printf("Entre com o nome do arquivo a ser lido: ");
    scanf("%s", nome_arq);
    le_campos(nome_arq);

    return 0;
}
