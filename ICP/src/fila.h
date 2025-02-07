#ifndef FILA_H 
#define FILA_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <ctype.h>

typedef struct Cliente Cliente;
typedef struct Block Block;
typedef struct Fila Fila;

struct Cliente{
    char nome[101];
    char cpf[12];
    enum prioridades{
        baixa= 3,
        media= 2,
        alta= 1
    }prioridade;
    int itens;
};

struct Block{
	Cliente data;
	Block *prox;
};

struct Fila{
	Block *first;
	Block *last;
};

bool isVazia(Fila *f);
void Vazia(Fila *f);
void Enfileira_prioridade(Fila *f, Cliente c);
void Desenfileira(Fila *f, Cliente *c);
void Imprime(Fila *f);
int Tamanho(Fila *f);

#endif