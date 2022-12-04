#include <windows.h>
#include <conio.h>

#include "menu.h"
#include "uteis.h"
#include "interface.h"

#define POS_Y(opcao, numConfig) (inicioOpcoes + (opcao >= numOpcoes - numConfig ? opcao + 1 : opcao) - 1)

/**
 * @brief Lida com a entrada do usuário em uma configuração
 *
 * @param opcoes Matriz de opções disponíveis
 * @param placeholder Texto em cinza que aparecerá como dica ao selecionar uma configuração
 * @param maxChars Número máximo de caracteres que o usuário pode digitar durante a configuração
 * @param indiceOpcao Índice da opção escolhida pelo usuário
 * @param inicioOpcoes Número da linha onde as opções começam no console
 * @param stdoutHandle Handle para o console
 * @return (int) Configuração digitada pelo usuário convertida pra números
 */
int coletaConfig(char opcoes[][TAM], char placeholder[], int maxChars, int indiceOpcao, int inicioOpcoes, HANDLE stdoutHandle);

/**
 * @brief Lida com a navegação e escolha de opções em um menu
 *
 * @param opcoes Matriz de opções
 * @param inicioOpcoes Número da linha onde se encontra a primeira opção no terminal
 * @param numOpcoes Número de opções disponíveis
 * @param numSeparadas Número de opções separadas das demais, como "Voltar", "Sair" ou "Configurações"
 * @return (int) Opção selecionada
 */
int handleMenuOptions(char opcoes[][TAM], int inicioOpcoes, int numOpcoes, int numSeparadas);

/**
 * @brief Coleta a entrada do usuário até que seja uma entrada válida
 *
 * @param numOpcoes Número de opções, se preciso
 * @param saiComEsc Valor booleano, determina se terminará o programa ao pressionar a tecla ESC
 * @return (int) Número correspondente ao caractere digitado
 */
int inputUsuario(int numOpcoes, int saiComEsc);

/**
 * @brief Verifica se o valor input é um dos valores definidos em INPUTS
 *
 * @param input Valor coletado através da função getch
 * @return int
 */
int verificaInput(INPUTS input, int numOpcoes);

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

int coletaConfig(char opcoes[][TAM], char placeholder[], int maxChars, int indiceOpcao, int inicioOpcoes, HANDLE stdoutHandle)
{
  char input[TAM];
  int i, baseY, baseX, flag = 1;

  baseX = inicioOpcoes + indiceOpcao - 1;
  baseY = MIN_ESPACO_LATERAL + strlen(opcoes[indiceOpcao]);

  SetConsoleCursorPosition(stdoutHandle, (COORD){baseY - 2, baseX});
  printf(": %s%s%s", COR_CINZA, placeholder, RESET);

  for (i = 0; i < maxChars && flag; i++)
  {
    input[i] = inputUsuario(0, 0);

    flag = input[i] != TECLA_ENTER && input[i] != TECLA_ESC;

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

int handleMenuOptions(char opcoes[][TAM], int inicioOpcoes, int numOpcoes, int numSeparadas)
{
  int opcaoAnt, opcao = 0;
  INPUTS input = inputUsuario(numOpcoes, 1);
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

    SetConsoleCursorPosition(stdoutHandle, (COORD){MIN_ESPACO_LATERAL, POS_Y(opcaoAnt, numSeparadas)});
    printf("%s  ", opcoes[opcaoAnt]);

    SetConsoleCursorPosition(stdoutHandle, (COORD){MIN_ESPACO_LATERAL, POS_Y(opcao, numSeparadas)});
    printf("%s", opcoes[opcao]);

    input = inputUsuario(numOpcoes, 1);
  }

  return opcao;
}

int inputUsuario(int numOpcoes, int saiComEsc)
{
  INPUTS input = getch();

  while (!verificaInput(input, numOpcoes))
    input = getch();

  if (saiComEsc && input == TECLA_ESC)
    exit(0);

  return input;
}

int verificaInput(INPUTS input, int numOpcoes)
{
  return (
      input == SETA_CIMA ||
      input == SETA_BAIXO ||
      input == SETA_DIREITA ||
      input == SETA_ESQUERDA ||
      input == TECLA_ENTER ||
      input == TECLA_ESC ||
      input == TECLA_W ||
      input == TECLA_W_CAPS ||
      input == TECLA_A ||
      input == TECLA_A_CAPS ||
      input == TECLA_S ||
      input == TECLA_S_CAPS ||
      input == TECLA_D ||
      input == TECLA_D_CAPS ||
      // input > NUM_ZERO && input < numOpcoes + NUM_ZERO + 1 ||
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