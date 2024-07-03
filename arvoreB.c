#include "arvoreB.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// --- Funções Árvore B --- //

// Função para criar uma nova página
pagina *criarPagina(int ordem, chave chv, pagina *filho, arvoreB *arv) {
  pagina *novaPagina;

  novaPagina = (pagina *)malloc(sizeof(pagina));

  novaPagina->chaves = (chave *)malloc(sizeof(chave) * ordem);
  novaPagina->filhos = (pagina **)malloc(sizeof(pagina *) * ordem);

  novaPagina->chaves[1] = chv;
  novaPagina->numChaves = 1;
  novaPagina->filhos[0] = arv->raiz;
  novaPagina->filhos[1] = filho;

  return novaPagina;
}

arvoreB *criarArvoreB(int ordem) {
  arvoreB *arv;

  arv = (arvoreB *)malloc(sizeof(arv));
  arv->raiz = NULL;
  arv->ordem = ordem;

  return arv;
}

pagina *getRaiz(arvoreB *arv) {return arv->raiz;}

// Função que adiciona a chave na página
void addChavePag(chave chv, int pos, pagina *pag, pagina *filho) {
  int j;
  j = pag->numChaves;
  while(j > pos) {
    pag->chaves[j+1] = pag->chaves[j];
    pag->filhos[j+1] = pag->filhos[j];
    j--;
  }
  pag->chaves[j+1] = chv;
  pag->filhos[j+1] = filho;
  pag->numChaves++;
  return;
}

// Função que efetua o split
void split(chave chv, chave *pval, int pos, pagina *pag, pagina *filho, pagina **novaPag, int ordem) {
  int mediano, j, min, max;
  max = ordem-1;        // Máximo de chaves em uma página
  min = floor(max/2); // Mínimo de chaves em uma página
  if(pos>min)
    mediano = min+1;
  else
    mediano = min;

  *novaPag = (pagina*) malloc(sizeof(pagina));
  (*novaPag)->chaves = (chave *)malloc(sizeof(chave) * ordem);
  (*novaPag)->filhos = (pagina **)malloc(sizeof(pagina *) * ordem);

  j = mediano + 1;
  while(j <= max) { // Atribui as chaves e filhos para nova página
    (*novaPag)->chaves[j-mediano] = pag->chaves[j];
    (*novaPag)->filhos[j-mediano] = pag->filhos[j];
    j++;
  }

  // Ajusta tamanhos
  pag->numChaves = mediano;
  (*novaPag)->numChaves = max - mediano;

  // Dependendo da posição correta da chave aloca para a página antiga ou para a nova
  if(pos <= min) {
    addChavePag(chv, pos, pag, filho);
  }
  else {
    addChavePag(chv, pos - mediano, *novaPag, filho);
  }

  *pval = pag->chaves[pag->numChaves]; 
  (*novaPag)->filhos[0] = pag->filhos[pag->numChaves];
  pag->numChaves--;
  return;
} 

// Função auxiliar de inserirChave()
int inserirAux(chave chv, chave *pval, pagina *pag, pagina **filho, int ordem) {
  int pos, max;
  max = ordem-1;  // Máximo de chaves em uma página
  if(!pag) {  // Se a raiz for nula
    *pval = chv;
    *filho = NULL;
    return 1;
  }

  if(chv.mat < pag->chaves[1].mat) { 
    pos = 0;  // Se for menor, ele deve estar na primeira posição (0)
  }
  else {
    for(pos = pag->numChaves; (chv.mat < pag->chaves[pos].mat && pos > 1); pos--); // Atribui valor apropriado a pos
    if(chv.mat == pag->chaves[pos].mat) {
      return 0;  // Código identificador deve ser único
    }
  }
  if(inserirAux(chv, pval, pag->filhos[pos], filho, ordem)) { // Árvore será percorrida recursivamente
    if(pag->numChaves < max)  // Se houver espaço, adiciona chave na página
      addChavePag(*pval, pos, pag, *filho);
    else {                    // Se não, faz split
      split(*pval, pval, pos, pag, *filho, filho, ordem);
      return 1;
    }
  }
  return 0;
}

// Função para inserir uma chave na árvore B
void inserirChave(chave chv, arvoreB *arv) {
  int flag;  // Váriavel que indicará se será necessário criar nova raiz
  chave i;
  pagina *filho;

  flag = inserirAux(chv, &i, arv->raiz, &filho, arv->ordem);
  if(flag)  // Se flag == 1, uma nova raiz será criada
    arv->raiz = criarPagina(arv->ordem, i, filho, arv);
  return;
}

// Função que acha e copia o sucessor
void copiarSucessor(pagina *pag, int pos) {
  pagina *aux;
  aux = pag->filhos[pos];  // Sub-árvore direita à chave

  while(aux->filhos[0] != NULL)
    aux = aux->filhos[0];  // Sub-árvore mais a esquerda
  pag->chaves[pos] = aux->chaves[1];
}

// Função que Remove o valor da página
void removerValor(pagina *pag, int pos) {
  int i;
  i = pos + 1;
  while(i <= pag->numChaves) {  // Percorre página ajustando e removendo valor especificado
    pag->chaves[i - 1] = pag->chaves[i];
    pag->filhos[i - 1] = pag->filhos[i];
    i++;
  }
  pag->numChaves--;
}

// Função que realiza a rotação à direita
void rotacaoDireita(pagina *pag, int pos) {
  pagina *x;  // Página que será rotacionada
  int j; 
  j = x->numChaves;
  x = pag->filhos[pos];  // Página à direita

  while(j > 0) {  // Move todas as chaves e filhos de x para a direita
    x->chaves[j + 1] = x->chaves[j];
    x->filhos[j + 1] = x->filhos[j];
  }

  // Insere a chave e o filho da página em x
  x->chaves[1] = pag->chaves[pos];
  x->filhos[1] = x->filhos[0];
  x->numChaves++;

  // Atualiza página com chave e filho apropriados
  x = pag->filhos[pos - 1];
  pag->chaves[pos] = x->chaves[x->numChaves];
  pag->filhos[pos] = x->filhos[x->numChaves];
  x->numChaves--;
  return;
}

// Função que realiza a rotação à esquerda
void rotacaoEsquerda(pagina *pag, int pos) {
  pagina *x;  // Página que será rotacionada
  int j;
  j = 1;
  x = pag->filhos[pos - 1];   // Página à esquerda

  // Move a chave e o filho apropriados de pag para x
  x->numChaves++;
  x->chaves[x->numChaves] = pag->chaves[pos];
  x->filhos[x->numChaves] = pag->filhos[pos]->filhos[0];

  // Atualiza pag com a chave e filho de x
  x = pag->filhos[pos];
  pag->chaves[pos] = x->chaves[1];
  x->filhos[0] = x->filhos[1];
  x->numChaves--;

  while (j <= x->numChaves) {  // Move chaves de x à esquerda
    x->chaves[j] = x->chaves[j + 1];
    x->filhos[j] = x->filhos[j + 1];
    j++;
  }
  return;
}

// Função que faz merge nas páginas
void mergePag(pagina *pag, int pos) {
  int j;
  pagina *x1, *x2;
  j = 1;
  x1 = pag->filhos[pos];    // Página filha a direita
  x2 = pag->filhos[pos - 1];// Página filha a esquerda

  // Adiciona chave em x2
  x2->numChaves++;
  x2->chaves[x2->numChaves] = pag->chaves[pos];
  x2->filhos[x2->numChaves] = pag->filhos[0];

  while(j <= x1->numChaves) {  // Move chaves e filhos de x1 para x2
    x2->numChaves++;
    x2->chaves[x2->numChaves] = x1->chaves[j];
    x2->filhos[x2->numChaves] = x1->filhos[j];
    j++;
  }

  j = pos;
  while(j < pag->numChaves) {  // Move chaves de pag para a esquerda
    pag->chaves[j] = pag->chaves[j + 1];
    pag->filhos[j] = pag->filhos[j + 1];
    j++;
  }
  pag->numChaves--;
  free(x1);  // Libera memória da página antiga
}

// Função que ajusta a página após a remoção
void ajustarPag(pagina *pag, int pos, int ordem) {
  int min;
  min = floor((ordem-1)/2);
  if(!pos) {  // pos == 0 se é primeira posição
    if(pag->filhos[1]->numChaves > min)  // Se irmão tiver como emprestar
      rotacaoEsquerda(pag, 1);
    else  // Irmão não consegue emprestar, então faz merge
      mergePag(pag, 1);
  } 
  else {  // Não está na primeira posição
    if(pag->numChaves != pos) {  // Se não tiver na última
      if(pag->filhos[pos - 1]->numChaves > min) // Se o irmão esquerdo tiver como emprestar
        rotacaoDireita(pag, pos);
      else {  // Irmão esquerdo não consegue emprestar
        if(pag->filhos[pos + 1]->numChaves > min) // Se o irmão direito tiver como emprestar
          rotacaoEsquerda(pag, pos + 1);
        else  // Nenhum irmão pode emprestar, então faz merge
          mergePag(pag, pos);
      }
    } 
    else {  // Se tiver na última posição
      if(pag->filhos[pos - 1]->numChaves > min)  // Se irmão tiver como emprestar
        rotacaoDireita(pag, pos);
      else  // Não consegue emprestar, então faz merge
        mergePag(pag, pos);
    }
  }
}

// Função auxiliar de removerChave()
int removerAux(int chave, pagina *pag, int ordem) {
  int min;
  int pos, flag;
  min = floor((ordem-1)/2);

  flag = 0;  // váriavel flag será usada para saber se operação foi bem sucedida

  if(pag) {
    if(chave < pag->chaves[1].mat) {  // Se for menor que primeiro elemento, não está nessa página
      pos = 0;
      flag = 0;
    } 
    else {  // É preciso percorrer a página
      for(pos = pag->numChaves; (chave < pag->chaves[pos].mat && pos > 1); pos--);  // Atribui valor apropriado a pos
      if(chave == pag->chaves[pos].mat) {  // Chave foi encontrada nessa página
        flag = 1;
      } 
      else {  // Não foi encontrada na página
        flag = 0;
      }
    }
    if(flag) {  // Se for encontrada, será removida
      if(pag->filhos[pos - 1]) {  // Tem filho a esquerda
        copiarSucessor(pag, pos);  //  Copia o sucessor para a posição atual
        flag = removerAux(pag->chaves[pos].mat, pag->filhos[pos], ordem);  // Recursivamente remove a chave copiada
        if(flag == 0) {
          printf("Chave não presente na Árvore B\n");
        }
      } 
      else {
        removerValor(pag, pos);  // Remove chave da página atual
      }
    } 
    else {  // Se não foi encontrada na página, continua busca
      flag = removerAux(chave, pag->filhos[pos], ordem);
    }

    // Após a remoção, faz correções na página se necessário
    if(pag->filhos[pos]) {
      if(pag->filhos[pos]->numChaves < min)
        ajustarPag(pag, pos, ordem);
    }
  }
  return flag;
}

// Função para remover uma chave da árvore B
void removerChave(int chave, arvoreB *arv) {
  pagina *tmp;
  pagina *pag = arv->raiz;
  if(!removerAux(chave, pag, arv->ordem)) {  // Se remoção não for sucedida
    printf("Chave não presente\n");
    return;
  } 
  else {  // Remoção foi bem sucedida, então será checado se a pagina atual ficou sem chaves
    if(pag->numChaves == 0) {
      tmp = pag;
      pag = pag->filhos[0];  // Pag passa a ser o filho da raiz vazia
      free(tmp);
    }
  }
  arv->raiz = pag;  // Atualiza raiz
  return;
}

// Função para buscar uma chave na árvore B
// Retorna a Linha do Registro no Arquivo
int buscarChave(int mat, int *pos, struct pagina *pag) {
  if(!pag)
    return -1; // Erro
  if(mat < pag->chaves[1].mat) {
    *pos = 0;
  } 
  else {
    for (*pos = pag->numChaves; (mat < pag->chaves[*pos].mat && *pos > 1); (*pos)--);  // Atribui valor apropriado a pos
    if(mat == pag->chaves[*pos].mat)
      return pag->chaves[*pos].linha; // Retorna a linha da chave que foi buscada
  }
  return buscarChave(mat, pos, pag->filhos[*pos]); // Retorna o valor da chamada recursiva
}


chave criarChave(int mat, int linha) {
  chave chv;
  chv.mat = mat;
  chv.linha = linha;
  return chv;
}