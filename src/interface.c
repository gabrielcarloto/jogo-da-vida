#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "uteis.h"
#include "celulas.h"
#include "console.h"
#include "interface.h"

#define CHAR_CELULA_MORTA '.'
#define CHAR_CELULA_VIVA 'O'

void imprimeMatriz(char **matriz, int nl, int nc, Cores cor)
{
  int i, j;
  char corUsada[10];

  assert(cor >= 0 && cor <= 3);

  switch (cor)
  {
  case AZUL:
    strcpy(corUsada, COR_AZUL);
    break;
  case VERDE:
    strcpy(corUsada, COR_VERDE);
    break;
  case AMARELO:
    strcpy(corUsada, COR_AMARELO);
    break;
  case VERMELHO:
    strcpy(corUsada, COR_VERMELHO);
    break;
  }

  for (i = 0; i < nl; i++)
  {
    for (j = 0; j < nc; j++)
      if (matriz[i][j] == ORG)
        printf("%s%c ", corUsada, CHAR_CELULA_VIVA);
      else
        printf("%s%c ", COR_CINZA, CHAR_CELULA_MORTA);

    printf("\n");
  }
}

int contStr(const char *str[]);
int maiorStr(const char *str[]);

int printSign(Sign_Settings settings, const char *str[])
{
  int i, j, k, len, signLen, strLines, strMax, totalLines, usedLines, usedWidth, verticalAlignLines, halfVerticalLines, halfTitleWidth, lastHalfLines, firstOptionLine = 0;
  Terminal_Size tsize;

  assert(settings.alignment >= 0 && settings.alignment <= 2);
  assert(settings.maxWidth >= 0);
  assert(settings.maxHeight >= 0);

  apagaTela(0);
  tamanhoTerminal(&tsize);

  strMax = maiorStr(str);
  strLines = contStr(str) - 1;

  if (settings.maxWidth > 0)
  {
    assert(settings.maxWidth > strMax / 2);
    usedWidth = tsize.width > settings.maxWidth ? settings.maxWidth : tsize.width;
  }
  else
    usedWidth = tsize.width;

  if (settings.maxHeight > 0)
  {
    assert(settings.maxHeight >= strLines);
    usedLines = tsize.height > settings.maxHeight ? settings.maxHeight : tsize.height;
  }
  else
    usedLines = tsize.height;

  totalLines = strLines > (usedLines - 4) ? strLines + usedLines : usedLines;
  verticalAlignLines = (totalLines - strLines) - 2;
  halfVerticalLines = verticalAlignLines / 2;

  signLen = usedWidth - 4;

  // TITULO
  halfTitleWidth = (usedWidth - strlen(*str) - 2) / 2;

  for (i = 0; i < halfTitleWidth; i++)
    printf("=");

  printf(" %s ", *str);

  for (i = 0; i < halfTitleWidth + (usedWidth % 2); i++)
    printf("=");

  printf("\n");

  // ESPAÇO EM BRANCO
  for (i = 0; i < halfVerticalLines; i++)
    printf("= %*s =\n", signLen, "");

  *str++;
  lastHalfLines = halfVerticalLines + (strLines % 2);
  firstOptionLine += settings.firstOptionIndex > 0 ? 1 + i : 0;
  i = 0;

  // CONTEÚDO
  while (*str)
  {
    /**
     * Se o número de caracteres na string for maior que a largura disponível na linha do terminal,
     * ele acaba criando um loop infinito. Essa variável calcula o número de linhas necessárias para
     * imprimir a string.
     */
    int dividedStrLines = ceil(strlen(*str) / (float)signLen);

    if (dividedStrLines > 1)
    {
      int letters;
      char **splittedString = split(*str, "", &letters);
      for (j = 0; j < dividedStrLines; j++)
      {
        int start = signLen * j, end;

        letters -= signLen;
        end = letters < 0 ? strlen(*str) : signLen * (j + 1);

        if (j > 0)
          lastHalfLines -= 1;

        printf("= %*s", (settings.alignment == RIGHT) * (signLen - (end - start)), "");
        for (k = start; k < end; k++)
          printf("%s", splittedString[k]);

        if (i != settings.firstOptionIndex)
          i++;

        printf("%*s =\n", (settings.alignment == LEFT || settings.alignment == CENTER) * (signLen - (end - start)), "");
      }

      desalocaMatriz(splittedString, letters);
    }
    else if (settings.alignment != CENTER)
    {
      if (i != settings.firstOptionIndex)
        i++;

      int availableSpace = signLen - strlen(*str);
      int isLeftAlign = (settings.alignment == LEFT);
      printf("= %*s%s%*s =\n", !isLeftAlign * availableSpace, "", *str, isLeftAlign * availableSpace, "");
    }
    else
    {
      if (i != settings.firstOptionIndex)
        i++;

      int centerAlignSpaces = (signLen - strlen(*str)) / 2;
      int leftSpace = centerAlignSpaces * 2 + strlen(*str) == signLen ? centerAlignSpaces : centerAlignSpaces + 1;
      printf("= %*s%s%*s =\n", centerAlignSpaces, "", *str, leftSpace, "");
    }

    *str++;
  }

  firstOptionLine += i;

  // ESPAÇO EM BRANCO
  for (i = 0; i < lastHalfLines; i++)
    printf("= %*s =\n", signLen, "");

  for (i = 0; i < usedWidth; i++)
    printf("=");

  printf("\n");

  return firstOptionLine;
}

/* Conta o número de strings em uma matriz que termina com NULL */
int contStr(const char *str[])
{
  int cont = 0;

  while (*str)
  {
    cont++;
    *str++;
  }

  return cont;
}

/* Retorna o número de caracteres da maior string */
int maiorStr(const char *str[])
{
  int cont = 0;

  while (*str)
  {
    if (strlen(*str) > cont)
      cont = strlen(*str);
    *str++;
  }

  return cont;
}

void apagaTela(int nl)
{
  int i;

  assert(nl >= 0);

  if (nl == 0)
    system("cls");
  else
    for (i = 0; i < nl; i++)
      printf("%s%s", APAGA_LINHA, COMECO_LINHA_ANT);
}
