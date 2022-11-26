#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>

#define ORG 'X'
#define VAZ '.'

#define TAM_LINHA 1024

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

/**
 * Aloca uma matriz de nl linhas e nc colunas
 *
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
char **alocaMatriz(int nl, int nc)
{
  int i;
  char **matriz;

  matriz = (char **)malloc(nl * sizeof(char *));

  if (matriz == NULL)
  {
    fprintf(stderr, "Erro ao alocar matriz.\n");
    exit(1);
  }

  for (i = 0; i < nl; i++)
  {
    matriz[i] = (char *)malloc(nl * sizeof(char));

    if (matriz[i] == NULL)
    {

      fprintf(stderr, "Erro ao alocar matriz.\n");
      exit(1);
    }
  }

  return matriz;
}

/**
 * Libera memória alocada pela função alocaMatriz
 *
 * @param matriz Matriz a ser liberada
 * @param nl Número de linhas
 */
void desalocaMatriz(char **matriz, int nl)
{
  int i;

  for (i = 0; i < nl; i++)
    free(matriz[i]);

  free(matriz);
}

/* Preenche a matriz com o valor de VAZ */
void limpaMatriz(char **matriz, int nl, int nc)
{
  int i, j;
  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      matriz[i][j] = VAZ;
}

/**
 * Copia a matriz original para nova matriz (assumindo que ambas terão o mesmo tamanho)
 *
 * @param matrizOriginal Matriz que será copiada
 * @param novaMatriz Matriz que receberá os valores da matrizOriginal
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
void copiaMatriz(char **matrizOriginal, char **novaMatriz, int nl, int nc)
{
  int i, j;

  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      novaMatriz[i][j] = matrizOriginal[i][j];
}

/**
 * Separa uma string em um vetor de strings (na verdade é uma matriz, mas lide como se fosse um vetor)
 *
 * Ex.: uma string "Olá, meu nome é Pedro!" separada por " " retornará `{"Olá,", "meu", "nome", "é", "Pedro"}`
 *
 * @param str String a ser separada
 * @param separador O que vai separar a string
 * @param contPalavras Ponteiro para um int, que receberá o número de palavras (strings) contidas no retorno
 */
char **split(char *str, char *separador, int *contPalavras)
{
  char **retorno;
  int i, linhas;

  if (separador == "")
  {
    linhas = strlen(str);

    char **letras = alocaMatriz(linhas, 2);

    for (i = 0; i < linhas; i++)
    {
      letras[i][0] = str[i];
      letras[i][1] = '\0';
    }

    retorno = letras;
    *contPalavras = linhas;
  }
  else
  {
    linhas = TAM_LINHA;

    char **palavras = alocaMatriz(linhas, TAM_LINHA);
    char *palavra = strtok(str, separador);

    for (i = 0; i < linhas && palavra != NULL; i++)
    {
      strcpy(palavras[i], palavra);
      palavra = strtok(NULL, separador);
    }

    if (i < linhas)
      palavras = realloc(palavras, i * sizeof(char **));

    retorno = palavras;
    *contPalavras = i;
  }

  return retorno;
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

int contStr(const char *firstArg, va_list args);

/**
 * Imprime uma "placa", ocupando todo o espaço disponível no terminal
 *
 * @param alignment Alinhamento das strings (LEFT, CENTER ou RIGHT)
 * @param str Strings a serem impressas
 */
void printSign(Sign_Alignment alignment, const char *str, ...)
{
  int i, j, k, len, signLen, strLines, totalLines, verticalAlignLines, halfVerticalLines;
  Terminal_Size tsize;
  va_list list, list2;

  if (alignment < 0 || alignment > 2)
  {
    fprintf(stderr, "O alinhamento escolhido (%d) não existe.\n", alignment);
    exit(1);
  }

  system("cls");

  tamanhoTerminal(&tsize);

  va_start(list, str);
  va_copy(list2, list);

  strLines = contStr(str, list);
  totalLines = strLines > (tsize.height - 4) ? strLines + tsize.height : tsize.height;
  verticalAlignLines = (totalLines - strLines) - 2;
  halfVerticalLines = verticalAlignLines / 2;

  va_end(list);

  signLen = tsize.width - 4;

  for (i = 0; i < tsize.width; i++)
    printf("=");

  printf("\n");

  for (i = 0; i < halfVerticalLines; i++)
    printf("= %*s =\n", signLen, "");

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

          printf("= %*s", (alignment == RIGHT) * (signLen - (end - start)), "");
          for (k = start; k < end; k++)
            printf("%s", splittedString[k]);

          printf("%*s =", (alignment == LEFT || alignment == CENTER) * (signLen - (end - start)), "");
        }
      }
      else
      {
        if (alignment != CENTER)
          printf("= %*s%s%*s =\n", (alignment == RIGHT) * (signLen - strlen(str)), "", str, (alignment == LEFT) * (signLen - strlen(str)), "");
        else
        {
          int centerAlignSpaces = (signLen - strlen(str)) / 2;
          int leftSpace = centerAlignSpaces * 2 + strlen(str) == signLen ? centerAlignSpaces : centerAlignSpaces + 1;
          printf("= %*s%s%*s =\n", centerAlignSpaces, "", str, leftSpace, "");
        }
      }

      str = va_arg(list, char *);
    }
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