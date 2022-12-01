#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define MOSTRA_CURSOR "\e[?25l"
#define ESCONDE_CURSOR "\e[?25h"

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
 * @brief Configura o terminal para aceitar escapes. Fonte:
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
}