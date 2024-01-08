#include <stdio.h>
#include <string.h>
//GABRIEL SARAIVA DE GOUVEIA RA: 129145
//20/07 ATIVIDADE PRATICA 03
#define MAX_BUFFER 100 //definir o tamanho total da variável BUFFER
#define NUM_CAMPOS 6
int input(char str[], int size) {
    int i = 0;
    char c = getchar();
    while (c != '\n' && c != EOF && i < size - 1) {
        str[i] = c;
        i++;
        c = getchar();
    }
    str[i] = '\0';
    return i;
}

void escreve_registros (char NOME_ARQ[21]){
    FILE *arq;
    arq = fopen(NOME_ARQ, "wb");

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
    }

    char campo[31];
    int comprimento;
    char buffer[MAX_BUFFER];

    printf("Digite o sobrenome: ");
    scanf("%s", campo);

    comprimento = input(campo, 31);

    while (comprimento > 0) {
        buffer[0] = '\0';
        strcat(buffer, campo);
        strcat(buffer, "|");

        for (int i = 0; i < NUM_CAMPOS; i++) {
            printf("Digite o campo %d: ", i + 1);
            scanf("%s", campo);
            strcat(buffer, campo);
            strcat(buffer, "|");
        }

        int comp_reg = strlen(buffer);
        fwrite(&comp_reg, sizeof(int), 1, arq);
        fwrite(buffer, sizeof(char), comp_reg, arq);

        printf("Digite o proximo sobrenome: ");
        scanf("%s", campo);
        comprimento = input(campo, 31);
    }

    fclose(arq);
}

int main(){
    char nome_arquivo[21] = "atividade03.bin";
    escreve_registros(nome_arquivo);
    return 0;
}
