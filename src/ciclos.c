#include <assert.h>
#include "shared.c"
#include "padroes.c"

/**
 * Inicia um dos padrões disponíveis
 *
 * @param padrao Pode ser um número entre 1 e NUM_PADROES, ou o nome do padrão
 * @param ciclo A matriz que receberá o padrão
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 */
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

/* Retorna 0 se a célula está morta e 1 caso esteja viva */
int verificaCelula(const char celula)
{
  if (celula == ORG)
    return 1;

  return 0;
}

/**
 * Conta o número de células vizinhas vivas
 *
 * @param ciclo Matriz a se comparar as vizinhas
 * @param linha Linha da célula
 * @param coluna Coluna da célula
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 */
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

/**
 * Possivelmente adicionará invasores em um ciclo. Se for adicionar, há 50%
 * de chance de inserir um padrão aleatório e 50% de chance de adicionar células
 * em lugares aleatórios.
 *
 * @param chance De 0 a 100, a chance de aparecer um invasor no ciclo atual
 * @param maxInvasores Caso insira células em lugares aleatórios, esse será o máximo possível
 * @param cicloAtual O ciclo a ser alterado
 * @param nl Número de linhas na matriz do ciclo
 * @param nc Número de colunas na matriz do ciclo
 */
void adicionaInvasores(const int chance, const int maxInvasores, char **cicloAtual, int nl, int nc)
{
  int i, numInvasores, padrao;
  const int dado = 1 + rand() % 100;

  assert(chance >= 0 && chance <= 100);

  if (dado > 100 - chance)
    if (rand() % 2)
    {
      padrao = 1 + rand() % NUM_PADROES;
      iniciaPadrao(padrao, cicloAtual, nl, nc);
    }
    else
    {
      numInvasores = 1 + rand() % maxInvasores;

      for (i = 0; i < numInvasores; i++)
        cicloAtual[rand() % nl][rand() % nc] = ORG;
    }
}

/**
 * Atualiza o ciclo atual com base nos dados do ciclo anterior
 *
 * @param cicloAnt Matriz do ciclo anterior
 * @param cicloAtual Matriz (do ciclo atual) que será atualizada
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
void atualizaMat(const char **cicloAnt, char **cicloAtual, int nl, int nc)
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

  adicionaInvasores(15, 10, cicloAtual, nl, nc);
}
