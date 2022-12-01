#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "uteis.c"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define CHAR_CELULA_MORTA '.'
#define CHAR_CELULA_VIVA 'O'

#define APAGA_LINHA "\x1b[2K"
#define COMECO_LINHA_ANT "\x1b[1F"
#define MOSTRA_CURSOR "\e[?25l"
#define ESCONDE_CURSOR "\e[?25h"
#define RESET "\033[0m"
#define COR_AZUL "\033[34m"
#define COR_VERDE "\033[92m"
#define COR_CINZA "\033[90m"
#define COR_AMARELO "\033[93m"
#define COR_VERMELHO "\033[31m"
#define COR_PADRAO COR_VERDE

typedef struct
{
  int width;
  int height;
} Terminal_Size;

typedef enum
{
  LEFT,
  CENTER,
  RIGHT
} Sign_Alignment;

typedef struct
{
  Sign_Alignment alignment;
} Sign_Settings;

void apagaTela(int nl)
{
  int i;

  for (i = 0; i < nl; i++)
    printf("%s%s", APAGA_LINHA, COMECO_LINHA_ANT);
}

/**
 * Mede a largura e a altura (em número de caracteres disponíveis) do terminal rodando o programa
 *
 * @param tsize Ponteiro para uma variável do tipo Terminal_Size
 */
void tamanhoTerminal(Terminal_Size *tsize)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  // https://stackoverflow.com/a/12642749
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  tsize->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  tsize->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

static HANDLE stdoutHandle;
static DWORD outModeInit;

/**
 * @brief Configura o terminal para aceitar escapes. Fonte:
 * https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/
 */
void setupConsole()
{
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

int contStr(const char *firstArg, va_list args);

/**
 * Imprime uma "placa", ocupando todo o espaço disponível no terminal.
 * É necessário inserir um NULL ao final para não criar um loop infinito.
 *
 * @param settings Configurações de exibição
 * @param str Strings a serem impressas. A primeira é o título.
 */
void printSign(Sign_Settings settings, const char *str, ...)
{
  int i, j, k, len, signLen, strLines, totalLines, verticalAlignLines, halfVerticalLines, halfTitleSize;
  Terminal_Size tsize;
  va_list list, list2;

  if (settings.alignment < 0 || settings.alignment > 2)
  {
    fprintf(stderr, "O alinhamento escolhido (%d) não existe.\n", settings.alignment);
    exit(1);
  }

  system("cls");

  tamanhoTerminal(&tsize);

  va_start(list, str);
  va_copy(list2, list);

  strLines = contStr(str, list) - 1;
  totalLines = strLines > (tsize.height - 4) ? strLines + tsize.height : tsize.height;
  verticalAlignLines = (totalLines - strLines) - 2;
  halfVerticalLines = verticalAlignLines / 2;

  va_end(list);

  signLen = tsize.width - 4;

  // TITULO
  halfTitleSize = (tsize.width - strlen(str) - 2) / 2;

  for (i = 0; i < halfTitleSize; i++)
    printf("=");

  printf(" %s ", str);

  for (i = 0; i < halfTitleSize + (strlen(str) % 2); i++)
    printf("=");

  printf("\n");

  // ESPAÇO EM BRANCO
  for (i = 0; i < halfVerticalLines; i++)
    printf("= %*s =\n", signLen, "");

  // CONTEÚDO
  str = va_arg(list, char *);

  for (i = 0; i < totalLines - halfVerticalLines - 2; i++)
  {
    if (str)
    {
      /**
       * Se o número de caracteres na string for maior que a largura disponível na linha do terminal,
       * ele acaba criando um loop infinito. Essa variável calcula o número de linhas necessárias para
       * imprimir a string.
       */
      int dividedStrLines = ceil(strlen(str) / (float)signLen);

      if (dividedStrLines > 1)
      {
        int letters;
        char **splittedString = split(str, "", &letters);
        totalLines -= dividedStrLines - 1;
        for (j = 0; j < dividedStrLines; j++)
        {
          int start = signLen * j, end;

          letters -= signLen;
          end = letters < 0 ? strlen(str) : signLen * (j + 1);

          printf("= %*s", (settings.alignment == RIGHT) * (signLen - (end - start)), "");
          for (k = start; k < end; k++)
            printf("%s", splittedString[k]);

          printf("%*s =", (settings.alignment == LEFT || settings.alignment == CENTER) * (signLen - (end - start)), "");
        }

        desalocaMatriz(splittedString, letters);
      }
      else
      {
        if (settings.alignment != CENTER)
        {
          int availableSpace = signLen - strlen(str);
          int isLeftAlign = (settings.alignment == LEFT);
          printf("= %*s%s%*s =\n", !isLeftAlign * availableSpace, "", str, isLeftAlign * availableSpace, "");
        }
        else
        {
          int centerAlignSpaces = (signLen - strlen(str)) / 2;
          int leftSpace = centerAlignSpaces * 2 + strlen(str) == signLen ? centerAlignSpaces : centerAlignSpaces + 1;
          printf("= %*s%s%*s =\n", centerAlignSpaces, "", str, leftSpace, "");
        }
      }

      str = va_arg(list, char *);
    }
    // ESPAÇO EM BRANCO
    else
      printf("= %*s =\n", signLen, "");
  }

  va_end(list2);

  for (i = 0; i < tsize.width; i++)
    printf("=");

  printf("\n");
}

/* Conta o número de strings em uma va_list */
int contStr(const char *firstArg, va_list args)
{
  int cont = 0;
  char *arg = firstArg;

  while (arg)
  {
    cont++;
    arg = va_arg(args, char *);
  }

  return cont;
}

// essa função não deve estar disponível em outros arquivos
#define contStr NULL