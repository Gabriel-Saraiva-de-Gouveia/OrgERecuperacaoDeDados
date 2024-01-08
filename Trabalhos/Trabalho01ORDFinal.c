// Trabalho 01 - Organizacao e Recuperacao de dados
// Docente: Paulo Roberto de Oliveira  9895/32
// Discentes
//  {
//      Gabriel Saraiva RA: 129145
//      Eduardo Felippe RA: 129379
//  }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PIPE '|'
// Funcionalidades Principais do programa: Busca, Insercao e Remocao


//Funcão utilizada para a leitura do identificador de um registro
char * le_registro_todo(FILE *nome_arquivo, int byteOffSet){
    fseek(nome_arquivo, byteOffSet, SEEK_SET);
    char *registro_atual; //valor maximo estipulado para ler um registro
    int quant_pipes = 0; //variavel contadora que ira ajudar a ler o registro (registro possui 6 pipes "|")
    int i = 0;
    char caracter_atual_arq = fgetc(nome_arquivo);
    while(quant_pipes <= 6){
        registro_atual[i] = caracter_atual_arq;
            
        if (registro_atual[i] == PIPE){
            quant_pipes++;
        }
        caracter_atual_arq = fgetc(nome_arquivo);
 
    }
    registro_atual[i] = '\0'; 
    return registro_atual;
}
void le_registro_no_identificador(FILE *nome_arquivo, int byteOffset){
    char *campo_registro;
    fseek(nome_arquivo, byteOffset, SEEK_SET);
    int i = 0;
    char char_aquivo = fgetc(nome_arquivo);
    int tamanho_max_identificador = 5;
    while(char_aquivo != PIPE){
        if(i < tamanho_max_identificador){
            campo_registro[i] = char_aquivo;
            i++;
        }
        char_aquivo = fgetc(nome_arquivo);
    }
    campo_registro[i] = '\0';
}

void busca_registro(FILE *nome_arquivo, int *chave){
    int tamanho_atual;
    fseek(nome_arquivo, 4, SEEK_SET); // vai para o tamanho do primeiro registro
    fread(&tamanho_atual, sizeof(tamanho_atual), 1, nome_arquivo);
    
    int *chave_primaria;
    int byte_off_set = 0;
    int encontrou_registro = 0;
    while(encontrou_registro != 1){
        fseek(nome_arquivo, tamanho_atual+2, SEEK_SET);
        fread(chave_primaria, sizeof(chave_primaria), 1, nome_arquivo); //li a chave do primeiro registro para a chave principal
        char *registro_atual;
        if(chave == chave_primaria ){
            registro_atual = le_registro_todo(nome_arquivo, byte_off_set);
            printf("%s", registro_atual);
            encontrou_registro = 1;
        }else{
            byte_off_set += tamanho_atual;
            fseek(nome_arquivo, byte_off_set, SEEK_SET);
            fread(&tamanho_atual, sizeof(tamanho_atual), 1, nome_arquivo);
        }
    }   
    fclose(nome_arquivo);
}

int imprime_led(){
    int bfs_topo_da_led;
    int tamanho_proximo;

    FILE *arquivo_de_dados_modificado;

    if((arquivo_de_dados_modificado = fopen("dados.dat", "rb")) == NULL){
        printf("Erro na abertura do arquivo de dados\n");
        exit(EXIT_FAILURE);
    }
    int espacos_disponiveis = 0;
    int tamanho_reg_atual;
    while(fread(&bfs_topo_da_led, sizeof(int), 1, arquivo_de_dados_modificado) == 1){
        if(bfs_topo_da_led == '*'){

            bfs_topo_da_led++;
            fseek(arquivo_de_dados_modificado, bfs_topo_da_led, SEEK_SET);
            fread(&tamanho_reg_atual, sizeof(tamanho_reg_atual), 1, arquivo_de_dados_modificado); // estaremos lendo o tamanho do espacao disponivel
            
            tamanho_reg_atual++;
            fseek(arquivo_de_dados_modificado, tamanho_reg_atual, SEEK_SET);
            fread(&tamanho_proximo, sizeof(tamanho_proximo),1,arquivo_de_dados_modificado);

            if(tamanho_reg_atual >= tamanho_proximo) // A ideia seria comparar o tamanho do registro atual com o registro do proximo, faltou implementar do anterior...
            {
                printf("LED -> [offset: %d, tam: %d]\n", bfs_topo_da_led, tamanho_reg_atual);
            }else
            {
                printf("LED -> [offset: %d, tam: %d]\n", bfs_topo_da_led, tamanho_proximo);
            }
            espacos_disponiveis++;

        }else{
            bfs_topo_da_led = -1;
            printf("offset: -1\n");
            break;
        }
    }
    printf("Total: %d espacos disponiveis", espacos_disponiveis);
    return bfs_topo_da_led;

    // A ideia é procurar pelos registros sinalizados com * no arquivo, ou seja, os registros removidos lógicamente
}


void insere_registro(FILE *nome_arquivo, char *registro){
    int topo_led;

    if (topo_led == -1)
    {
        fseek(nome_arquivo, 0, SEEK_END);
        fwrite(registro, sizeof(char), 1, nome_arquivo);
        printf("Local: fim do arquivo\n");

    } else
    {
        topo_led = imprime_led();
        topo_led++;
        fseek(nome_arquivo, topo_led, SEEK_SET);
        fwrite(registro, sizeof(registro), 1, nome_arquivo);
        printf("Tamanho do espaco reutilzado: ... bytes\n");
        printf("Local: offset = %d bytes\n", topo_led);
    }

    fclose(nome_arquivo);


}

void remove_registro(FILE *nome_arquivo, int *chave_a_remover){
    // O propósito da funcao remover é sinalizar o local do registro, ou seja, remover logicamente o registro
    // sinalizando-o com um "*"
    int offset_atual = 4; // cabecalho
    int tamanho_reg;
    short encontrou_registro = 0; // variavel de parada do laco
    int *chave_primaria; // tamanho maximo estipulado para um identificador

    while(encontrou_registro != 1){
        fseek(nome_arquivo, offset_atual, SEEK_SET);
        fread(&tamanho_reg, sizeof(tamanho_reg), 1, nome_arquivo);

        offset_atual = offset_atual + 2; // para pular o primeiro tamanho e ir para o identificador
        fseek(nome_arquivo, offset_atual, SEEK_SET);
        fread(chave_primaria,sizeof(chave_primaria), 1, nome_arquivo);
       //le_registro_no_identificador(nome_arquivo, offset_atual);

        if(chave_a_remover == chave_primaria){
            printf("Registro removido!   (%d bytes)\n", &tamanho_reg);
            printf("Local: offsett = %d", &offset_atual);

            offset_atual = offset_atual - 1;
            fseek(nome_arquivo, offset_atual, SEEK_SET);
            fputs("*", nome_arquivo);
            // Registro sinalizado logicamente como removido!
            encontrou_registro = 1;
        }else{
            offset_atual = offset_atual + tamanho_reg;
            encontrou_registro = 0;
        }
            
    }

    fclose(nome_arquivo);

}

void executa_operacoes(char *nome_arquivo_operacoes) {
    char operacao;
    FILE *arquivo_operacoes;
    if ((arquivo_operacoes = fopen(nome_arquivo_operacoes, "rb")) == NULL) {
        printf("Erro na abertura do arquivo de operacoes\n");
        exit(EXIT_FAILURE);
    }

    FILE *arquivo_de_dados;

    if ((arquivo_de_dados = fopen("dados.dat", "rb+")) == NULL) {
        printf("Erro na abertura do arquivo de dados\n");
    }

    while (fread(&operacao, sizeof(operacao), 1, arquivo_operacoes) == 1) {
        if (operacao == 'b') {
            fseek(arquivo_operacoes, 1, SEEK_CUR);
            int *chave_primaria;
            fread(&chave_primaria, sizeof(chave_primaria), 1, arquivo_operacoes);
            busca_registro(arquivo_de_dados, chave_primaria);
            printf("Busca pelo registro de chave '%d'\n", chave_primaria);

        } else if (operacao == 'r') {
            int *remov_chave;
            fseek(arquivo_operacoes, 1, SEEK_CUR);
            fread(remov_chave, sizeof(remov_chave), 1, arquivo_operacoes);
            printf("Remocao do registro de chave %d", remov_chave);
            remove_registro(arquivo_de_dados, remov_chave);

        } else if (operacao == 'i') {
            int chave_registro;
            fseek(arquivo_operacoes, 1, SEEK_CUR);
            fread(&chave_registro, sizeof(chave_registro), 1, arquivo_operacoes);
            char *registro_atual;
            registro_atual = le_registro_todo(arquivo_de_dados, chave_registro);
            printf("Insercao do registro de chave %d", chave_registro);
            insere_registro(arquivo_de_dados, registro_atual);
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

        printf("Modo de impressao da LED ativado ...\n");
        // chamada da funcao que imprime as informacoes da led
        imprime_led();

    } else {

        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s -e nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);

    }

    return 0;
}
