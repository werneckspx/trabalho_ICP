#include "fila.h"

void Vazia(Fila *f) {
    f->first = NULL;
    f->last = NULL;
}

bool isVazia(Fila *f) {
    return f->first == NULL;
}

void Enfileira_prioridade(Fila *f, Cliente c) {
    Block *novo = (Block *)malloc(sizeof(Block));
    novo->data = c;
    novo->prox = NULL;

    if (isVazia(f) || c.prioridade < f->first->data.prioridade) {
        // Insere no início
        novo->prox = f->first;
        f->first = novo;
        if (f->last == NULL) f->last = novo;
        return;
    }

    // Insere no meio ou no fim
    Block *atual = f->first;
    while (atual->prox != NULL && atual->prox->data.prioridade <= c.prioridade) {
        atual = atual->prox;
    }

    novo->prox = atual->prox;
    atual->prox = novo;

    if (novo->prox == NULL) {
        f->last = novo; // Atualiza o último elemento, se necessário
    }
}

void Desenfileira(Fila *f, Cliente *c) {
    if (isVazia(f)) {
        printf("FILA VAZIA!\n");
        return;
    }

    Block *aux = f->first;
    *c = aux->data;
    f->first = aux->prox;

    if (f->first == NULL) {
        f->last = NULL;
    }

    free(aux);
}

void Imprime(Fila *f) {
    if (isVazia(f)) {
        printf("FILA VAZIA!\n");
        return;
    }

    Block *aux = f->first;
    while (aux != NULL) {
        printf("Nome: %s, CPF: %s, Prioridade: %d, Itens: %d\n",
               aux->data.nome, aux->data.cpf, aux->data.prioridade, aux->data.itens);
        aux = aux->prox;
    }
}

int Tamanho(Fila *f) {
    int count = 0;
    Block *aux = f->first;
    while (aux != NULL) {
        count++;
        aux = aux->prox;
    }
    return count;
}
