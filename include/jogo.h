#ifndef JOGO_H
#define JOGO_H

#define TAMANHO_PALAVRA 5

// Declaração da lista de palavras
extern const char* palavras[];

// Estrutura para um nó da lista encadeada
typedef struct No {
    char letra;
    struct No* proximo;
} No;

// Estrutura da pilha
typedef struct {
    No* topo;
} Pilha;

// Estrutura do jogo
typedef struct {
    char palavra[TAMANHO_PALAVRA + 1];
    char palavra_mostrada[TAMANHO_PALAVRA * 2];
    Pilha letras_testadas;
    int tentativas;
} Jogo;

// Funções para inicializar e jogar
void iniciar_jogo(Jogo* jogo, int modo);
void gerar_palavra(char* palavra);
void verificar_letra(Jogo* jogo, char letra);
void empilhar(Pilha* pilha, char letra);
int contem(Pilha* pilha, char letra);
void exibir_estado(Jogo* jogo);
void limpar_tela();
void ocultar_entrada();
void mostrar_entrada();
void* entrada_usuario(void* arg);
void* verificar_palavra(void* arg);

#endif // JOGO_H
