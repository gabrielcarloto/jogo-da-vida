#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "uteis.c"

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