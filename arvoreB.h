// --- Tipos de Dados --- //

typedef struct chave {
  int mat;    // Mátricula (chave primária única)
  int linha;  // Linha no arquivo original (Será usada para localizar no arquivo)
} chave;

typedef struct pagina {
  int numChaves;
  chave *chaves;
  struct pagina **filhos;
} pagina;

typedef struct arvoreB {
  pagina *raiz;
  int ordem;
} arvoreB;

// --- Funções Árvore B --- //

// Função para criar uma nova página
pagina *criarPagina(int ordem, chave chv, pagina *filho, arvoreB *arv);

// Função para inicializar uma ArvoreB
arvoreB *criarArvoreB(int ordem);

// Função para recuperar a raiz da arvore
pagina *getRaiz(arvoreB *arv);


// Função que adiciona a chave na página
void addChavePag(chave chv, int pos, pagina *pag, pagina *filho);

// Função que efetua o split
void split(chave chv, chave *pval, int pos, pagina *pag, pagina *filho, pagina **novaPag, int ordem);

// Função auxiliar de inserirChave()
int inserirAux(chave chv, chave *pval, pagina *pag, pagina **filho, int ordem);

// Função para inserir uma chave na árvore B
void inserirChave(chave chv, arvoreB *arv);


// Função que acha e copia o sucessor
void copiarSucessor(pagina *pag, int pos);

// Função que Remove o valor da página
void removerValor(pagina *pag, int pos);

// Função que realiza a rotação à direita
void rotacaoDireita(pagina *pag, int pos);

// Função que realiza a rotação à esquerda
void rotacaoEsquerda(pagina *pag, int pos);

// Função que faz merge nas páginas
void mergePag(pagina *pag, int pos);

// Função que ajusta a página após a remoção
void ajustarPag(pagina *pag, int pos, int ordem);

// Função auxiliar de removerChave()
int removerAux(int chave, pagina *pag, int ordem);

// Função para remover uma chave da árvore B
void removerChave(int chave, arvoreB *arv);

// Função para buscar uma chave na árvore B
// Retorna a Linha do Registro no Arquivo
int buscarChave(int chave, int *pos, struct pagina *pag);

// Função para criar uma nova chave
chave criarChave(int mat, int linha);