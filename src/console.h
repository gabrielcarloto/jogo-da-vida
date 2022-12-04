#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define MOSTRA_CURSOR "\e[?25l"
#define ESCONDE_CURSOR "\e[?25h"
#define RESET "\033[0m"

/**
 * @brief Move o cursor para a posição (x, y) do console. Fonte:
 * https://stackoverflow.com/a/26423946
 *
 * @deprecated Use a função SetConsoleCursorPosition
 */
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

typedef struct
{
  int width;
  int height;
} Terminal_Size;

// https://cplusplus.com/articles/4z18T05o/
void ClearScreen();

/**
 * @brief Esconde/mostra o cursor no console. Adaptado de:
 * https://www.autoscripts.net/c-hide-cursor/
 *
 * @param show Se verdadeiro, mostra, do contrário esconde.
 */
void toggleCursor(int show);

/**
 * @brief Mede a largura e a altura (em número de caracteres disponíveis) do terminal rodando o programa.
 * Adaptado de: https://stackoverflow.com/a/12642749
 *
 * @param tsize Ponteiro para uma variável do tipo Terminal_Size
 */
void tamanhoTerminal(Terminal_Size *tsize);

/**
 * @brief Redimensiona a janela, centralizando-a no processo
 *
 * @param width Largura em pixels (se 0, a janela será maximizada)
 * @param height Altura em pixels (se 0, a janela será maximizada)
 */
void resizeWindow(int width, int height);

/**
 * @brief Configura o terminal para aceitar escapes, redimensiona a janela e adiciona título. Adaptado de:
 * https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/
 */
void setupConsole();

// Reseta o console para seu estado inicial
void resetaConsole();
#endif