#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "uteis.h"
#include "padroes.h"
#include "ciclos.h"
#include "celulas.h"

#define TAM 101

void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos);
void imprimeMatriz(char **matriz, int nl, int nc);
void menuInicJogo(char **mat, int nL, int nC);

int main()
{
  int nL = 20, nC = 20, nCiclos = 50, jogando = 1; // ou fornecidos pelo usuario
  char **mat;

  srand(time(NULL));

  mat = alocaMatriz(nL, nC);

  while (jogando)
  {
    menuInicJogo(mat, nL, nC);
    jogaJogoVida(mat, nL, nC, nCiclos);
    // fim do jogo
    printf("Digite 1 para jogar novamente, 0 para parar: ");
    scanf("%d", &jogando); // depois eu penso numa UI melhor pra isso
  }

  desalocaMatriz(mat, nL);
}

void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos)
{
  char **mAnt;
  int c;

  // imprimindo na tela a matriz inicial
  system("cls");
  imprimeMatriz(mAtual, nL, nC);
  // getchar();
  Sleep(100);

  mAnt = alocaMatriz(nL, nC);

  for (c = 1; c <= nCiclos; c++)
  {
    copiaMatriz(mAtual, mAnt, nL, nC);

    atualizaMat(mAnt, mAtual, nL, nC);
    system("cls");
    imprimeMatriz(mAtual, nL, nC);
    // getchar();
    Sleep(100);
  }
  desalocaMatriz(mAnt, nL);
}

/* A função ainda é bem simples, por enquanto está aqui apenas para testarmos o resto */
void imprimeMatriz(char **matriz, int nl, int nc)
{
  int i, j;

  for (i = 0; i < nl; i++)
  {
    for (j = 0; j < nc; j++)
      printf("%c ", matriz[i][j]);

    printf("\n");
  }
}

void menuInicJogo(char **mat, int nL, int nC)
{
  int opcao;

  printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre com a opcao: ");
  scanf("%d", &opcao);

  limpaMatriz(mat, nL, nC);
  // iniciaPadrao(opcao, mat, nL, nC);
  lePadrao(opcao - 1, mat, nL, nC, -1, -1);
  imprimeMatriz(mat, nL, nC);

  printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo...");
  while (getchar() != '\n')
    ;
  getchar();
}
