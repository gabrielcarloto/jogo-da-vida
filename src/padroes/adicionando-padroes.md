# Adicionando Padrões

Para adicionar um novo padrão, crie um arquivo `nome_do_padrao.csv` e escreva as posições das células vivas. Imagine como se estivesse criando uma matriz: cada linha é uma linha na matriz e cada número é a coluna dessa linha.

Após adicionar o arquivo, atualize o tipo `Padroes` e a macro `NUM_PADROES` em [padroes.h](../padroes.h) e o menu do jogo.

## Exemplos

- **Legenda:**
  - Células mortas: `·`
  - Células vivas: `X`

### Bloco

```
· · · ·
· X X ·
· X X ·
· · · ·
```

```c
// bloco.csv
0,1
0,1
```

### Sapo

```
· · · · · ·
· · X X X ·
· X X X · ·
· · · · · · 
```

```c
// sapo.csv
1,2,3
0,1,2
```

### Glider

```
· · · · ·
· X X X · 
· · · X ·
· · X · ·
· · · · ·
```

```c
// glider.csv
0,1,2
2
1
```