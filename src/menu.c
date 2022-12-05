#include <windows.h>
#include <conio.h>

#include "menu.h"
#include "uteis.h"
#include "console.h"
#include "interface.h"

#define POS_Y(opcao, numConfig) (inicioOpcoes + (opcao >= numOpcoes - numConfig ? opcao + 1 : opcao) - 1)

/**
 * @brief Lida com a entrada do usuário em uma configuração
 *
 * @param opcoes Matriz de opções disponíveis
 * @param placeholder Texto em cinza que aparecerá como dica ao selecionar uma configuração
 * @param maxChars Número máximo de caracteres que o usuário pode digitar durante a configuração
 * @param indiceOpcao Índice da opção escolhida pelo usuário
 * @param indiceSeta Índice da opção onde está a seta
 * @param inicioOpcoes Número da linha onde as opções começam no console
 * @param configAnterior Configuração anterior à do usuário
 * @param stdoutHandle Handle para o console
 * @return (int) Configuração digitada pelo usuário convertida pra números
 */
int coletaConfig(char opcoes[][TAM], char placeholder[], int maxChars, int indiceOpcao, int indiceSeta, int inicioOpcoes, int configAnterior, HANDLE stdoutHandle);

/**
 * @brief Lida com a navegação e escolha de opções em um menu
 *
 * @param opcoes Matriz de opções
 * @param inicioOpcoes Número da linha onde se encontra a primeira opção no terminal
 * @param numOpcoes Número de opções disponíveis
 * @param numSeparadas Número de opções separadas das demais, como "Voltar", "Sair" ou "Configurações"
 * @param indiceSeta Ponteiro que receberá o índice onde está a seta de seleção
 * @return (int) Opção selecionada
 */
int handleMenuOptions(char opcoes[][TAM], int inicioOpcoes, int numOpcoes, int numSeparadas, int *indiceSeta);

/**
 * @brief Mostra um menu com mais opções de padrões
 */
void maisPadroes(Game_Settings *settings);

/**
 * @brief Faz das opções uma lista ordenada (Ex.: ["opcao1", "opcao2"] -> ["1. opcao1", "2. opcao2"])
 *
 * @param opcoes Matriz de opcões
 */
void ordenaOpcoes(char opcoes[][TAM], int numOpcoes);

/**
 * @brief Coleta a entrada do usuário até que seja uma entrada válida
 *
 * @param saiComEsc Valor booleano, determina se terminará o programa ao pressionar a tecla ESC
 * @return (int) Número correspondente ao caractere digitado
 */
int inputUsuario(int saiComEsc);

/**
 * @brief Verifica se o valor input é um dos valores definidos em INPUTS
 *
 * @param input Valor coletado através da função getch
 * @return int
 */
int verificaInput(INPUTS input);

/**
 * @brief Mostra o menu de configurações do jogo e lida com as entradas do usuário
 *
 * @param settings Ponteiro para as configurações do jogo
 */
void configJogo(Game_Settings *settings);

/**
 * @brief Mostra o menu de seleção de cores e retorna a cor escolhida
 *
 * @param settings Ponteiro para as configurações do jogo
 * @return Cores
 */
Cores configCor(Game_Settings *settings);

void menuInicial(Game_Settings *settings)
{
  typedef enum
  {
    BLOCO,
    BLINKER,
    SAPO,
    GLIDER,
    LWSS,
    MAIS,
    CONFIG,
    SAIR
  } Opcoes;

  char opcoes[][TAM] = {"Bloco <", "Blinker", "Sapo", "Glider", "LWSS", "Mais padroes", "Configuracoes", "Sair do jogo"}; // "Pulsar", "Fogo de Artificio", "Fogos de Artificio", "Explosao",
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings signSettings;
  Opcoes opcao;

  ordenaOpcoes(opcoes, numOpcoes);

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 3;

  inicioOpcoes = imprimePlaca(
      signSettings,
      "MENU",
      "Escolha um dos padroes para iniciar o jogo:",
      " ",
      opcoes[BLOCO],
      opcoes[BLINKER],
      opcoes[SAPO],
      opcoes[GLIDER],
      opcoes[LWSS],
      opcoes[MAIS],
      " ",
      opcoes[CONFIG],
      opcoes[SAIR]);

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 2, NULL);

  if (opcao == MAIS)
    return maisPadroes(settings);
  else if (opcao == CONFIG)
    return configJogo(settings);
  else if (opcao == SAIR)
    exit(0);

  settings->padrao = opcao;
}

void maisPadroes(Game_Settings *settings)
{
  typedef enum
  {
    PULSAR, // 5
    FIREWORK,
    FIREWORKS,
    EXPLOSAO,
    CRESCIMENTO_INFINITO,
    BIG_A,
    VOLTAR
  } Opcoes;

  char opcoes[][TAM] = {"Pulsar <", "Fogo de Artificio", "Fogos de Artificio", "Explosao", "Crescimento Infinito", "Big A (A-lado)", "Voltar"};
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings signSettings;
  Opcoes opcao;

  ordenaOpcoes(opcoes, numOpcoes);

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 1;

  inicioOpcoes = imprimePlaca(
      signSettings,
      "MENU > MAIS PADROES",
      opcoes[PULSAR],
      opcoes[FIREWORK],
      opcoes[FIREWORKS],
      opcoes[EXPLOSAO],
      opcoes[CRESCIMENTO_INFINITO],
      opcoes[BIG_A],
      " ",
      opcoes[VOLTAR]);

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 1, NULL);

  if (opcao == VOLTAR)
    return menuInicial(settings);

  settings->padrao = opcao + 5;
}

void configJogo(Game_Settings *settings)
{
  typedef enum
  {
    ATUALIZACAO,
    CICLOS,
    LINHAS,
    COLUNAS,
    CHANCE_INVASORES,
    NUMERO_INVASORES,
    COR,
    VOLTAR
  } Opcoes_Config;

  char opcoes[][TAM] = {"Tempo de Atualizacao <", "Ciclos", "Linhas", "Colunas", "Chance dos Invasores", "Numero de Invasores", "Cor", "Voltar"}, placeholder[TAM];
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]), indiceSeta;
  HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  Sign_Settings signSettings;
  Opcoes_Config opcao;

  ordenaOpcoes(opcoes, numOpcoes);

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 3;

  ClearScreen();

  inicioOpcoes = imprimePlaca(
      signSettings,
      "CONFIGURACOES",
      "Selecione o que quiser configurar",
      " ",
      opcoes[ATUALIZACAO],
      opcoes[CICLOS],
      opcoes[LINHAS],
      opcoes[COLUNAS],
      opcoes[CHANCE_INVASORES],
      opcoes[NUMERO_INVASORES],
      opcoes[COR],
      " ",
      opcoes[VOLTAR]);

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 1, &indiceSeta);

  switch (opcao)
  {
  case ATUALIZACAO:
    snprintf(placeholder, TAM, "%dms", settings->velocidade_atualizacao);
    settings->velocidade_atualizacao = coletaConfig(opcoes, placeholder, 4, ATUALIZACAO, indiceSeta, inicioOpcoes, settings->velocidade_atualizacao, stdoutHandle);
    break;
  case CICLOS:
    snprintf(placeholder, TAM, "%d", settings->ciclos);
    settings->ciclos = coletaConfig(opcoes, placeholder, 4, CICLOS, indiceSeta, inicioOpcoes, settings->ciclos, stdoutHandle);
    break;
  case LINHAS:
    snprintf(placeholder, TAM, "%d", settings->linhas);
    int linhas = coletaConfig(opcoes, placeholder, 3, LINHAS, indiceSeta, inicioOpcoes, settings->linhas, stdoutHandle);
    DWORD scrHeight = GetSystemMetrics(SM_CYSCREEN);

    linhas = linhas < scrHeight / 18 ? linhas : scrHeight / 18;

    settings->linhas = linhas;
    break;
  case COLUNAS:
    snprintf(placeholder, TAM, "%d", settings->colunas);
    int colunas = coletaConfig(opcoes, placeholder, 3, COLUNAS, indiceSeta, inicioOpcoes, settings->colunas, stdoutHandle);
    DWORD scrWidth = GetSystemMetrics(SM_CXSCREEN);

    colunas = colunas < scrWidth / 17 ? colunas : scrWidth / 17;

    settings->colunas = colunas;
    break;
  case CHANCE_INVASORES:
    snprintf(placeholder, TAM, "%d", settings->chance_invasores);
    int chance = coletaConfig(opcoes, placeholder, 3, CHANCE_INVASORES, indiceSeta, inicioOpcoes, settings->chance_invasores, stdoutHandle);
    settings->chance_invasores = chance > 100 ? 100 : chance;
    break;
  case NUMERO_INVASORES:
    snprintf(placeholder, TAM, "%d", settings->numero_invasores);
    settings->chance_invasores = coletaConfig(opcoes, placeholder, 3, NUMERO_INVASORES, indiceSeta, inicioOpcoes, settings->numero_invasores, stdoutHandle);
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

Cores configCor(Game_Settings *settings)
{
  char opcoes[][TAM] = {"Azul <", "Verde", "Amarelo", "Vermelho", "Voltar"}, opcao;
  int inicioOpcoes, numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
  Sign_Settings signSettings;

  ordenaOpcoes(opcoes, numOpcoes);

  signSettings.alignment = LEFT;
  signSettings.maxHeight = 20;
  signSettings.maxWidth = 75;
  signSettings.firstOptionIndex = 3;

  ClearScreen();

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

  opcao = handleMenuOptions(opcoes, inicioOpcoes, numOpcoes, 1, NULL);

  if (opcao == numOpcoes - 1)
  {
    configJogo(settings);
    return VERDE;
  }

  return opcao;
}

int coletaConfig(char opcoes[][TAM], char placeholder[], int maxChars, int indiceOpcao, int indiceSeta, int inicioOpcoes, int configAnterior, HANDLE stdoutHandle)
{
  char input[TAM];
  int i, baseY, baseX, setaMesmoIndice, flag = 1;

  baseX = MIN_ESPACO_LATERAL + strlen(opcoes[indiceOpcao]);
  baseY = inicioOpcoes + indiceOpcao - 1;

  setaMesmoIndice = indiceSeta == indiceOpcao;

  if (!setaMesmoIndice)
  {
    SetConsoleCursorPosition(stdoutHandle, (COORD){strlen(opcoes[indiceSeta]), inicioOpcoes + indiceSeta - 1});
    printf("  ");
  }

  SetConsoleCursorPosition(stdoutHandle, (COORD){baseX - (setaMesmoIndice ? 2 : 0), baseY});
  printf(": %s%s%s", COR_CINZA, placeholder, RESET);

  for (i = 0; i < maxChars && flag; i++)
  {
    input[i] = inputUsuario(0);

    flag = input[i] != TECLA_ENTER && input[i] != TECLA_ESC;

    if (i == 0)
    {
      SetConsoleCursorPosition(stdoutHandle, (COORD){baseX, baseY});
      printf(": %*s", maxChars + 2, "");
      SetConsoleCursorPosition(stdoutHandle, (COORD){baseX + (!setaMesmoIndice ? 2 : 0), baseY});
    }
    else
      SetConsoleCursorPosition(stdoutHandle, (COORD){baseX + (!setaMesmoIndice ? 2 : 0) + i, baseY});
    printf("%c", input[i]);

    if (i == maxChars - 1 && flag)
      Sleep(80); // mostra para o usuário o último input
  }

  if (!(input[0] >= NUM_ZERO && input[0] <= NUM_NOVE) || input[0] == TECLA_ENTER || input[0] == TECLA_ESC)
    return configAnterior;

  return atoi(input);
}

int handleMenuOptions(char opcoes[][TAM], int inicioOpcoes, int numOpcoes, int numSeparadas, int *indiceSeta)
{
  int opcaoAnt, opcao = 0;
  INPUTS input = inputUsuario(TRUE);
  HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

  while (input != TECLA_ENTER)
  {
    opcaoAnt = opcao;

    if (input == SETA_BAIXO || input == TECLA_S || input == TECLA_S_CAPS)
      opcao += (opcao < numOpcoes - 1);
    else if (input == SETA_CIMA || input == TECLA_W || input == TECLA_W_CAPS)
      opcao -= (opcao != 0);

    if (input - NUM_ZERO > 0 && input - NUM_ZERO <= numOpcoes)
    {
      if (indiceSeta != NULL)
        *indiceSeta = opcao;
      return input - NUM_ZERO - 1;
    }

    if (opcao != opcaoAnt)
    {
      opcoes[opcaoAnt][strlen(opcoes[opcaoAnt]) - 2] = '\0';
      strcat(opcoes[opcao], " <");
    }

    SetConsoleCursorPosition(stdoutHandle, (COORD){MIN_ESPACO_LATERAL, POS_Y(opcaoAnt, numSeparadas)});
    printf("%s  ", opcoes[opcaoAnt]);

    SetConsoleCursorPosition(stdoutHandle, (COORD){MIN_ESPACO_LATERAL, POS_Y(opcao, numSeparadas)});
    printf("%s", opcoes[opcao]);

    input = inputUsuario(1);
  }

  if (indiceSeta != NULL)
    *indiceSeta = opcao;

  return opcao;
}

void ordenaOpcoes(char opcoes[][TAM], int numOpcoes)
{
  int i;
  char copiaOpcao[TAM];

  for (i = 0; i < numOpcoes; i++)
  {
    strcpy(copiaOpcao, opcoes[i]);
    snprintf(opcoes[i], TAM, "%d. %s", i + 1, copiaOpcao);
  }
}

int inputUsuario(int saiComEsc)
{
  INPUTS input = getch();

  while (!verificaInput(input))
    input = getch();

  if (saiComEsc && input == TECLA_ESC)
    exit(0);

  return input;
}

int verificaInput(INPUTS input)
{
  return (
      input == SETA_CIMA ||
      input == SETA_BAIXO ||
      input == TECLA_ENTER ||
      input == TECLA_ESC ||
      input == TECLA_W ||
      input == TECLA_W_CAPS ||
      input == TECLA_S ||
      input == TECLA_S_CAPS ||
      input == NUM_ZERO ||
      input == NUM_UM ||
      input == NUM_DOIS ||
      input == NUM_TRES ||
      input == NUM_QUATRO ||
      input == NUM_CINCO ||
      input == NUM_SEIS ||
      input == NUM_SETE ||
      input == NUM_OITO ||
      input == NUM_NOVE);
}