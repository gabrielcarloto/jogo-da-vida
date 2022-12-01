#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uteis.c"
#include "console.c"

#define CHAR_CELULA_MORTA '.'
#define CHAR_CELULA_VIVA 'O'

#define APAGA_LINHA "\x1b[2K"
#define COMECO_LINHA_ANT "\x1b[1F"

#define RESET "\033[0m"
#define COR_AZUL "\033[34m"
#define COR_VERDE "\033[92m"
#define COR_CINZA "\033[90m"
#define COR_AMARELO "\033[93m"
#define COR_VERMELHO "\033[31m"
#define COR_PADRAO COR_VERDE

typedef enum
{
  LEFT,
  CENTER,
  RIGHT
} Sign_Alignment;

typedef struct
{
  Sign_Alignment alignment;
  int maxWidth;
  int maxHeight;
} Sign_Settings;

/**
 * @brief Apaga a terminal
 *
 * @param nl Número de linhas. Caso não o número de linhas seja desconhecido,
 * o valor 0 apaga todo o terminal.
 */
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

/**
 * @brief Imprime o ciclo representando células mortas como um ponto cinza
 * e células vivas como um "O" verde
 *
 * @param matriz Matriz do ciclo a ser impresso na tela
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 */
void imprimeMatriz(char **matriz, int nl, int nc)
{
  int i, j;

  for (i = 0; i < nl; i++)
  {
    for (j = 0; j < nc; j++)
      if (matriz[i][j] == ORG)
        printf("%s%c ", COR_PADRAO, CHAR_CELULA_VIVA);
      else
        printf("%s%c ", COR_CINZA, CHAR_CELULA_MORTA);

    printf("\n");
  }
}

int contStr(const char *str[]);
int maiorStr(const char *str[]);

/**
 * Imprime uma "placa", ocupando todo o espaço disponível no terminal.
 * É necessário inserir um NULL ao final para não criar um loop infinito.
 *
 * @param settings Configurações de exibição
 * @param str Strings a serem impressas. A primeira é o título
 */
void printSign(Sign_Settings settings, const char *str[])
{
  int i, j, k, len, signLen, strLines, strMax, totalLines, usedLines, usedWidth, verticalAlignLines, halfVerticalLines, halfTitleWidth, lastHalfLines;
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

  for (i = 0; i < halfTitleWidth + (halfTitleWidth % 2); i++)
    printf("=");

  printf("\n");

  // ESPAÇO EM BRANCO
  for (i = 0; i < halfVerticalLines; i++)
    printf("= %*s =\n", signLen, "");

  *str++;

  lastHalfLines = halfVerticalLines + (strLines % 2);

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

        printf("%*s =\n", (settings.alignment == LEFT || settings.alignment == CENTER) * (signLen - (end - start)), "");
      }

      desalocaMatriz(splittedString, letters);
    }
    else if (settings.alignment != CENTER)
    {
      int availableSpace = signLen - strlen(*str);
      int isLeftAlign = (settings.alignment == LEFT);
      printf("= %*s%s%*s =\n", !isLeftAlign * availableSpace, "", *str, isLeftAlign * availableSpace, "");
    }
    else
    {
      int centerAlignSpaces = (signLen - strlen(*str)) / 2;
      int leftSpace = centerAlignSpaces * 2 + strlen(*str) == signLen ? centerAlignSpaces : centerAlignSpaces + 1;
      printf("= %*s%s%*s =\n", centerAlignSpaces, "", *str, leftSpace, "");
    }

    *str++;
  }

  // ESPAÇO EM BRANCO
  for (i = 0; i < lastHalfLines; i++)
    printf("= %*s =\n", signLen, "");

  for (i = 0; i < usedWidth; i++)
    printf("=");

  printf("\n");
}

/**
 * Imprime uma "placa", ocupando todo o espaço disponível no terminal.
 *
 * @param config Configurações de exibição
 * @param str Strings a serem impressas (a primeira é o título)
 */
#define imprimePlaca(config, ...) printSign((Sign_Settings)config, (const char *[]){__VA_ARGS__, NULL})

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

// essas funções não devem estar disponíveiss em outros arquivos
#define contStr NULL
#define maiorStr NULL