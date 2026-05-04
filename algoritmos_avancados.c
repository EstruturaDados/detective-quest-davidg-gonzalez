#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash

// ================= ESTRUTURA DA SALA =================

// Struct Sala
// Representa cada cômodo da mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; // NOVO: pista associada ao cômodo
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ================= ESTRUTURA DA BST DE PISTAS =================

// Struct PistaNode
// Representa um nó da árvore binária de busca de pistas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ================= FUNÇÕES =================

// criarSala()
// Cria dinamicamente uma nova sala com nome e pista
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (nova == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return NULL;
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// inserirPista()
// Insere uma pista na árvore BST (ordenada alfabeticamente)
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

// exibirPistas()
// Exibe as pistas em ordem alfabética (percorrimento em ordem)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// explorarSalasComPistas()
// Permite navegação e coleta automática de pistas
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Se houver pista
        if (strlen(atual->pista) > 0) {
            printf("🔍 Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("Escolha para onde ir:\n");

        if (atual->esquerda != NULL)
            printf(" (e) Esquerda -> %s\n", atual->esquerda->nome);

        if (atual->direita != NULL)
            printf(" (d) Direita  -> %s\n", atual->direita->nome);

        printf(" (s) Sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (opcao == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opcao invalida!\n");
        }
    }
}

// ================= MAIN =================

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária

    // 🔧 Montagem da árvore (agora com pistas)

    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado na estante");
    Sala *cozinha = criarSala("Cozinha", "Faca com manchas estranhas");
    Sala *salaEstar = criarSala("Sala de Estar", "Pegadas no tapete");
    Sala *jardim = criarSala("Jardim", "Terra revirada recentemente");
    Sala *sotao = criarSala("Sotao", "Caixa trancada com cadeado");
    Sala *porao = criarSala("Porao", "Barulho estranho vindo das paredes");

    // Conectando as salas
    hall->esquerda = biblioteca;
    hall->direita = cozinha;

    biblioteca->esquerda = salaEstar;
    biblioteca->direita = jardim;

    cozinha->esquerda = sotao;
    cozinha->direita = porao;

    // BST de pistas
    PistaNode *arvorePistas = NULL;

    // Início da exploração
    printf("=== Exploracao da Mansao ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibir pistas coletadas
    printf("\n=== Pistas Coletadas (ordem alfabetica) ===\n");
    exibirPistas(arvorePistas);

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}