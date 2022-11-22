# Jogo da Vida

## Como funciona o jogo

### O que é?

- Um jogo baseado em autômatos celulares que simula os processos de nascimento, sobrevivência e morte.
- Um ser vivo necessita de outros seres vivos para sobreviver e procriar.
- O excesso de indivíduos provoca a morte devido à escassez de comida.

#### Autômatos celulares

- Sistemas evolutivos baseados em regras simples.
- Formados por uma rede de células.
- **Principais características:**
  - *Paralelismo:* as células evoluem simultaneamente e independentes.
  - *Localidade:* a atualização do estado de uma célula depende do seu próprio estado atual e do estado atual das vizinhas.
  - *Homogeneidade:* as regras valem para todas as células.
- **Células Vizinhas:** células que estão na horizontal, vertical ou diagonal.

### Regras

- Uma célula viva morre de solidão se tiver menos de duas vizinhas vivas.
- Uma célula viva morre por superpopulação se tiver mais que três vizinhas vivas.
- Uma célula viva sobrevive se tiver duas ou três vizinhas vivas.
- Uma célula morta ganha vida se tiver exatamente três vizinhas vivas.
- São consideradas vizinhas células na horizontal, vertical e diagonal.

## Contribuição

Para detalhes sobre nosso fluxo de trabalho, acesse o arquivo [CONTRIBUTING](CONTRIBUTING.md).