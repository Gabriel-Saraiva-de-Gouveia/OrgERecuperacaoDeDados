#include <stdio.h>
#include <string.h>
//GABRIEL SARAIVA DE GOUVEIA RA: 129145
//17/07 ATIVIDADE PRATICA 01
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

void escreve_campos(char NOME_ARQ[21]) {
    FILE *arq;
    arq = fopen(NOME_ARQ, "w");
    char SOBRENOME[30], NOME[20], ENDERECO[30], CIDADE[20], ESTADO[3], CEP[8];
    int comprimento_sobrenome = 1;

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.");
    } else {
        printf("Arquivo aberto com sucesso\n");
        while(comprimento_sobrenome > 0){
            printf("Digite o SOBRENOME: ");
            comprimento_sobrenome = input(SOBRENOME, 30);
            //if (strlen(SOBRENOME) == 0) break; //outro exemplo de condição de parada
            fputs(SOBRENOME, arq);
            fputc('|', arq);

            printf("Digite o NOME: ");
            input(NOME, 20);
            fputs(NOME, arq);
            fputc('|', arq);

            printf("Digite o ENDERECO: ");
            input(ENDERECO, 30);
            fputs(ENDERECO, arq);
            fputc('|', arq);

            printf("Digite a CIDADE: ");
            input(CIDADE, 20);
            fputs(CIDADE, arq);
            fputc('|', arq);

            printf("Digite o ESTADO: ");
            input(ESTADO, 3);
            fputs(ESTADO, arq);
            fputc('|', arq);

            printf("Digite o CEP: ");
            input(CEP, 9);
            fputs(CEP, arq);
            fputc('|', arq);
            // teste: printf("%d", comprimento_sobrenome)...funciona

        }
    }
    fclose(arq);
}


int main() {
    char nome_arquivo[21] = "atividade01.txt";
    escreve_campos(nome_arquivo);
    return 0;
}
/*OBS :os inputs são utilizados para coletar as informações do usuário e armazená-las em strings que, posteriormente,
são escritas em um arquivo de texto. Isso permite que os dados sejam persistidos
e possam ser utilizados ou consultados em outra ocasião.*/
