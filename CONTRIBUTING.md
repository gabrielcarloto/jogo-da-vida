# Guia de Contribuição

Estou deixando esse arquivo como uma referência rápida para os comandos do git e o fluxo de trabalho.

## Tabela de Conteúdos

- [Guia de Contribuição](#guia-de-contribuição)
  - [Tabela de Conteúdos](#tabela-de-conteúdos)
  - [Resumo](#resumo)
  - [Primeiros passos](#primeiros-passos)
  - [Modificando arquivos](#modificando-arquivos)
  - [Tarefas](#tarefas)

## Resumo

1. Crie seu fork
2. Crie sua branch
3. Faça suas mudanças e commits.
4. Crie um pull request.
5. Repita os passos 3 e 4 até terminarmos o trabalho.

## Primeiros passos

A primeira coisa a se fazer é clonar o repositório para a sua conta. Isso é chamado **fork**. Para fazer isso, acesse o [repositório no github](https://github.com/gabrielcarloto/jogo-da-vida) e, no canto superior direito, logo abaixo das notificações e da foto de perfil, clique em **Fork**.

![](https://ik.imagekit.io/698xlahbaqz/Control-V_F6D6HpQ9C.png?ik-sdk-version=javascript-1.4.3&updatedAt=1668989655752)

Quando o repositório estiver na sua conta, clique no botão verde **code** e, na seção *clone*, copie o link HTTPS.

![](https://ik.imagekit.io/698xlahbaqz/Captura_de_tela_2022-11-20_212038_mKxOEYrj8.png?ik-sdk-version=javascript-1.4.3&updatedAt=1668990057228)

No seu computador, abra o terminal numa pasta (ou o vscode em uma pasta e abra o terminal dentro dele no canto superior esquerdo em Terminal > New Terminal) e digite o comando

```bash
git clone <link>
```

Com isso, o repositório será clonado para a sua máquina local.

## Modificando arquivos

Com o repositório clonado em sua máquina, crie uma ramificação do código (aka **branch**) usando o comando

```bash
git checkout -b <nome-do-branch>
```

Esse comando irá criar a branch e automaticamente mudar para ela (se quiser apenas mudar para uma branch existente, apenas tire o `-b`).

Ou, pelo vscode, abra a paleta de comando (CTRL+SHIFT+P) e selecione "Git: Create Branch...", por fim digite o nome da branch.

Para verificar em qual branch está, digite (no terminal)

```bash
git branch
```

Pelo vscode, no canto inferior esquedo você pode ver a branch que está e mudar de branch também.

![](https://ik.imagekit.io/698xlahbaqz/Control-V_1__Lt_yOhiow.png?ik-sdk-version=javascript-1.4.3&updatedAt=1669075593689)

![](https://ik.imagekit.io/698xlahbaqz/Captura_de_tela_2022-11-21_210716_sfdFVPCFl.png?ik-sdk-version=javascript-1.4.3&updatedAt=1669075657551)

Assim que você estiver na nova branch, pode fazer as mudanças nos arquivos. Não se esqueça de fazer um commit a cada mudança:

```bash
git add . // adiciona todos os arquivos
git commit -m <mensagem>
```

Pelo vscode, você pode abrir a aba *Source Control*, selecionar os arquivos que quer adicionar ao commit, escrever a mensagem e commitar.

![](https://ik.imagekit.io/698xlahbaqz/Captura_de_tela_2022-11-21_211729_TYXryeIay.png?ik-sdk-version=javascript-1.4.3&updatedAt=1669076272990)

>Obs.: é ideal que a mensagem do commit seja curta (cerca de 50 caracteres), escrita de forma imperativa. Por exemplo, ao adicionar uma função, uma mensagem poderia ser "adiciona <nome da função>".

>Caso esteja em dúvida sobre o quão frequentes devem ser os commits, tenha em mente que não é correto commitar a cada letra digitada, nem é ideal commitar após **muitas** mudanças. Exemplos de quando commitar: ao adicionar uma nova função, corrigir um erro ou reescrever uma função.

Quando você tiver um ou mais commits e quer me enviar esse código, primeiro deve sincronizar seus commits locais com seu repositório. Para isso, basta um simples comando:

```bash
git push
```

Pelo vscode, na mesma aba de Source Control, você pode clicar nos três pontinhos logo ao lado do título Source Control e clicar em push (lá também tem outros comandos e se vc não tiver nenhuma alteração após o último commit ele mostrará um botão para sincronizar com a origem no lugar de "commit").

É bom de vez em quando sincronizar seu fork com o meu repositório, a fim de evitar conflitos. Para fazer isso, vá ao seu repositório e clique nesse botão:

![](https://ik.imagekit.io/698xlahbaqz/Captura_de_tela_2022-11-21_212834_bDEe7EYau.png?ik-sdk-version=javascript-1.4.3&updatedAt=1669076949860)

Para enfim enviar o código para o repositório original, onde irei revisá-lo para verificar erros ou sugerir algo antes de juntar (fazer um merge / mergear) com a minha branch, basta clicar no botão *Contribute*, ao lado de *Sync fork* no print anterior, e então *Create pull request*.

Procure dar um nome descritivo ao pull request, destacando as mudanças feitas. Se necessário, use o corpo para escrever uma mensagem mais elaborada. Caso haja um ou mais issues relacionados às mudanças que fez, é bom citá-los também (basta digitar # e o número do issue).

## Tarefas

Para evitar deixar tudo perdido no whatsapp, utilizaremos o github para gerenciar e designar tarefas, que serão criadas através de *issues* (já existem algumas iniciais que podem tomar como exemplo).

Vocês podem ver essas tarefas tanto na aba issues quanto na aba *projects* do github (este último possibilita uma visão em kanban e outra em lista de todas as tarefas).