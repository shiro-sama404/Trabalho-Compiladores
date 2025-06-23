# Compilador MiniJava Modificado - Análise Léxica, Sintática e Semântica

[cite_start]Este repositório contém a implementação de um compilador para a linguagem MiniJava modificada, desenvolvido como parte da disciplina de Compiladores I (2025/1) da FACOM - Universidade Federal de Mato Grosso do Sul (UFMS).

## 1. Descrição do Projeto

Este trabalho prático aborda as três primeiras etapas do processo de compilação:

* [cite_start]**Análise Léxica:** Responsável por ler o código-fonte e convertê-lo em uma sequência de tokens (unidades atômicas da linguagem).
* [cite_start]**Análise Sintática:** Implementada como um analisador sintático descendente preditivo, verifica se a sequência de tokens obedece às regras gramaticais da linguagem MiniJava modificada (Gramática fornecida na Seção 6 do `etapa2_analise_sintatica.pdf`). Possui estratégias de recuperação de erros (modo pânico) para continuar a análise após a detecção de um erro sintático.
* **Análise Semântica:** Realizada concomitantemente à análise sintática, com o auxílio de uma Tabela de Símbolos. [cite_start]As funcionalidades semânticas implementadas incluem:
    * [cite_start]Inserção de informações de classes, variáveis e métodos na Tabela de Símbolos do escopo adequado.
    * [cite_start]Verificação da existência e acessibilidade de variáveis e métodos a partir do escopo corrente (incluindo busca em escopos pai).
    * [cite_start]Verificação de compatibilidade de tipos em atribuições (tipo da expressão à direita igual ao da esquerda).
    * [cite_start]Validação de operadores aritméticos (apenas para operandos inteiros) e lógicos (`&&` apenas para operandos booleanos).

## 2. Como Compilar

[cite_start]O programa será compilado utilizando o compilador `g++`.

Navegue até o diretório raiz do projeto (onde estão as pastas `src/` e `tests/`).
Execute o seguinte comando:

```bash
g++ *.cpp -o mj_compiler
./mj_compiler <nome_do_arquivo>.mj

[cite_start]Exemplo:

```bash
./mj_compiler tests/01_hello_world.mj