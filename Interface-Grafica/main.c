#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Lista de palavras possíveis
const char *word_list[] = {"EXEMPLO", "LINGUAGEM", "PROGRAMACAO", "DESENVOLVIMENTO", "COMPUTADOR", "TECNOLOGIA"};
const int num_words = sizeof(word_list) / sizeof(word_list[0]);

char word[20]; // Palavra a ser adivinhada
char guessed[20]; // Letras corretas adivinhadas
int attempts = 6; // Número de tentativas

void pickRandomWord() {
    srand(time(NULL));
    int index = rand() % num_words;
    strcpy(word, word_list[index]);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Failed to create surface: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

void renderWord(SDL_Renderer *renderer, TTF_Font *font) {
    char display[20] = "";
    for (int i = 0; i < strlen(word); i++) {
        if (strchr(guessed, word[i])) {
            strncat(display, &word[i], 1);
        } else {
            strcat(display, "_");
        }
        strcat(display, " ");
    }
    renderText(renderer, font, display, 50, 50);
}

void renderAttempts(SDL_Renderer *renderer, TTF_Font *font) {
    char text[50];
    sprintf(text, "Tentativas restantes: %d", attempts);
    renderText(renderer, font, text, 50, 100);
}

void renderGameStatus(SDL_Renderer *renderer, TTF_Font *font, bool allow_guess) {
    if (attempts == 1 && !strchr(guessed, '_')) {
        renderText(renderer, font, "Última tentativa! Chute a palavra!", 50, 150);
    } else if (allow_guess) {
        renderText(renderer, font, "Pressione ENTER para chutar a palavra!", 50, 150);
    }
}

void checkVictoryOrLoss(bool *running) {
    if (strcmp(word, guessed) == 0) {
        printf("Você ganhou! A palavra era: %s\n", word);
        *running = false;
    } else if (attempts <= 0) {
        printf("Você perdeu! A palavra era: %s\n", word);
        *running = false;
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Jogo da Forca", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    const char *font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"; // Caminho da fonte no sistema
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    pickRandomWord();
    bool running = true;
    bool allow_guess = false;
    SDL_Event event;
    memset(guessed, 0, sizeof(guessed));

    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderWord(renderer, font);
        renderAttempts(renderer, font);
        renderGameStatus(renderer, font, allow_guess);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                char guessedChar = event.key.keysym.sym;
                if (allow_guess && event.key.keysym.sym == SDLK_RETURN) {
                    if (strcmp(word, guessed) == 0) {
                        printf("Você ganhou! A palavra era: %s\n", word);
                    } else {
                        printf("Você perdeu! A palavra era: %s\n", word);
                    }
                    running = false;
                } else if (strchr(word, guessedChar)) {
                    strncat(guessed, &guessedChar, 1);
                } else {
                    attempts--;
                }
            }
        }

        if (attempts == 1 && !strchr(guessed, '_')) {
            allow_guess = true;
        }

        checkVictoryOrLoss(&running);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
