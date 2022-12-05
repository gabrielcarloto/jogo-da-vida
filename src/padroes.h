#ifndef _PADROES_H_
#define _PADROES_H_
#include <stdio.h>
#include <string.h>

#define MAX_CHARS 50
#define NUM_PADROES 5

typedef enum
{
  BLOCO,
  BLINKER,
  SAPO,
  GLIDER,
  LWSS,
  FIREWORK
} Padroes;

/**
 * @brief Lê o arquivo do padrão escolhido e o insere na matriz do ciclo
 *
 * @param opcao Padrão a ser lido
 * @param ciclo Matriz do ciclo onde o padrão será inserido
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 * @param xInic Coluna na qual o padrão será inserido (se for -1, será inserido no centro)
 * @param yInic Linha na qual o padrão será inserido (se for -1, será inserido no centro)
 */
void lePadrao(Padroes opcao, char **ciclo, int nl, int nc, int xInic, int yInic);

/**
 * @deprecated use a função lePadrao
 */
void inicBlinker(char **m, int nL, int nC);

/**
 * @deprecated use a função lePadrao
 */
void inicBloco(char **m, int nL, int nC);

/**
 * @deprecated use a função lePadrao
 */
void inicSapo(char **m, int nL, int nC);

/**
 * @deprecated use a função lePadrao
 */
void inicGlider(char **m, int nL, int nC);

/**
 * @deprecated use a função lePadrao
 */
void inicLWSS(char **m, int nL, int nC);
#endif