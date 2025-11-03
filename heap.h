#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HEAP_SIZE 20
#define MAX_VARS 10

// estratégias de alocação
typedef enum {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    NEXT_FIT
} Strategy;

// lista de áreas livres
typedef struct freeBlock {
    int start;   //Endereço inicial da área livre
    int size;    //Quantidade de blocos livres
    struct freeBlock* next;
} freeBlock;

// armazenar variáveis alocadas
typedef struct {
    char name[20];
    int start;
    int size;
} Alocadas;

// estrutura principal do heap
typedef struct {
    bool memory[HEAP_SIZE];   //false = livre, true = ocupado
    freeBlock* freeList;       //lista de áreas livres
    Alocadas vars[MAX_VARS];  //Variáveis alocadas
    int varCount;
    Strategy strategy;
    int nextFitPos;           //guarda a posição onde a última alocação foi feita
} Heap;

// protótipos
void initHeap(Heap* heap);
void setStrategy(Heap* heap, Strategy s);
void newAlloc(Heap* heap, const char* name, int size);
void delAlloc(Heap* heap, const char* name);
void assign(Heap* heap, const char* dest, const char* fonte);
void exibe(Heap* heap);
void addfreeBlock(Heap* heap, int start, int size);
void mergefreeBlocks(Heap* heap);
int findAlocadas(Heap* heap, const char* name);
void clearBuffer();
void showMenu();

#endif