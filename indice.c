#include "arvoreB.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_REGISTROS 20000

// --- Funções de Indice --- //

// Função para gerar um nome aleatório
void gerarNome(char *nome, int tamanho) {
    const char *consoantes = "bcdfghjklmnpqrstvwxyz";
    const char *vogais = "aeiou";

    for (int i = 0; i < tamanho; i++) {
        if (i % 2 == 0) {
            nome[i] = consoantes[rand() % 21];
        } else {
            nome[i] = vogais[rand() % 5];
        }
    }
    nome[tamanho] = '\0';
}

// Função para gerar um curso aleatório
const char* gerarCurso() {
    const char *cursos[] = {"CCO", "SIN", "ECO"};
    return cursos[rand() % 3];
}

// Função para verificar se a matrícula já foi usada
int matriculaJaUsada(int matricula, int *matriculas, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (matriculas[i] == matricula) {
            return 1;
        }
    }
    return 0;
}

// Função para gerar os registros de alunos e salvar em um arquivo .txt
void gerarRegistros(char *nomeArq) {
    FILE *file;
    file = fopen(nomeArq, "a+");

    srand(time(0));

    int *matriculas = (int *)malloc(NUM_REGISTROS * sizeof(int));
    int numGerados = 0;

    for (int i = 0; i < NUM_REGISTROS; i++) {
        int matricula;
        do {
            matricula = rand() % 1000000;
        } while (matriculaJaUsada(matricula, matriculas, numGerados));

        matriculas[numGerados++] = matricula;

        int idade = (rand() % 13) + 18; // Idades entre 18 e 30
        char nome[11];
        gerarNome(nome, 10); // Gera um nome de 10 caracteres
        const char *curso = gerarCurso();

        fprintf(file, "%06d %s %d %s\n", matricula, nome, idade, curso);
    }

    free(matriculas);
    fclose(file);
    printf("Arquivo %s gerado com sucesso!\n", nomeArq);
}

// Imprime o Registro na Linha do Arquivo
void imprimirRegistro(int linhaArq, char *nomeArquivo, int tamReg) {
    FILE *file;
    int mat;
    int idade;
    char nome[30];
    char curso[4];
    
    file = fopen(nomeArquivo, "r");
    
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
    }
    
    fseek(file, (linhaArq * tamReg) - 1, SEEK_SET);
    
    fscanf(file, "%d %s %d %s", &mat, nome, &idade, curso);
    
    printf("Matrícula: %d\n", mat);
    printf("Nome: %s\n", nome);
    printf("Idade: %d\n", idade);
    printf("Curso: %s\n", curso);
    
    fclose(file);
}

// Função para criar o índice a partir de um arquivo de dados
void criarIndice(char *nomeArquivo, arvoreB *arv) {
    FILE *file = fopen(nomeArquivo, "r");
    char resposta;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        printf("Deseja criar um novo arquivo aleatório? (s/n): ");
        scanf(" %c", &resposta);

        if (resposta == 's' || resposta == 'S') {
            gerarRegistros(nomeArquivo);
            criarIndice(nomeArquivo, arv);
        }

        return;
    }

    char linha[100];
    int numLinha = 0;

    while (fgets(linha, sizeof(linha), file)) {
        int matricula;
        char nome[50];
        int idade;
        char curso[4];

        sscanf(linha, "%d %s %d %s", &matricula, nome, &idade, curso);
        chave chv = criarChave(matricula, numLinha);
        inserirChave(chv, arv);
        numLinha++;
    }

    fclose(file);
    printf("Índice criado com sucesso a partir do arquivo '%s'\n", nomeArquivo);
}

void comparacao(char *nomeArquivo, int quant, int tamReg, arvoreB *arv) {
  FILE *arq;
  int i, pos;
  int mat;
  int idade;
  int pesquisa;
  int linhaArq;
  char nome[30];
  char curso[4];
  double tempo, tempoDif;
  double totalVA = 0, minVA = 0, maxVA = 0;
  double totalBT = 0, minBT = 0, maxBT = 0;

  arq = fopen(nomeArquivo, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o arquivo");
  }

  srand(time(0));

  for (i = 0; i < quant; i++) {

    // Gera uma matricula existente a ser pesquisada
    pesquisa = rand() % 20000;
    fseek(arq, (pesquisa * 25) - 1, SEEK_SET);
    fscanf(arq, "%d %s %d %s", &pesquisa, nome, &idade, curso);
    rewind(arq);

    // Pesquisa Varrendo o Arquivo
    tempo = (double)clock() / (CLOCKS_PER_SEC / 1000);
    while (pesquisa != mat) {
      fscanf(arq, "%d %s %d %s", &mat, nome, &idade, curso);

      if (feof(arq))
        break;
    }
    tempoDif = (double)clock() / (CLOCKS_PER_SEC / 1000) - tempo;

    // Atualiza o Tempo de Varrer Arquivo
    totalVA = totalVA + tempoDif;

    if (i == 0) {
      minVA = tempoDif;
    } else if (tempoDif < minVA) {
      minVA = tempoDif;
    }

    if (tempoDif > maxVA) {
      maxVA = tempoDif;
    }

/*  
    if (pesquisa == mat) {
      printf("Tempo VA: %f ms\n", tempoDif);
    }
*/
      
    // Pesquisa pela B Tree
    rewind(arq);
    tempo = (double)clock() / (CLOCKS_PER_SEC / 1000);

    linhaArq = buscarChave(pesquisa, &pos, arv->raiz);
    fseek(arq, (linhaArq * tamReg) - 1, SEEK_SET);
    fscanf(arq, "%d %s %d %s", &mat, nome, &idade, curso);

    tempoDif = (double)clock() / (CLOCKS_PER_SEC / 1000) - tempo;

    // Atualiza o Tempo pela B tree
    totalBT = totalBT + tempoDif;

    if (i == 0) {
      minBT = tempoDif;
    } else if (tempoDif < minBT) {
      minBT = tempoDif;
    }

    if (tempoDif > maxBT) {
      maxBT = tempoDif;
    }
      
/*  
    if (pesquisa == mat) {
      printf("Tempo BT: %f ms\n", tempoDif);
    }
*/
  }

  printf("\nPesquisa Varrendo o Arquivo\n");
  printf("Tempo total: %f ms\n", totalVA);
  printf("Tempo Médio: %f ms\n", totalVA / quant);
  printf("Tempo Mínimo: %f ms\n", minVA);
  printf("Tempo Máximo: %f ms\n", maxVA);

  printf("\nPesquisa pela B Tree\n");
  printf("Tempo total: %f ms\n", totalBT);
  printf("Tempo Médio: %f ms\n", totalBT / quant);
  printf("Tempo Mínimo: %f ms\n", minBT);
  printf("Tempo Máximo: %f ms\n", maxBT);
  fclose(arq);
}