#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "uteis.c"

#define ORG 'X'
#define VAZ '.'
#define TAM 101

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void inicBlinker(char **m, int nL, int nC)
{
  char padrao[1][3] = {{ORG, ORG, ORG}};
  int i, j, xInic = nL / 2, yInic = nC / 2;

  limpaMatriz(m, VAZ, nL, nC);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 3; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicBloco(char **m, int nL, int nC)
{
  char padrao[2][2] = {{ORG, ORG}, {ORG, ORG}};
  int i, j, xInic = nL / 2, yInic = nC / 2;

  limpaMatriz(m, VAZ, nL, nC);

  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicSapo(char **m, int nL, int nC)
{

  char padrao[2][4] = {{VAZ, ORG, ORG, ORG}, {ORG, ORG, ORG, VAZ}};
  int i, j, xInic = nL / 2, yInic = nC / 2;

  limpaMatriz(m, VAZ, nL, nC);

  for (i = 0; i < 2; i++)
    for (j = 0; j < 4; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicGlider(char **m, int nL, int nC)
{
  char padrao[3][3] = {{ORG, ORG, ORG}, {ORG, VAZ, VAZ}, {VAZ, ORG, VAZ}};
  int i, j, xInic, yInic;

  limpaMatriz(m, VAZ, nL, nC);

  xInic = nL - 4;
  yInic = nC - 4;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicLWSS(char **m, int nL, int nC)
{
  char padrao[4][5] = {{VAZ, ORG, VAZ, VAZ, ORG}, {ORG, VAZ, VAZ, VAZ, VAZ}, {ORG, VAZ, VAZ, VAZ, ORG}, {ORG, ORG, ORG, ORG, VAZ}};
  int i, j, xInic, yInic;

  limpaMatriz(m, VAZ, nL, nC);

  xInic = nL - 5;
  yInic = nC - 6;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 5; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void menuInicJogo(char **mat, int nL, int nC)
{
  int opcao;

  printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre com a opcao: ");
  scanf("%d", &opcao);
  switch (opcao)
  {
  case 1:
    inicBloco(mat, nL, nC);
    break;
  case 2:
    inicBlinker(mat, nL, nC);
    break;
  case 3:
    inicSapo(mat, nL, nC);
    break;
  case 4:
    inicGlider(mat, nL, nC);
    break;
  case 5:
    inicLWSS(mat, nL, nC);
    break;
  }

  imprimeMatriz(mat, nL, nC);

  printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo...");
  while (getchar() != '\n')
    ;
  getchar();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Parte a ser completada //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

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
    copiaMatriz(mAnt, mAtual, nL, nC);

    atualizaMat(mAtual, mAnt, nL, nC);
    system("cls");
    imprimeMatriz(mAtual, nL, nC);
    // getchar();
    Sleep(100);
  }
  desalocaMatriz(mAnt, nL);
}

int main()
{

  char **mat;

  int nL = 20, nC = 20, nCiclos = 50; // ou fornecidos pelo usuario

  mat = alocaMatriz(nL, nC);

  // inicio laço indeterminado
  menuInicJogo(mat, nL, nC);
  jogaJogoVida(mat, nL, nC, nCiclos);
  // fim do laco indeterminado

  desalocaMatriz(mat, nL);
}
