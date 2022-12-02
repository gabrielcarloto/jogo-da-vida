#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "shared.c"
#include "ciclos.c"
#include "interface.c"

#define TAM 101

void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos);
void menuInicJogo(char **mat, int nL, int nC);

int main()
{
  int nL = 20, nC = 20, nCiclos = 50; // ou fornecidos pelo usuario
  char **mat;

  atexit(resetaConsole);
  setupConsole();
  srand(time(NULL));

  mat = alocaMatriz(nL, nC);

  // inicio laço indeterminado
  menuInicJogo(mat, nL, nC);
  jogaJogoVida(mat, nL, nC, nCiclos);
  // fim do laco indeterminado

  desalocaMatriz(mat, nL);
}

void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos)
{
  char **mAnt;
  int c;

  // imprimindo na tela a matriz inicial
  apagaTela(0);
  toggleCursor(0);
  imprimeMatriz(mAtual, nL, nC);
  // getchar();
  Sleep(100);

  mAnt = alocaMatriz(nL, nC);

  for (c = 1; c <= nCiclos; c++)
  {
    copiaMatriz(mAtual, mAnt, nL, nC);

    atualizaMat(mAnt, mAtual, nL, nC);
    apagaTela(nL);
    imprimeMatriz(mAtual, nL, nC);
    // getchar();
    Sleep(150);
  }

  desalocaMatriz(mAnt, nL);
  resetaConsole();
}

int imprimeOpcoesMenu(Sign_Settings config, const char opcoes[][TAM], int tamanho)
{
  return imprimePlaca(config, "MENU", "Escolha um dos padroes para iniciar o jogo:", " ", opcoes[0], opcoes[1], opcoes[2], opcoes[3], opcoes[4], " ", opcoes[tamanho - 1]);
}

void menuInicJogo(char **mat, int nL, int nC)
{
  char opcoes[][TAM] = {"Bloco <", "Blinker", "Sapo", "Glider", "LWSS", "Sair do jogo"}, input;
  int opcao = 0, opcaoAnt, alturaTerminal, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings set;

  set.alignment = LEFT;
  set.maxHeight = 20;
  set.maxWidth = 75;

  alturaTerminal = imprimeOpcoesMenu(set, opcoes, numOpcoes);

  input = getch();

  while (!verificaInput(input))
    input = getch();

  while (input != TECLA_ENTER)
  {
    opcaoAnt = opcao;

    if (input == SETA_BAIXO || input == TECLA_S || input == TECLA_S_CAPS)
      opcao += (opcao < 5);
    else if (input == SETA_CIMA || input == TECLA_W || input == TECLA_W_CAPS)
      opcao -= (opcao != 0);

    if (opcao != opcaoAnt)
    {
      opcoes[opcaoAnt][strlen(opcoes[opcaoAnt]) - 2] = '\0';
      strcat(opcoes[opcao], " <");
    }

    apagaTela(alturaTerminal);
    alturaTerminal = imprimeOpcoesMenu(set, opcoes, numOpcoes);
    input = getch();
    while (!verificaInput(input))
      input = getch();
  }

  if (opcao == 5)
    exit(0);

  limpaMatriz(mat, nL, nC);
  iniciaPadrao(opcao + 1, mat, nL, nC);
  apagaTela(alturaTerminal);
  imprimeMatriz(mat, nL, nC);

  printf("%sPressione qualquer tecla para iniciar...", RESET);
  getch();
}
