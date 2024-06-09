#include <stdio.h>
#include "jogo.h"

int main() {
    int modo;
    printf("Bem-vindo ao Jogo da Forca!\n");
    printf("Escolha o modo de jogo:\n");
    printf("1. Jogar contra a máquina\n");
    printf("2. Jogar com um amigo\n");
    printf("Digite o número correspondente ao modo desejado: ");
    scanf("%d", &modo);

    if (modo != 1 && modo != 2) {
        printf("Modo inválido!\n");
        return 1;
    }

    Jogo jogo;
    iniciar_jogo(&jogo, modo);

    return 0;
}
