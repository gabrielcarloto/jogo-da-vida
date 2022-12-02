#include "shared.c"

/*/função que lê arquivo csv, e bota na matriz
opcao = opção do menu
matMain = matriz que será preenchida
nl = número de linhas
nc = número de colunas
xInic = coordenada da linha
yInic = coordenada da coluna
/*/

typedef enum
{
  BLOCO,
  BLINKER,
  SAPO,
  GLIDER,
  LWSS,
} Padroes;

void lePadrao(Padroes opcao, char **matMain, int nl, int nc, int xInic, int yInic)
{
  FILE *arquivo;
  char string[50], **padrao, *endereco; // se tiver algum padrão com mais de 50 colunas de largura da ruim
  int i, j, dy, terms;

  for (i = 0; i < nl; i++) // inicialmente preenche a matriz com células mortas
    for (j = 0; j < nc; j++)
      matMain[i][j] = '.';

  i = 0; // resetando o i

  if (xInic == -1 || yInic == -1)
  {
    xInic = (nl / 2) - 1;
    yInic = (nc / 2) - 1;
  }

  switch (opcao)
  {
  case BLOCO:
    endereco = "padroes/bloco.csv";
    break;
  case BLINKER:
    endereco = "padroes/blinker.csv";
    break;
  case SAPO:
    endereco = "padroes/sapo.csv";
    break;
  case GLIDER:
    endereco = "padroes/glider.csv";
    break;
  case LWSS:
    endereco = "padroes/LWSS.csv";
    break;
  }

  arquivo = fopen(endereco, "r"); // abre o arquivo em questão

  while (fgets(string, 50, arquivo)) // lê linha por linha
  {
    padrao = split(string, ",", &terms);

    for (j = 0; j < terms; j++)
    {
      dy = atoi(padrao[j]); // converte a palavra para um inteiro

      if (xInic + i > nl - 1 || yInic + dy > nc) // caso ele tente preencher pra fora da matriz
        continue;

      matMain[xInic + i][yInic + dy] = 'X';
    }

    i++; // incrementa o i a cada iteração
  }
}
