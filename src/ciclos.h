#ifndef _CICLOS_H_
#define _CICLOS_H_
#include "shared.c"
#include "padroes.h"

/**
 * @brief Inicia um dos padrões disponíveis
 *
 * @param padrao Pode ser um número entre 1 e NUM_PADROES, ou o nome do padrão
 * @param ciclo A matriz que receberá o padrão
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 */
void iniciaPadrao(Padroes padrao, char **ciclo, int nl, int nc);

/* Retorna 0 se a célula está morta e 1 caso esteja viva */
int verificaCelula(const char celula);

/**
 * @brief Conta o número de células vizinhas vivas
 *
 * @param ciclo Matriz a se comparar as vizinhas
 * @param linha Linha da célula
 * @param coluna Coluna da célula
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 */
int verificaVizinhas(const char **ciclo, int linha, int coluna, int nl, int nc);

/**
 * @brief Possivelmente adicionará invasores em um ciclo. Se for adicionar, há 50%
 * de chance de inserir um padrão aleatório e 50% de chance de adicionar células
 * em lugares aleatórios.
 *
 * @param chance De 0 a 100, a chance de aparecer um invasor no ciclo atual
 * @param maxInvasores Caso insira células em lugares aleatórios, esse será o máximo possível
 * @param cicloAtual O ciclo a ser alterado
 * @param nl Número de linhas na matriz do ciclo
 * @param nc Número de colunas na matriz do ciclo
 */
void adicionaInvasores(const int chance, const int maxInvasores, char **cicloAtual, int nl, int nc);

/**
 * @brief Atualiza o ciclo atual com base nos dados do ciclo anterior
 *
 * @param cicloAnt Matriz do ciclo anterior
 * @param cicloAtual Matriz (do ciclo atual) que será atualizada
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
void atualizaMat(const char **cicloAnt, char **cicloAtual, int nl, int nc);
#endif