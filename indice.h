#include "arvoreB.h"

// --- Funções de Indice --- //

// Função para criar o índice a partir de um arquivo de dados
void criarIndice(char *nomeArquivo, arvoreB *arv);

// Função para gerar um nome aleatório
void gerarNome(char *nome, int tamanho);

// Função para gerar um curso aleatório
const char* gerarCurso();

// Função para verificar se a matrícula já foi usada
int matriculaJaUsada(int matricula, int *matriculas, int tamanho);

// Função para gerar os registros de alunos e salvar em um arquivo .txt
void gerarRegistros();

// Imprime o Registro na Linha do Arquivo
void imprimirRegistro(int linhaArq, char *nomeArquivo, int tamReg);

void comparacao(char *nomeArquivo, int quant, int tamReg, arvoreB *arv);