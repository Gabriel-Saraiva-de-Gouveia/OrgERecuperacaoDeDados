// Trabalho 02 - Organizacao e Recuperacao de dados 9895/32
// Implementação - Árvore B (Insersão e Busca)
// Docente: Paulo Roberto de Oliveira
// Discentes
//  {
//      Gabriel Saraiva RA: 129145
//      Eduardo Felippe RA: 129379
//  }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 5
#define NULO -1
#define TAMBUFFER 500
#define TAM_MAX_REG 256

typedef struct {
    int NUM_CHAVES;
    char CHAVES[ORDEM - 1];
    int FILHOS[ORDEM];
} PAGINA;

typedef struct {
    int rrn_raiz;
} CABECALHO;

FILE *arvb;

short leia_reg(char * buffer, int tam, FILE * arq){
    short comp_reg;

    if (fread(&comp_reg, sizeof(comp_reg), 1, arq) == 0) {
        return 0;
    }
    if (comp_reg < tam) {
        fread(buffer, sizeof(char), comp_reg, arq);
        buffer[comp_reg] = '\0';
        return comp_reg;
    } else {
        printf("Buffer overflow\n");
        return 0;
    }
}

int le_pagina(int rrn, PAGINA *pag) {
    fseek(arvb, sizeof(CABECALHO) + rrn * sizeof(PAGINA), SEEK_SET);
    return fread(pag, sizeof(PAGINA), 1, arvb);
}

int escreve_pagina(int rrn, PAGINA *pag) {
    fseek(arvb, sizeof(CABECALHO) + rrn * sizeof(PAGINA), SEEK_SET);
    return fwrite(pag, sizeof(PAGINA), 1, arvb);
}

int novo_rrn() {
    fseek(arvb, 0, SEEK_END);
    return ftell(arvb) / sizeof(PAGINA);
}

void inicializa_pagina(PAGINA *pag) {
    pag->NUM_CHAVES = 0;
    for (int i = 0; i < ORDEM - 1; i++) {
        pag->CHAVES[i] = NULO;
        pag->FILHOS[i] = NULO;
    }
    pag->FILHOS[ORDEM - 1] = NULO;
}

int busca_na_pagina(int chave, PAGINA pag, int *pos) {
    int i = 0;
    while (i < pag.NUM_CHAVES && chave > pag.CHAVES[i])
        i++;

    *pos = i;

    if (i < pag.NUM_CHAVES && chave == pag.CHAVES[i])
        return 1;
    else
        return 0;
}

int busca(int rrn, int chave, int *rrn_encontrado, int *pos_encontrada) {
    if (rrn == NULO) {
        return 0;
    }

    PAGINA pag;
    if (le_pagina(rrn, &pag)) {
        int pos;
        if (busca_na_pagina(chave, pag, &pos)) {
            *rrn_encontrado = rrn;
            *pos_encontrada = pos;
            return 1;
        } else {
            return busca(pag.FILHOS[pos], chave, rrn_encontrado, pos_encontrada);
        }
    }

    return 0;
}

void insere_na_pagina(int chave, int filho_d, char chaves[], int filhos[], int *num_chaves) {
    int i = *num_chaves;
    while (i > 0 && chave < chaves[i - 1]) {
        chaves[i] = chaves[i - 1];
        filhos[i + 1] = filhos[i];
        i--;
    }

    chaves[i] = chave;
    filhos[i + 1] = filho_d;
    (*num_chaves)++;
}

void divide(int chave_i, int rrn_i, PAGINA *pag, int *chave_pro, int *filho_d_pro, PAGINA *novapag) {
    PAGINA pagaux;
    int chaves_aux[ORDEM];
    int filhos_aux[ORDEM + 1];
    int i, j;
    int meio = ORDEM / 2;

    inicializa_pagina(novapag);

    // Copia os elementos de pag para pagaux
    memcpy(&pagaux, pag, sizeof(PAGINA));

    // Insere chave_i e rrn_i na posição apropriada em pagaux
    insere_na_pagina(chave_i, rrn_i, pagaux.CHAVES, pagaux.FILHOS, &pagaux.NUM_CHAVES);

    // Copia chaves e filhos de pagaux para chaves_aux e filhos_aux
    for (i = 0, j = 0; i < ORDEM; i++, j++) {
        if (j == meio) j++;
        chaves_aux[i] = pagaux.CHAVES[j];
        filhos_aux[i + 1] = pagaux.FILHOS[j + 1];
    }

    // Inicializa pag e novapag
    inicializa_pagina(pag);
    inicializa_pagina(novapag);

    // Copia as primeiras metades de chaves_aux e filhos_aux para pag
    for (i = 0; i < meio; i++) {
        pag->CHAVES[i] = chaves_aux[i];
        pag->FILHOS[i] = filhos_aux[i];
        pag->FILHOS[i + 1] = NULO;
        pag->NUM_CHAVES++;
    }

    // Copia as segundas metades de chaves_aux e filhos_aux para novapag
    for (i = meio, j = 0; i < ORDEM; i++, j++) {
        novapag->CHAVES[j] = chaves_aux[i];
        novapag->FILHOS[j] = filhos_aux[i];
        novapag->FILHOS[j + 1] = NULO;
        novapag->NUM_CHAVES++;
    }

    // Define chave_pro e filho_d_pro para a chamada recursiva
    *chave_pro = chaves_aux[meio];
    *filho_d_pro = novo_rrn();
}

void insere(int chave, int rrn) {
    int chave_pro, filho_d_pro;
    PAGINA pag, novapag;

    if (busca(0, chave, NULL, NULL)) {
        printf("Chave %d ja existe na arvore.\n", chave);
        return;
    }

    // Inicializa a nova raiz se necessário
    int rrn_raiz;
    if (fread(&rrn_raiz, sizeof(int), 1, arvb) != 1 || rrn_raiz == NULO) {
        rrn_raiz = novo_rrn();
        escreve_pagina(rrn_raiz, &novapag);
    }

    if (le_pagina(rrn_raiz, &pag)) {
        if (pag.NUM_CHAVES < ORDEM - 1) {
            // Se a página tem espaço, insira a chave
            insere_na_pagina(chave, rrn, pag.CHAVES, pag.FILHOS, &pag.NUM_CHAVES);
            escreve_pagina(rrn_raiz, &pag);
        } else {
            // Se a página está cheia, divida-a
            divide(chave, rrn, &pag, &chave_pro, &filho_d_pro, &novapag);
            escreve_pagina(rrn_raiz, &pag);
            escreve_pagina(filho_d_pro, &novapag);

            // Atualiza a raiz se necessário
            if (chave_pro != NULO) {
                PAGINA newraiz;
                inicializa_pagina(&newraiz);
                insere_na_pagina(chave_pro, filho_d_pro, newraiz.CHAVES, newraiz.FILHOS, &newraiz.NUM_CHAVES);
                newraiz.FILHOS[0] = rrn_raiz;
                escreve_pagina(rrn_raiz, &newraiz);
                fseek(arvb, 0, SEEK_SET);
                fwrite(&rrn_raiz, sizeof(int), 1, arvb);
            }
        }
    } else {
        printf("Erro ao ler a raiz da arvore.\n");
    }
}

void imprime(int rrn) {
    if (rrn != NULO) {
        PAGINA pag;
        if (le_pagina(rrn, &pag)) {
            printf("[");
            for (int i = 0; i < pag.NUM_CHAVES; i++) {
                printf("%d", pag.CHAVES[i]);
                if (i < pag.NUM_CHAVES - 1) {
                    printf(", ");
                }
            }
            printf("]");

            for (int i = 0; i <= pag.NUM_CHAVES; i++) {
                imprime(pag.FILHOS[i]);
            }
        } else {
            printf("Erro ao ler a pagina %d.\n", rrn);
        }
    }
}
void executa_operacoes(char *nome_arquivo_operacoes) {
    char operacao;
    FILE *arquivo_operacoes;
    if ((arquivo_operacoes = fopen(nome_arquivo_operacoes, "rb")) == NULL) {
        printf("Erro na abertura do arquivo de operacoes\n");
        exit(EXIT_FAILURE);
    }

    FILE *arquivo_de_dados;

    if ((arquivo_de_dados = fopen("dados.dat", "r+b")) == NULL) {
        printf("Erro na abertura do arquivo de dados\n");
    }

    while (fread(&operacao, sizeof(operacao), 1, arquivo_operacoes) == 1) {
        if (operacao == 'b') {
            fseek(arquivo_operacoes, 1, SEEK_CUR);
            int *chave_primaria;
            fread(&chave_primaria, sizeof(chave_primaria), 1, arquivo_operacoes);
            busca(arquivo_de_dados, chave_primaria);
            printf("Busca pelo registro de chave '%d'\n", chave_primaria);
        } else if (operacao == 'i') {
            int chave_registro;
            fseek(arquivo_operacoes, 1, SEEK_CUR);
            fread(&chave_registro, sizeof(chave_registro), 1, arquivo_operacoes);
            char *registro_atual;
            char * buffer;
            registro_atual = leia_reg(buffer,TAMBUFFER, chave_registro);
            printf("Insercao do registro de chave %d", chave_registro);
            insere(chave_registro, 1);
        }
    }

    fclose(arquivo_de_dados);
    fclose(arquivo_operacoes);
}

int main(int argc, char *argv[]) {

    if (argc == 3 && strcmp(argv[1], "-e") == 0) {

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        // chamada da funcao que executa o arquivo de operacoes
        // o nome do arquivo de operacoes estara armazenado na variavel argv[2]
        executa_operacoes(argv[2]);

    } else if (argc == 2 && strcmp(argv[1], "-p") == 0) {

    } else {

        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s -e nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);

    }

    return 0;
}

