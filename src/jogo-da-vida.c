#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "interface.h"
#include "console.h"
#include "uteis.h"
#include "padroes.h"
#include "ciclos.h"
#include "celulas.h"
#include "menu.h"

void jogaJogoVida(char **mAtual, Game_Settings *settings);
void menuInicial(Game_Settings *settings);
int inputUsuario(int numOpcoes, int saiComEsc);

int main()
{
  Sign_Settings signSettings;
  Game_Settings gameSettings;
  Terminal_Size tsize;
  int jogando = 1;
  char **mat;

  atexit(resetaConsole);
  setupConsole();
  srand(time(NULL));

  gameSettings.velocidade_atualizacao = 150;
  gameSettings.ciclos = 50;
  gameSettings.linhas = 20;
  gameSettings.colunas = 20;
  gameSettings.chance_invasores = 15;
  gameSettings.numero_invasores = 10;
  strcpy(gameSettings.cor_tema, COR_VERDE);

  signSettings.alignment = CENTER;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;

  imprimePlaca(signSettings, "INSTRUCOES", "Navegue com as setas ou numeros do teclado", "Pressione enter para continuar");
  getch();

  while (jogando)
  {
    resizeWindow(MIN_LARGURA_TELA, MIN_ALTURA_TELA);
    menuInicial(&gameSettings);

    if (!gameSettings.linhas || !gameSettings.colunas)
    {
      resizeWindow(0, 0);
      tamanhoTerminal(&tsize);

      gameSettings.linhas = tsize.height - 4;
      gameSettings.colunas = tsize.width / 2;
    }
    else
    {
      resizeWindow((gameSettings.colunas) * 8 * 2 + 40, (gameSettings.linhas + 4) * 16);
    }

    mat = alocaMatriz(gameSettings.linhas, gameSettings.colunas);
    limpaMatriz(mat, gameSettings.linhas, gameSettings.colunas);
    lePadrao(gameSettings.padrao, mat, gameSettings.linhas, gameSettings.colunas, -1, -1);
    ClearScreen();
    imprimeMatriz(mat, gameSettings.linhas, gameSettings.colunas, gameSettings.cor_tema);

    printf("%sPressione qualquer tecla para iniciar...", RESET);
    getch();

    jogaJogoVida(mat, &gameSettings);
    printf("Digite 1 para jogar novamente, 0 para parar: ");
    scanf("%d", &jogando); // depois eu penso numa UI melhor pra isso
  }

  desalocaMatriz(mat, gameSettings.linhas);
}

void jogaJogoVida(char **mAtual, Game_Settings *settings)
{
  int c, nL = settings->linhas, nC = settings->colunas;
  char **mAnt;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

  ClearScreen();
  imprimeMatriz(mAtual, nL, nC, settings->cor_tema);
  Sleep(settings->velocidade_atualizacao);

  mAnt = alocaMatriz(nL, nC);

  for (c = 1; c <= settings->ciclos; c++)
  {
    copiaMatriz(mAtual, mAnt, nL, nC);

    atualizaMat(mAnt, mAtual, nL, nC, settings->chance_invasores, settings->numero_invasores);
    SetConsoleCursorPosition(h, (COORD){0, 0});
    imprimeMatriz(mAtual, nL, nC, settings->cor_tema);

    Sleep(settings->velocidade_atualizacao);
  }

  desalocaMatriz(mAnt, nL);
  resetaConsole();
}
