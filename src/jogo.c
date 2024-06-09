#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // Para definir STDIN_FILENO
#include <termios.h>  // Para manipulação do terminal
#include "jogo.h"

#define NUM_PALAVRAS (sizeof(palavras) / sizeof(palavras[0]))

const char* palavras[] = {
    "piano", "salto", "amigo", "banco", "canil", 
    "dente", "flore", "gosto", "honra", "ideal"
};

sem_t sem_tentativa;
sem_t sem_verificacao;

// Função para gerar uma palavra aleatória
void gerar_palavra(char* palavra) {
    srand((unsigned int)time(NULL));
    size_t index = (size_t)rand() % NUM_PALAVRAS;
    strcpy(palavra, palavras[index]);
}

// Função para empilhar uma letra testada
void empilhar(Pilha* pilha, char letra) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->letra = letra;
    novo_no->proximo = pilha->topo;
    pilha->topo = novo_no;
}

// Função para verificar se uma letra já foi testada
int contem(Pilha* pilha, char letra) {
    No* atual = pilha->topo;
    while (atual != NULL) {
        if (atual->letra == letra) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

// Função para verificar a letra inserida pelo jogador
void verificar_letra(Jogo* jogo, char letra) {
    int encontrada = 0;
    for (int i = 0; i < TAMANHO_PALAVRA; i++) {
        if (jogo->palavra[i] == letra) {
            jogo->palavra_mostrada[i * 2] = letra;
            encontrada = 1;
        }
    }
    if (!encontrada) {
        empilhar(&jogo->letras_testadas, letra);
    }
}

// Função para exibir o estado atual do jogo
void exibir_estado(Jogo* jogo) {
    printf("Palavra: %s\n", jogo->palavra_mostrada);
    printf("Letras testadas: ");
    No* atual = jogo->letras_testadas.topo;
    while (atual != NULL) {
        printf("%c ", atual->letra);
        atual = atual->proximo;
    }
    printf("\nTentativas restantes: %d\n", jogo->tentativas);
}

// Função para limpar a tela
void limpar_tela() {
    printf("\033[H\033[J");  // Sequência de escape ANSI para limpar a tela
}

// Função para ocultar a entrada do usuário
void ocultar_entrada() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= (unsigned int)~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Função para mostrar a entrada do usuário
void mostrar_entrada() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Função que cuida da entrada do usuário em uma thread separada
void* entrada_usuario(void* arg) {
    Jogo* jogo = (Jogo*)arg;
    while (jogo->tentativas > 0) {
        limpar_tela(); // Limpa a tela antes de cada nova entrada
        exibir_estado(jogo);
        char input[100];
        printf("Digite uma letra%s: ", (jogo->tentativas == 1) ? " ou a palavra inteira" : "");
        scanf("%s", input);

        sem_wait(&sem_verificacao); // Espera pela verificação da tentativa anterior

        if (strlen(input) == 1) {
            char letra = tolower(input[0]);

            if (contem(&jogo->letras_testadas, letra) || strchr(jogo->palavra_mostrada, letra)) {
                printf("Você já testou essa letra. Tente outra.\n");
                sem_post(&sem_tentativa); // Libera a verificação para tentar novamente
                continue;
            }

            verificar_letra(jogo, letra);
            jogo->tentativas--;

        } else if (jogo->tentativas == 1 && strcmp(input, jogo->palavra) == 0) {
            printf("Parabéns! Você adivinhou a palavra: %s\n", jogo->palavra);
            exit(0);
        } else if (jogo->tentativas == 1) {
            printf("Que pena! Suas tentativas acabaram. A palavra era: %s\n", jogo->palavra);
            exit(0);
        } else {
            printf("Você só pode tentar adivinhar a palavra inteira na última tentativa.\n");
            sem_post(&sem_tentativa); // Libera a verificação para tentar novamente
            continue;
        }

        sem_post(&sem_tentativa); // Libera a verificação da tentativa
    }
    printf("Que pena! Suas tentativas acabaram. A palavra era: %s\n", jogo->palavra);
    return NULL;
}

// Função que cuida da verificação da palavra em uma thread separada
void* verificar_palavra(void* arg) {
    Jogo* jogo = (Jogo*)arg;
    while (jogo->tentativas > 0) {
        sem_wait(&sem_tentativa); // Espera por uma nova tentativa

        if (strchr(jogo->palavra_mostrada, '_') == NULL) {
            printf("Parabéns! Você adivinhou a palavra: %s\n", jogo->palavra);
            exit(0);
        }

        sem_post(&sem_verificacao); // Libera a entrada para a próxima tentativa
    }
    return NULL;
}

// Função para iniciar o jogo
void iniciar_jogo(Jogo* jogo, int modo) {
    gerar_palavra(jogo->palavra);
    jogo->palavra[TAMANHO_PALAVRA] = '\0';
    for (int i = 0; i < TAMANHO_PALAVRA; i++) {
        jogo->palavra_mostrada[i * 2] = '_';
        jogo->palavra_mostrada[i * 2 + 1] = ' ';
    }
    jogo->palavra_mostrada[TAMANHO_PALAVRA * 2 - 1] = '\0';
    jogo->tentativas = 6;
    jogo->letras_testadas.topo = NULL;

    sem_init(&sem_tentativa, 0, 1);
    sem_init(&sem_verificacao, 0, 1);

    pthread_t th_entrada, th_verificacao;
    pthread_create(&th_entrada, NULL, entrada_usuario, jogo);
    pthread_create(&th_verificacao, NULL, verificar_palavra, jogo);

    pthread_join(th_entrada, NULL);
    pthread_join(th_verificacao, NULL);

    sem_destroy(&sem_tentativa);
    sem_destroy(&sem_verificacao);
}
