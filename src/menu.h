#ifndef _MENU_H_
#define _MENU_H_

#include "padroes.h"

typedef enum
{
  SETA_CIMA = 72,
  SETA_BAIXO = 80,
  SETA_ESQUERDA = 75,
  SETA_DIREITA = 7,
  TECLA_ENTER = 13,
  TECLA_ESC = 27,
  TECLA_W = 119,
  TECLA_W_CAPS = 87,
  TECLA_A = 97,
  TECLA_A_CAPS = 65,
  TECLA_S = 115,
  TECLA_S_CAPS = 83,
  TECLA_D = 100,
  TECLA_D_CAPS = 68,
  NUM_ZERO = 48,
  NUM_UM,
  NUM_DOIS,
  NUM_TRES,
  NUM_QUATRO,
  NUM_CINCO,
  NUM_SEIS,
  NUM_SETE,
  NUM_OITO,
  NUM_NOVE
} INPUTS;

typedef struct
{
  int ciclos;
  int linhas;
  int colunas;
  int cor_tema;
  int velocidade_atualizacao;
  Padroes padrao;
} Game_Settings;

/**
 * @brief Mostra o menu inicial do jogo e retorna as configurações inseridas pelo usuário
 *
 * @param settings Ponteiro para a variável de configurações do jogo
 */
void menuInicial(Game_Settings *settings);
#endif