#include <windows.h>
#include <winuser.h>
#include <assert.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define MOSTRA_CURSOR "\e[?25l"
#define ESCONDE_CURSOR "\e[?25h"
#define RESET "\033[0m"

// https://stackoverflow.com/a/26423946
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

typedef enum
{
  SETA_CIMA = 72,
  SETA_BAIXO = 80,
  SETA_ESQUERDA = 75,
  SETA_DIREITA = 7,
  TECLA_ENTER = 13,
  TECLA_ESC = 27,
  TECLA_W = 119,
  TECLA_W_CAPS = 87,
  TECLA_A = 97,
  TECLA_A_CAPS = 65,
  TECLA_S = 115,
  TECLA_S_CAPS = 83,
  TECLA_D = 100,
  TECLA_D_CAPS = 68,
  NUM_ZERO = 48
} INPUTS;

typedef struct
{
  int width;
  int height;
} Terminal_Size;

/**
 * @brief Esconde/mostra o cursor no console. Adaptado de:
 * https://www.autoscripts.net/c-hide-cursor/
 *
 * @param show Se verdadeiro, mostra, do contrário esconde.
 */
void toggleCursor(int show)
{
  assert(show == 0 || show == 1);

#ifdef _WIN32
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(handle, &cci);
  cci.bVisible = show;
  SetConsoleCursorInfo(handle, &cci);
#elif __linux__
  printf("%s", show ? MOSTRA_CURSOR : ESCONDE_CURSOR);
#endif // Windows/Linux
}

/**
 * Mede a largura e a altura (em número de caracteres disponíveis) do terminal rodando o programa.
 * Adaptado de: https://stackoverflow.com/a/12642749
 *
 * @param tsize Ponteiro para uma variável do tipo Terminal_Size
 */
void tamanhoTerminal(Terminal_Size *tsize)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  tsize->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  tsize->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

/**
 * @brief Redimensiona a janela, centralizando-a no processo
 *
 * @param width Largura em pixels (se 0, a janela será maximizada)
 * @param height Altura em pixels (se 0, a janela será maximizada)
 */
void resizeWindow(int width, int height)
{
  DWORD scrWidth = GetSystemMetrics(SM_CXSCREEN), scrHeight = GetSystemMetrics(SM_CYSCREEN);
  HWND hWnd = GetConsoleWindow();

  assert(width >= 0 && height >= 0);

  if (width && height)
    MoveWindow(hWnd, (scrWidth - width) / 2, (scrHeight - height) / 2, width, height, TRUE);
  else
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
}

/**
 * @brief Configura o terminal para aceitar escapes. Adaptado de:
 * https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/
 */
void setupConsole()
{
  HANDLE stdoutHandle;
  DWORD outModeInit;
  DWORD outMode = 0;
  stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

  if (stdoutHandle == INVALID_HANDLE_VALUE)
    exit(GetLastError());

  if (!GetConsoleMode(stdoutHandle, &outMode))
    exit(GetLastError());

  outModeInit = outMode;

  outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  if (!SetConsoleMode(stdoutHandle, outMode))
    exit(GetLastError());

  system("cls");
  SetConsoleTitle("Jogo da Vida de Conway");
  resizeWindow(600, 400);
  toggleCursor(0);
}

void resetaConsole()
{
  toggleCursor(1);
  printf("%s", RESET);
}

/**
 * @brief Verifica se o valor input é um dos valores definidos em INPUTS
 *
 * @param input Valor coletado através da função getch
 * @return int
 */
int verificaInput(INPUTS input, int numOpcoes)
{
  return (
      input == SETA_CIMA ||
      input == SETA_BAIXO ||
      input == SETA_DIREITA ||
      input == SETA_ESQUERDA ||
      input == TECLA_ENTER ||
      input == TECLA_ESC ||
      input == TECLA_W ||
      input == TECLA_W_CAPS ||
      input == TECLA_A ||
      input == TECLA_A_CAPS ||
      input == TECLA_S ||
      input == TECLA_S_CAPS ||
      input == TECLA_D ||
      input == TECLA_D_CAPS ||
      input > NUM_ZERO && input < numOpcoes + NUM_ZERO + 1);
}