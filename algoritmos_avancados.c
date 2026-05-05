#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash

#define TAM_HASH 10

// ================= ESTRUTURA DA SALA =================

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ================= BST DE PISTAS =================

typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ================= TABELA HASH =================

// Cada nó da hash relaciona pista -> suspeito
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

#define TAM_PILHA 50

Sala* pilha[TAM_PILHA];
int topo = -1;

void push(Sala* s) {
    if (topo < TAM_PILHA - 1)
        pilha[++topo] = s;
}

Sala* pop() {
    if (topo >= 0)
        return pilha[topo--];
    return NULL;
}

// ================= FUNÇÕES =================

// criarSala()
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    strcpy(nova->nome, nome);
    if (pista) strcpy(nova->pista, pista);
    else strcpy(nova->pista, "");

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// ================= BST =================

PistaNode* inserirPista(PistaNode *raiz, const char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->direita = inserirPista(raiz->direita, texto);

    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (raiz) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// ================= HASH =================

// função de hash simples
int funcaoHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// inserirHash()
// relaciona pista -> suspeito
void inserirHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// buscarSuspeito()
// retorna o suspeito associado à pista
char* buscarSuspeito(const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabelaHash[indice];

    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// conta quantas pistas apontam para um suspeito
int contarPistasSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL) return 0;

    int count = 0;

    char *s = buscarSuspeito(raiz->texto);
    if (s && strcmp(s, suspeito) == 0)
        count++;

    count += contarPistasSuspeito(raiz->esquerda, suspeito);
    count += contarPistasSuspeito(raiz->direita, suspeito);

    return count;
}

// ================= EXPLORAÇÃO =================

void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("🔍 Pista: %s\n", atual->pista);

            char *suspeito = buscarSuspeito(atual->pista);
            if (suspeito)
                printf("👤 Suspeito associado: %s\n", suspeito);

            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("\nOpcoes:\n");

        if (atual->esquerda)
            printf(" (e) Esquerda -> %s\n", atual->esquerda->nome);

        if (atual->direita)
            printf(" (d) Direita  -> %s\n", atual->direita->nome);

        if (topo >= 0)
            printf(" (v) Voltar\n");

        printf(" (s) Sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda) {
            push(atual);
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita) {
            push(atual);
            atual = atual->direita;
        }
        else if (opcao == 'v') {
            Sala *anterior = pop();
            if (anterior)
                atual = anterior;
            else
                printf("Nao ha para onde voltar!\n");
        }
        else if (opcao == 's') {
            break;
        }
        else {
            printf("Opcao invalida\n");
        }
    }
}

// ================= MAIN =================

int main() {

    // inicializa hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // 🔧 Mapa da mansão

    Sala *hall = criarSala("Hall", "");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala *cozinha = criarSala("Cozinha", "Faca suja");
    Sala *sala = criarSala("Sala", "Pegadas");
    Sala *jardim = criarSala("Jardim", "Terra mexida");
    Sala *sotao = criarSala("Sotao", "Caixa trancada");
    Sala *porao = criarSala("Porao", "Barulho estranho");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;

    biblioteca->esquerda = sala;
    biblioteca->direita = jardim;

    cozinha->esquerda = sotao;
    cozinha->direita = porao;

    // 🔗 associa pistas a suspeitos
    inserirHash("Livro rasgado", "Mordomo");
    inserirHash("Faca suja", "Cozinheiro");
    inserirHash("Pegadas", "Jardineiro");
    inserirHash("Terra mexida", "Jardineiro");
    inserirHash("Caixa trancada", "Mordomo");
    inserirHash("Barulho estranho", "Desconhecido");

    PistaNode *arvorePistas = NULL;

    printf("=== Exploracao ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n=== Pistas coletadas ===\n");
    exibirPistas(arvorePistas);

    // 🎯 acusação
    char suspeito[50];
    printf("\nQuem e o culpado? ");
    scanf(" %[^\n]", suspeito);

    int total = contarPistasSuspeito(arvorePistas, suspeito);

    if (total >= 2)
        printf("✅ Acusacao consistente! %s parece culpado (%d pistas).\n", suspeito, total);
    else
        printf("❌ Poucas evidencias contra %s (%d pistas).\n", suspeito, total);

    return 0;
}