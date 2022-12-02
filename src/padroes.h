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
} Padroes;

void lePadrao(Padroes opcao, char **matMain, int nl, int nc, int xInic, int yInic);

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