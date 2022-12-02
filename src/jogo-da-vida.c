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
int inputUsuario(int numOpcoes);

int main()
{
  int nL = 20, nC = 20, nCiclos = 50; // ou fornecidos pelo usuario
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
  imprimeMatriz(mAtual, nL, nC);
  // getchar();
  Sleep(100);

  mAnt = alocaMatriz(nL, nC);

  for (c = 1; c <= nCiclos; c++)
  {
    copiaMatriz(mAtual, mAnt, nL, nC);

    atualizaMat(mAnt, mAtual, nL, nC);
    // apagaTela(nL);
    gotoxy(0, 0);
    imprimeMatriz(mAtual, nL, nC);
    // getchar();
    Sleep(150);
  }

  desalocaMatriz(mAnt, nL);
  resetaConsole();
}

int inputUsuario(int numOpcoes)
{
  int input = getch();

  while (!verificaInput(input, numOpcoes))
    input = getch();

  return input;
}

int imprimeOpcoesMenu(Sign_Settings config, const char opcoes[][TAM], int tamanho)
{
  int alturaTerminal;

  alturaTerminal = imprimePlaca(config, "MENU", "Escolha um dos padroes para iniciar o jogo:", " ", opcoes[0], opcoes[1], opcoes[2], opcoes[3], opcoes[4], " ", opcoes[tamanho - 2], opcoes[tamanho - 1]);

  return alturaTerminal / 2 - tamanho - (alturaTerminal % 2);
}

void menuInicJogo(char **mat, int nL, int nC)
{
  char opcoes[][TAM] = {"1. Bloco <", "2. Blinker", "3. Sapo", "4. Glider", "5. LWSS", "6. Configuracoes", "7. Sair do jogo"}, input;
  int opcao = 0, opcaoAnt, inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings set;

  set.alignment = LEFT;
  set.maxHeight = 20;
  set.maxWidth = 75;

  inicioOpcoes = imprimeOpcoesMenu(set, opcoes, numOpcoes);

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

    gotoxy(3, inicioOpcoes + (opcaoAnt >= numOpcoes - 2 ? opcaoAnt + 1 : opcaoAnt));
    printf("%s  ", opcoes[opcaoAnt]);

    gotoxy(3, inicioOpcoes + (opcao >= numOpcoes - 2 ? opcao + 1 : opcao));
    printf("%s", opcoes[opcao]);

    input = inputUsuario(numOpcoes);
  }

  if (opcao == 6 || opcao == 5)
    exit(0);

  limpaMatriz(mat, nL, nC);
  iniciaPadrao(opcao + 1, mat, nL, nC);
  apagaTela(0);
  imprimeMatriz(mat, nL, nC);

  printf("%sPressione qualquer tecla para iniciar...", RESET);
  getch();
}
