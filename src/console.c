#include <windows.h>
#include <winuser.h>
#include <assert.h>

#include "console.h"
#include "interface.h"

void ClearScreen()
{
  HANDLE hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD count;
  DWORD cellCount;
  COORD homeCoords = {0, 0};

  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdOut == INVALID_HANDLE_VALUE)
    return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
    return;
  cellCount = csbi.dwSize.X * csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
          hStdOut,
          (TCHAR)' ',
          cellCount,
          homeCoords,
          &count))
    return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
          hStdOut,
          csbi.wAttributes,
          cellCount,
          homeCoords,
          &count))
    return;

  /* Move the cursor home */
  SetConsoleCursorPosition(hStdOut, homeCoords);
}

void toggleCursor(int show)
{
  assert(show == 0 || show == 1);

  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(handle, &cci);
  cci.bVisible = show;
  SetConsoleCursorInfo(handle, &cci);
}

void tamanhoTerminal(Terminal_Size *tsize)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  tsize->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  tsize->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void resizeWindow(int width, int height)
{
  DWORD scrWidth = GetSystemMetrics(SM_CXSCREEN), scrHeight = GetSystemMetrics(SM_CYSCREEN);
  HWND hWnd = GetConsoleWindow();

  assert(width >= 0 && height >= 0);

  if (width && height)
  {
    if (width >= scrWidth - scrWidth / 10 || height >= scrHeight - scrHeight / 10)
    {
      ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    }
    else
    {
      if (width < MIN_LARGURA_TELA)
        width = MIN_LARGURA_TELA;
      if (height < MIN_ALTURA_TELA)
        height = MIN_ALTURA_TELA;

      MoveWindow(hWnd, (scrWidth - width) / 2, (scrHeight - height) / 2, width, height, TRUE);
    }
  }
  else
  {
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
  }

  toggleCursor(0);
}

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
  resizeWindow(MIN_LARGURA_TELA, MIN_ALTURA_TELA);
  toggleCursor(0);
}

void resetaConsole()
{
  toggleCursor(1);
  printf("%s", RESET);
}
