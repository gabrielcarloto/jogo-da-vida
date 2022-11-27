#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORG 'X'
#define VAZ '.'

#define TAM_LINHA 1024

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
  char **palavras, *palavra;
  int i, linhas = TAM_LINHA;

  palavras = alocaMatriz(linhas, TAM_LINHA);
  palavra = strtok(str, separador);

  for (i = 0; i < linhas && palavra != NULL; i++)
  {
    strcpy(palavras[i], palavra);
    palavra = strtok(NULL, separador);
  }

  if (i < linhas)
    palavras = realloc(palavras, i * sizeof(char **));

  *contPalavras = i;

  return palavras;
}

/**
 * Atualiza a matriz atual com base nos dados da matriz anterior
 *
 * A matriz anterior funciona é imutável e funciona como base para as comparações
 *
 * @param cicloAnt matriz anterior
 * @param cicloAtual matriz que será atualizada
 * @param nl número de linhas
 * @param nc número de colunas
 */
void atualizaMat(char **cicloAnt, char **cicloAtual, int nl, int nc)
{
  int i, j, coordenada_fixa = 0, vizinhas = 0;

  //*==Lendo cada canto da matriz==*

  // canto superior e inferior esquerdo:
  for (i = 0; i < 2; i++)
  {
    if (i == 0) // canto superior
    {
      if (cicloAnt[0][1] == 'ORG')
        vizinhas++;
      if (cicloAnt[1][1] == 'ORG')
        vizinhas++;
      if (cicloAnt[1][0] == 'ORG')
        vizinhas++;
    }
    else // canto inferior
    {
      if (cicloAnt[nl - 1][1] == 'ORG')
        vizinhas++;
      if (cicloAnt[nl - 2][1] == 'ORG')
        vizinhas++;
      if (cicloAnt[nl - 2][0] == 'ORG')
        vizinhas++;
    }

    if (cicloAnt[coordenada_fixa][0] == 'VAZ' && vizinhas == 3)
      cicloAtual[coordenada_fixa][0] = 'ORG';
    else if (cicloAnt[coordenada_fixa][0] == 'ORG' && vizinhas < 2)
      cicloAtual[coordenada_fixa][0] = 'VAZ';

    vizinhas = 0; // resetando contador

    coordenada_fixa += nl - 1; // aumenta a coordenada da linha para nl-1 (última linha)
  }

  coordenada_fixa = 0; // resetando para zero

  // canto superior e inferior direito:
  for (i = 0; i < 2; i++)
  {
    if (i == 0) // canto superior
    {
      if (cicloAnt[0][nc - 2] == 'ORG')
        vizinhas++;
      if (cicloAnt[1][nc - 2] == 'ORG')
        vizinhas++;
      if (cicloAnt[1][nc - 1] == 'ORG')
        vizinhas++;
    }
    else // canto inferior
    {
      if (cicloAnt[nl - 1][nc - 2] == 'ORG')
        vizinhas++;
      if (cicloAnt[nl - 2][nc - 2] == 'ORG')
        vizinhas++;
      if (cicloAnt[nl - 2][nc - 1] == 'ORG')
        vizinhas++;
    }

    if (cicloAnt[coordenada_fixa][nc - 1] == 'VAZ' && vizinhas == 3)
      cicloAtual[coordenada_fixa][nc - 1] = 'ORG';
    else if (cicloAnt[coordenada_fixa][nc - 1] == 'ORG' && vizinhas < 2)
      cicloAtual[coordenada_fixa][nc - 1] = 'VAZ';

    vizinhas = 0; // resetando contador

    coordenada_fixa += nl - 1; // aumenta a coordenada da linha para nl-1 (última linha)
  }

  coordenada_fixa = 0; // resetando para zero

  //*==Lendo as bordas (sem os cantos)==*

  // bordas horizontais:
  for (i = 0; i < 2; i++)
  {
    if (i == 0)                    // borda superior
      for (j = 1; j < nc - 1; j++) // vai da segunda posição até a penúltima posição da linha
      {
        if (cicloAnt[coordenada_fixa][j - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa + 1][j - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa + 1][j] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa + 1][j + 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa][j + 1] == 'ORG')
          vizinhas++;

        if (cicloAnt[coordenada_fixa][j] == 'VAZ' && vizinhas == 3)
          cicloAtual[coordenada_fixa][j] = 'ORG';
        else if ((cicloAnt[coordenada_fixa][j] == 'ORG' && vizinhas < 2) || (cicloAnt[coordenada_fixa][j] == 'ORG' && vizinhas > 3))
          cicloAtual[coordenada_fixa][j] = 'VAZ';

        vizinhas = 0; // resetando contador
      }
    else // borda inferior
      for (j = 1; j < nc - 1; j++)
      {
        if (cicloAnt[coordenada_fixa][j - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa - 1][j - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa - 1][j] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa - 1][j + 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[coordenada_fixa][j + 1] == 'ORG')
          vizinhas++;

        if (cicloAnt[coordenada_fixa][j] == 'VAZ' && vizinhas == 3)
          cicloAtual[coordenada_fixa][j] = 'ORG';
        else if ((cicloAnt[coordenada_fixa][j] == 'ORG' && vizinhas < 2) || (cicloAnt[coordenada_fixa][j] == 'ORG' && vizinhas > 3))
          cicloAtual[coordenada_fixa][j] = 'VAZ';

        vizinhas = 0; // resetando contador
      }

    coordenada_fixa += nl - 1; // aumenta a coordenada a linha em nl-1 (última linha)
  }

  coordenada_fixa = 0;

  // bordas verticais:
  for (i = 0; i < 2; i++)
  {
    if (i == 0)                    // borda da esquerda
      for (j = 1; j < nl - 1; j++) // vai da segunda posição até a penúltima posição da coluna
      {
        if (cicloAnt[j - 1][coordenada_fixa] == 'ORG')
          vizinhas++;
        if (cicloAnt[j - 1][coordenada_fixa + 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[j][coordenada_fixa + 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[j + 1][coordenada_fixa + 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[j + 1][coordenada_fixa] == 'ORG')
          vizinhas++;

        if (cicloAnt[j][coordenada_fixa] == 'VAZ' && vizinhas == 3)
          cicloAtual[j][coordenada_fixa] = 'ORG';
        else if ((cicloAnt[j][coordenada_fixa] == 'ORG' && vizinhas < 2) || (cicloAnt[coordenada_fixa][j] == 'ORG' && vizinhas > 3))
          cicloAtual[j][coordenada_fixa] = 'VAZ';

        vizinhas = 0; // resetando contador
      }
    else // borda da direita
      for (j = 1; j < nl - 1; j++)
      {
        if (cicloAnt[j - 1][coordenada_fixa] == 'ORG')
          vizinhas++;
        if (cicloAnt[j - 1][coordenada_fixa - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[j][coordenada_fixa - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[j + 1][coordenada_fixa - 1] == 'ORG')
          vizinhas++;
        if (cicloAnt[j + 1][coordenada_fixa] == 'ORG')
          vizinhas++;

        if (cicloAnt[j][coordenada_fixa] == 'VAZ' && vizinhas == 3)
          cicloAtual[j][coordenada_fixa] = 'ORG';
        else if ((cicloAnt[j][coordenada_fixa] == 'ORG' && vizinhas < 2) || (cicloAnt[j][coordenada_fixa] == 'ORG' && vizinhas > 3))
          cicloAtual[j][coordenada_fixa] = 'VAZ';

        vizinhas = 0; // resetando contador
      }

    coordenada_fixa += nc - 1; // aumenta a coordenada da coluna em nc-1 (última coluna)
  }

  //*==Lendo o miolo==*

  for (i = 1; i < nl - 1; i++) // vai da segunda a penúltima linha
  {
    for (j = 1; j < nc - 1; j++) // vai da segunda a penúltima coluna
    {
      if (cicloAnt[i - 1][j - 1] == 'ORG')
        vizinhas++;
      if (cicloAnt[i - 1][j] == 'ORG')
        vizinhas++;
      if (cicloAnt[i - 1][j + 1] == 'ORG')
        vizinhas++;
      if (cicloAnt[i][j - 1] == 'ORG')
        vizinhas++;
      if (cicloAnt[i][j + 1] == 'ORG')
        vizinhas++;
      if (cicloAnt[i + 1][j - 1] == 'ORG')
        vizinhas++;
      if (cicloAnt[i + 1][j] == 'ORG')
        vizinhas++;
      if (cicloAnt[i + 1][j + 1] == 'ORG')
        vizinhas++;

      if (cicloAnt[i][j] == 'VAZ' && vizinhas == 3)
        cicloAtual[i][j] = 'ORG';
      else if ((cicloAnt[i][j] == 'ORG' && vizinhas < 2) || (cicloAnt[i][j] == 'ORG' && vizinhas > 3))
        cicloAtual[i][j] = 'VAZ';

      vizinhas = 0; // resetando contador
    }

    /*/Pra não escrever 8 comparações, também pensei em fazer as comparações do miolo assim:

    for(x=-1; x<2; x++)
    {
        if(x == 0) //para não verificar cicloAnt[i][j]
            for(y=-1; y<2; y+=2)
                if(cicloAnt[i+x][j+y] == 'ORG')
                    vizinhas++;
        else
            for(y=-1; y<2; y++)
                if(cicloAnt[i+x][j+y] == 'ORG')
                    vizinhas++;

    }

    Só que dessa forma ao invés de 8 comparações por posição, teríamos 11 por posição, oque para uma matriz muito grande poderia pesar talvez, n sei
    /*/
  }
}
