#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "shared.c"
#include "ciclos.c"
#include "interface.c"

#define TAM 101

typedef enum
{
  CONFIG = 6,
  SAIR = 7
} Opcoes;

void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos);
Opcoes menuInicial(int *nl, int *nc);
int inputUsuario(int numOpcoes);

int main()
{
  int nL = 20, nC = 20, nCiclos = 50, opcao; // ou fornecidos pelo usuario
  char **mat;
  Sign_Settings set;

  set.alignment = CENTER;
  set.maxHeight = 20;
  set.maxWidth = 75;

  atexit(resetaConsole);
  setupConsole();
  srand(time(NULL));

  imprimePlaca(set, "INSTRUCOES", "Navegue com as setas ou numeros do teclado", "Pressione enter para continuar");
  getch();

  // inicio laço indeterminado
  opcao = menuInicial(&nL, &nC);

  mat = alocaMatriz(nL, nC);
  limpaMatriz(mat, nL, nC);
  iniciaPadrao(opcao + 1, mat, nL, nC);
  system("cls");
  imprimeMatriz(mat, nL, nC);

  printf("%sPressione qualquer tecla para iniciar...", RESET);
  getch();

  jogaJogoVida(mat, nL, nC, nCiclos);
  // fim do laco indeterminado

  desalocaMatriz(mat, nL);
}

void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos)
{
  int c;
  char **mAnt;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

  // imprimindo na tela a matriz inicial
  apagaTela(0);
  imprimeMatriz(mAtual, nL, nC);
  // getchar();
  Sleep(100);

  mAnt = alocaMatriz(nL, nC);

  for (c = 1; c <= nCiclos; c++)
  {
    copiaMatriz(mAtual, mAnt, nL, nC);

    atualizaMat(mAnt, mAtual, nL, nC);
    SetConsoleCursorPosition(h, (COORD){0, 0});
    imprimeMatriz(mAtual, nL, nC);
    // getchar();
    Sleep(150);
  }

  desalocaMatriz(mAnt, nL);
  resetaConsole();
}

/**
 * @brief Coleta a entrada do usuário até que esta seja uma entrada válida
 *
 * @param numOpcoes Número de opções, se disponível
 * @return (int) Número correspondente ao caractere digitado
 */
int inputUsuario(int numOpcoes)
{
  int input = getch();

  while (!verificaInput(input, numOpcoes))
    input = getch();

  return input;
}

Opcoes menuInicial(int *nl, int *nc)
{
  char opcoes[][TAM] = {"1. Bloco <", "2. Blinker", "3. Sapo", "4. Glider", "5. LWSS", "6. Configuracoes", "7. Sair do jogo"}, input;
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  Opcoes opcao = 0, opcaoAnt;
  Sign_Settings set;

  set.alignment = LEFT;
  set.maxHeight = 20;
  set.maxWidth = 75;
  set.firstOptionIndex = 3;

  inicioOpcoes = imprimePlaca(
      set,
      "MENU",
      "Escolha um dos padroes para iniciar o jogo:",
      " ",
      opcoes[0],
      opcoes[1],
      opcoes[2],
      opcoes[3],
      opcoes[4],
      " ",
      opcoes[numOpcoes - 2],
      opcoes[numOpcoes - 1]);

  // linhaCursor = wherey();
  input = inputUsuario(numOpcoes);

  while (input != TECLA_ENTER)
  {
    opcaoAnt = opcao;

    if (input == SETA_BAIXO || input == TECLA_S || input == TECLA_S_CAPS)
      opcao += (opcao < numOpcoes - 1);
    else if (input == SETA_CIMA || input == TECLA_W || input == TECLA_W_CAPS)
      opcao -= (opcao != 0);

    if (input - NUM_ZERO > 0 && input - NUM_ZERO <= numOpcoes)
      opcao = input - NUM_ZERO - 1;

    if (opcao != opcaoAnt)
    {
      opcoes[opcaoAnt][strlen(opcoes[opcaoAnt]) - 2] = '\0';
      strcat(opcoes[opcao], " <");
    }

    SetConsoleCursorPosition(h, (COORD){2, inicioOpcoes + (opcaoAnt >= numOpcoes - 2 ? opcaoAnt + 1 : opcaoAnt) - 1});
    printf("%s  ", opcoes[opcaoAnt]);

    SetConsoleCursorPosition(h, (COORD){2, inicioOpcoes + (opcao >= numOpcoes - 2 ? opcao + 1 : opcao) - 1});
    printf("%s", opcoes[opcao]);

    input = inputUsuario(numOpcoes);
  }

  if (opcao == CONFIG || opcao == SAIR)
    exit(0);

  return opcao;
}
