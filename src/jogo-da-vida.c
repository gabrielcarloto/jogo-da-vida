#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "shared.c"
#include "ciclos.c"
#include "interface.c"

#define DURACAO_PADRAO

#define TAM 101
#define POS_Y(opcao, numConfig) (inicioOpcoes + (opcao >= numOpcoes - numConfig ? opcao + 1 : opcao) - 1)

typedef struct
{
  int ciclos;
  int linhas;
  int colunas;
  int cor_tema;
  int velocidade_atualizacao;
  Padroes padrao;
} Game_Settings;

void jogaJogoVida(char **mAtual, Game_Settings *settings);
void menuInicial(Game_Settings *settings);
int inputUsuario(int numOpcoes);

int main()
{
  Sign_Settings signSettings;
  Game_Settings gameSettings;
  Terminal_Size tsize;
  char **mat;

  atexit(resetaConsole);
  setupConsole();
  srand(time(NULL));

  gameSettings.velocidade_atualizacao = 150;
  gameSettings.cor_tema = VERDE;
  gameSettings.ciclos = 50;
  gameSettings.linhas = 0;
  gameSettings.colunas = 0;

  signSettings.alignment = CENTER;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;

  imprimePlaca(signSettings, "INSTRUCOES", "Navegue com as setas ou numeros do teclado", "Pressione enter para continuar");
  getch();

  // inicio laço indeterminado
  menuInicial(&gameSettings);

  if (!gameSettings.linhas || !gameSettings.colunas)
  {
    resizeWindow(0, 0);
    tamanhoTerminal(&tsize);

    gameSettings.linhas = tsize.height - 5;
    gameSettings.colunas = tsize.width / 2;
  }

  mat = alocaMatriz(gameSettings.linhas, gameSettings.colunas);
  limpaMatriz(mat, gameSettings.linhas, gameSettings.colunas);
  iniciaPadrao(gameSettings.padrao + 1, mat, gameSettings.linhas, gameSettings.colunas);
  clear();
  imprimeMatriz(mat, gameSettings.linhas, gameSettings.colunas, gameSettings.cor_tema);

  printf("%sPressione qualquer tecla para iniciar...", RESET);
  getch();

  jogaJogoVida(mat, &gameSettings);
  // fim do laco indeterminado

  desalocaMatriz(mat, gameSettings.linhas);
}

void jogaJogoVida(char **mAtual, Game_Settings *settings)
{
  int c, nL = settings->linhas, nC = settings->colunas;
  char **mAnt;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

  // imprimindo na tela a matriz inicial
  clear();
  imprimeMatriz(mAtual, nL, nC, settings->cor_tema);
  // getchar();
  Sleep(settings->velocidade_atualizacao);

  mAnt = alocaMatriz(nL, nC);
  DEBUG();

  for (c = 1; c <= settings->ciclos; c++)
  {
    copiaMatriz(mAtual, mAnt, nL, nC);

    atualizaMat(mAnt, mAtual, nL, nC);
    SetConsoleCursorPosition(h, (COORD){0, 0});
    imprimeMatriz(mAtual, nL, nC, settings->cor_tema);
    // getchar();
    Sleep(settings->velocidade_atualizacao);
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
  INPUTS input = getch();

  while (!verificaInput(input, numOpcoes))
    input = getch();

  if (input == TECLA_ESC)
    exit(0);

  return input;
}

int handleMenuOptions(char opcoes[][TAM], int inicioOpcoes, int numOpcoes, int numConfig)
{
  int opcaoAnt, opcao = 0;
  INPUTS input = inputUsuario(numOpcoes);
  HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

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

    SetConsoleCursorPosition(stdoutHandle, (COORD){MIN_ESPACO_LATERAL, POS_Y(opcaoAnt, numConfig)});
    printf("%s  ", opcoes[opcaoAnt]);

    SetConsoleCursorPosition(stdoutHandle, (COORD){MIN_ESPACO_LATERAL, POS_Y(opcao, numConfig)});
    printf("%s", opcoes[opcao]);

    input = inputUsuario(numOpcoes);
  }

  return opcao;
}

int coletaConfig(char opcoes[][TAM], char placeholder[], int maxChars, int indiceOpcao, int inicioOpcoes, HANDLE stdoutHandle)
{
  char input[TAM];
  int i, baseY, baseX;

  baseX = inicioOpcoes + indiceOpcao - 1;
  baseY = MIN_ESPACO_LATERAL + strlen(opcoes[indiceOpcao]);

  SetConsoleCursorPosition(stdoutHandle, (COORD){baseY - 2, baseX});
  printf(": %s%s%s", COR_CINZA, placeholder, RESET);

  for (i = 0; (i < maxChars) && input != TECLA_ENTER && input != TECLA_ESC; i++)
  {
    input[i] = getch();
    if (i == 0)
    {
      SetConsoleCursorPosition(stdoutHandle, (COORD){baseY - 2, baseX});
      printf(": %*s", maxChars + 2, "");
      SetConsoleCursorPosition(stdoutHandle, (COORD){baseY, baseX});
    }
    else
      SetConsoleCursorPosition(stdoutHandle, (COORD){baseY + i, baseX});
    printf("%c", input[i]);
  }

  return atoi(input);
}

void configJogo(Game_Settings *settings);

Cores configCor(Game_Settings *settings)
{
  char opcoes[][TAM] = {"1. Azul <", "2. Verde", "3. Amarelo", "4. Vermelho", "5. Voltar"}, opcao;
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings signSettings;

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 3;

  system("cls");

  inicioOpcoes = imprimePlaca(
      signSettings,
      "CONFIG. > COR",
      "Selecione a cor desejada",
      " ",
      opcoes[AZUL],
      opcoes[VERDE],
      opcoes[AMARELO],
      opcoes[VERMELHO],
      " ",
      opcoes[numOpcoes - 1]);

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 1);

  if (opcao == numOpcoes - 1)
  {
    configJogo(settings);
    return VERDE;
  }

  return opcao;
}

void configJogo(Game_Settings *settings)
{
  typedef enum
  {
    ATUALIZACAO,
    CICLOS,
    LINHAS,
    COLUNAS,
    COR,
    VOLTAR
  } Opcoes_Config;

  char opcoes[][TAM] = {"1. Tempo de Atualizacao <", "2. Ciclos", "3. Linhas", "4. Colunas", "5. Cor", "6. Voltar"}, placeholder[TAM];
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  Sign_Settings signSettings;
  Opcoes_Config opcao;

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 3;

  system("cls");

  inicioOpcoes = imprimePlaca(
      signSettings,
      "CONFIGURACOES",
      "Selecione o que quiser configurar",
      " ",
      opcoes[ATUALIZACAO],
      opcoes[CICLOS],
      opcoes[LINHAS],
      opcoes[COLUNAS],
      opcoes[COR],
      " ",
      opcoes[VOLTAR]);

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 1);

  switch (opcao)
  {
  case ATUALIZACAO:
    snprintf(placeholder, TAM, "%dms", settings->velocidade_atualizacao);
    settings->velocidade_atualizacao = coletaConfig(opcoes, placeholder, 4, ATUALIZACAO, inicioOpcoes, stdoutHandle);
    break;
  case CICLOS:
    snprintf(placeholder, TAM, "%d", settings->ciclos);
    settings->ciclos = coletaConfig(opcoes, placeholder, 4, CICLOS, inicioOpcoes, stdoutHandle);
    break;
  case LINHAS:
    snprintf(placeholder, TAM, "%d", settings->linhas);
    settings->linhas = coletaConfig(opcoes, placeholder, 3, LINHAS, inicioOpcoes, stdoutHandle);
    break;
  case COLUNAS:
    snprintf(placeholder, TAM, "%d", settings->colunas);
    settings->colunas = coletaConfig(opcoes, placeholder, 3, COLUNAS, inicioOpcoes, stdoutHandle);
    break;
  case COR:
    settings->cor_tema = configCor(settings);
    break;
  case VOLTAR:
    menuInicial(settings);
    return;
  }

  configJogo(settings);
}

void menuInicial(Game_Settings *settings)
{
  typedef enum
  {
    PADRAO_1,
    PADRAO_2,
    PADRAO_3,
    PADRAO_4,
    PADRAO_5,
    CONFIG,
    SAIR
  } Opcoes;

  char opcoes[][TAM] = {"1. Bloco <", "2. Blinker", "3. Sapo", "4. Glider", "5. LWSS", "6. Configuracoes", "7. Sair do jogo"};
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings signSettings;
  Opcoes opcao;

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 3;

  inicioOpcoes = imprimePlaca(
      signSettings,
      "MENU",
      "Escolha um dos padroes para iniciar o jogo:",
      " ",
      opcoes[PADRAO_1],
      opcoes[PADRAO_2],
      opcoes[PADRAO_3],
      opcoes[PADRAO_4],
      opcoes[PADRAO_5],
      " ",
      opcoes[CONFIG],
      opcoes[SAIR]);

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 2);

  if (opcao == CONFIG)
  {
    configJogo(settings);
    return;
  }
  else if (opcao == SAIR)
    exit(0);

  settings->padrao = opcao;
}
