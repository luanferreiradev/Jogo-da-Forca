# Jogo da Forca 🎮

Um simples jogo da forca implementado em C, onde os jogadores podem escolher entre dois modos de jogo: jogar contra a máquina ou jogar com um amigo.

## Como Jogar

1. Clone este repositório:
   ```bash
   git clone https://github.com/seu-usuario/jogo-da-forca.git
   ```

2. Navegue até o diretório do projeto:
   ```bash
   cd jogo-da-forca
   ```

3. Compile o jogo:
   ```bash
   gcc -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0 -Iinclude src/main.c src/jogo.c -o build/Debug/jogo -lpthread
   ```

4. Execute o jogo:
   ```bash
   ./build/Debug/jogo
   ```

## Modos de Jogo

### Jogar contra a Máquina

Neste modo, a máquina escolherá uma palavra aleatória para o jogador adivinhar.

### Jogar com um Amigo

Neste modo, um jogador insere uma palavra e o outro jogador tenta adivinhar.

## Como Compilar Manualmente

Caso prefira compilar manualmente, você pode utilizar o seguinte comando:

```bash
gcc -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0 -Iinclude src/main.c src/jogo.c -o build/Debug/jogo -lpthread
```

## Licença

Este projeto é licenciado sob a Licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## Problemas com o VSCode

Se você encontrar problemas ao compilar o projeto no VSCode, pode ser necessário ajustar o arquivo `tasks.json` para se parecer com o seguinte:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "Compilar Projeto",
            "command": "/usr/bin/gcc",
            "args": [
                "-Wall",
                "-Wextra",
                "-Wpedantic",
                "-Wshadow",
                "-Wformat=2",
                "-Wcast-align",
                "-Wconversion",
                "-Wsign-conversion",
                "-Wnull-dereference",
                "-g3",
                "-O0",
                "-Iinclude",
                "src/main.c",
                "src/jogo.c",
                "-o",
                "build/Debug/jogo",
                "-lpthread"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Compilação do projeto de jogo da forca."
        }
    ]
}
```