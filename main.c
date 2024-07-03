#include "indice.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  int ordem;
  int chave;
  int escolha = 0;
  int linhaArq;
  int pos, nMat;
  arvoreB *arv;
  int tamReg; // No nosso caso, o tamanho do registro é 25
  char nomeArquivo[30];

  while (escolha != 4) {
    printf("\n--- Menu - Indice em Árvore B ---\n");
    printf("1. Criar Índice\n");
    printf("2. Procurar Elementos\n");
    printf("3. Remover Registro\n");
    printf("4. Sair\n");
    printf("5. Testes Velocidade\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);

    switch (escolha) {
      case 1:
        printf("\nDigite a ordem da Arvore do Índice: ");
        scanf("%d", &ordem);
        printf("Digite o nome do arquivo: ");
        scanf("%s", nomeArquivo);
        printf("Digite o tamanho do Registro: ");
        scanf("%d", &tamReg);

        arv = criarArvoreB(ordem);

        criarIndice(nomeArquivo, arv);
        break;

      case 2:
        printf("\nDigite a chave a ser procurada: ");
        scanf("%d", &chave);
        linhaArq = buscarChave(chave, &pos, getRaiz(arv));
        if (linhaArq != -1) {
          printf("Chave encontrada na linha %d\n", linhaArq);
          imprimirRegistro(linhaArq, nomeArquivo, tamReg);
        } else {
          printf("Chave não encontrada\n");
        }

        break;

      case 3:
        printf("\nDigite a chave a ser removida: ");
        scanf("%d", &chave);
        removerChave(chave, arv);
      break;

      case 5:
        printf("\nDigite o número de matriculas para pesquisa: ");
        scanf("%d", &nMat);

        comparacao(nomeArquivo, nMat, tamReg, arv);
      break;
    }
  }

  return 0;
}