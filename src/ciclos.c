#include <assert.h>
#include <stdlib.h>

#include "ciclos.h"
#include "celulas.h"
#include "padroes.h"
#include "uteis.h"

int verificaCelula(const char celula)
{
  if (celula == ORG)
    return TRUE;

  return FALSE;
}

int verificaVizinhas(const char **ciclo, int linha, int coluna, int nl, int nc)
{
  int i, j, vizinhasVivas = 0;

  for (i = linha - 1; i <= linha + 1; i++)
    for (j = coluna - 1; j <= coluna + 1; j++)
    {
      if (i == linha && j == coluna ||
          i < 0 || i >= nl ||
          j < 0 || j >= nc)
        continue;

      vizinhasVivas += verificaCelula(ciclo[i][j]);
    }

  return vizinhasVivas;
}

void adicionaInvasores(int chance, int maxInvasores, char **cicloAtual, int nl, int nc)
{
  int i, numInvasores, padrao;
  const int dado = 1 + rand() % 100;

  assert(chance >= 0 && chance <= 100);

  if (dado <= chance)
  {
    if (rand() % 2)
    {
      padrao = rand() % NUM_PADROES;
      lePadrao(padrao, cicloAtual, nl, nc, rand() % nc, rand() % nl);
    }
    else
    {
      numInvasores = 1 + rand() % maxInvasores;

      for (i = 0; i < numInvasores; i++)
        cicloAtual[rand() % nl][rand() % nc] = ORG;
    }
  }
}

void atualizaMat(const char **cicloAnt, char **cicloAtual, int nl, int nc, int chanceInvasores, int numInvasores)
{
  int i, j, vizinhasVivas, estaViva;

  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
    {
      vizinhasVivas = verificaVizinhas(cicloAnt, i, j, nl, nc);
      estaViva = verificaCelula(cicloAnt[i][j]);

      // Issue #4
      if (estaViva)
        if (vizinhasVivas < 2 || vizinhasVivas > 3)
          cicloAtual[i][j] = VAZ;
        else
          cicloAtual[i][j] = ORG;
      else if (vizinhasVivas == 3)
        cicloAtual[i][j] = ORG;
    }

  adicionaInvasores(chanceInvasores, numInvasores, cicloAtual, nl, nc);
}

void iniciaPadrao(Padroes padrao, char **ciclo, int nl, int nc)
{
  assert(padrao >= 1 && padrao <= NUM_PADROES);

  switch (padrao)
  {
  case 1:
    inicBloco(ciclo, nl, nc);
    break;
  case 2:
    inicBlinker(ciclo, nl, nc);
    break;
  case 3:
    inicSapo(ciclo, nl, nc);
    break;
  case 4:
    inicGlider(ciclo, nl, nc);
    break;
  case 5:
    inicLWSS(ciclo, nl, nc);
    break;
  }
}