#include <stdio.h>     
#include <stdlib.h>     
#include <string.h>     
#include <stdbool.h>    
#include "heap.h"       

#define HEAP_SIZE 20
#define MAX_VARS 10

// funções:

void clearBuffer() {
    int c;  
    while ((c = getchar()) != '\n' && c != EOF);
}

// inicializa o heap
void initHeap(Heap* heap) {
    // loop que percorre todos os blocos do heap
    for (int i = 0; i < HEAP_SIZE; i++) {
        heap->memory[i] = false;  // marca cada bloco como livre (false)
    }
    
    // aloca memória dinamicamente para o primeiro nó da lista de áreas livres
    heap->freeList = (freeBlock*)malloc(sizeof(freeBlock));
    heap->freeList->start = 0;        // a área livre começa no endereço 0
    heap->freeList->size = HEAP_SIZE; // a área livre tem o tamanho total do heap
    heap->freeList->next = NULL;      // não há próximo nó (lista tem apenas um elemento)
    
    heap->varCount = 0;           // inicializa contador de variáveis com zero
    heap->strategy = FIRST_FIT;   // define a estratégia padrão como first fit
    heap->nextFitPos = 0;         // inicializa a posição do next fit no início
}

// define a estratégia de alocação
void setStrategy(Heap* heap, Strategy s) {
    heap->strategy = s;  // atualiza a estratégia do heap com o valor recebido
    // se a estratégia escolhida for next fit
    if (s == NEXT_FIT) {
        heap->nextFitPos = 0;  // reinicia a posição de busca para o início
    }
    
    // vetor com os nomes das estratégias
    const char* names[] = {"First Fit", "Best Fit", "Worst Fit", "Next Fit"};
    printf("Estratégia alterada para: %s\n", names[s]);
}

// encontra o índice da variável no vetor de alocadas
int findAlocadas(Heap* heap, const char* name) {
    for (int i = 0; i < heap->varCount; i++) {
        // compara o nome da variável atual com o nome buscado
        if (strcmp(heap->vars[i].name, name) == 0) {
            return i;  // se encontrou, retorna o índice
        }
    }
    return -1;  // se não encontrou, retorna -1 indicando que não existe
}

// adiciona nó à lista de áreas livres (mantendo ordenação por endereço)
void addfreeBlock(Heap* heap, int start, int size) {
    // aloca memória dinamicamente para um novo nó
    freeBlock* newNode = (freeBlock*)malloc(sizeof(freeBlock));
    newNode->start = start;  // define o endereço inicial da área livre
    newNode->size = size;    // define o tamanho da área livre
    newNode->next = NULL;    // inicializa o próximo como NULL
    
    // verifica se a lista está vazia OU se o novo nó deve ir no início
    if (heap->freeList == NULL || heap->freeList->start > start) {
        newNode->next = heap->freeList;  // novo nó aponta para o antigo primeiro
        heap->freeList = newNode;        // lista agora aponta para o novo nó
    } else {  // senão, o nó vai no meio ou no final
        freeBlock* curr = heap->freeList;  // cria ponteiro auxiliar para percorrer
        // percorre até encontrar a posição correta de inserção
        while (curr->next != NULL && curr->next->start < start) {
            curr = curr->next;  
        }
        newNode->next = curr->next;  
        curr->next = newNode;        
    }
    
    mergefreeBlocks(heap);  // chama função pra fazer o merge de áreas livres adjacentes
}

// merge de áreas livres
void mergefreeBlocks(Heap* heap) {
    freeBlock* curr = heap->freeList;
    
    while (curr != NULL && curr->next != NULL) {
        // verifica se o fim do nó atual coincide com o início do próximo
        if (curr->start + curr->size == curr->next->start) {
            freeBlock* temp = curr->next;  // guarda o próximo nó para liberar depois
            curr->size += temp->size;      // soma o tamanho do próximo ao atual
            curr->next = temp->next;       // nó atual pula o próximo e aponta pro seguinte
            free(temp);                    // libera a memória do nó que foi mesclado
        } else {  // se não são adjacentes
            curr = curr->next;  // avança para o próximo nó
        }
    }
}

// aloca memória usando a estratégia definida
void newAlloc(Heap* heap, const char* name, int size) {
    // verifica se a variável já existe
    if (findAlocadas(heap, name) != -1) {
        printf("Erro: Variável '%s' já existe\n", name);  // exibe erro
        return;  // sai da função sem alocar
    }
    
    // ponteiros para controlar a busca na lista de áreas livres
    freeBlock* selected = NULL;  // nó selecionado para alocação
    freeBlock* prev = NULL;      // nó anterior ao selecionado
    freeBlock* currPrev = NULL;  // nó anterior ao atual durante a busca
    freeBlock* curr = heap->freeList;  // nó atual durante a busca
    
    // escolhe a estratégia de busca
    switch (heap->strategy) {
        case FIRST_FIT:  // primeira área que couber
            // percorre a lista de áreas livres
            while (curr != NULL) {
                // se o tamanho da área é suficiente
                if (curr->size >= size) {
                    selected = curr;     // seleciona este nó
                    prev = currPrev;     // guarda o nó anterior
                    break;               // para a busca (primeira que encontrou)
                }
                currPrev = curr;         
                curr = curr->next;       
            }
            break;
            
        case BEST_FIT:  // menor área que couber
            // percorre toda a lista
            while (curr != NULL) {
                // se o tamanho da área é suficiente
                if (curr->size >= size) {
                    // se ainda não selecionou nada OU encontrou área menor
                    if (selected == NULL || curr->size < selected->size) {
                        selected = curr;  // atualiza o selecionado
                        prev = currPrev;  // atualiza o anterior
                    }
                }
                currPrev = curr;      
                curr = curr->next;    
            }
            break;
            
        case WORST_FIT:  // maior área disponível
            // percorre toda a lista
            while (curr != NULL) {
                // se o tamanho da área é suficiente
                if (curr->size >= size) {
                    // se ainda não selecionou nada OU encontrou área maior
                    if (selected == NULL || curr->size > selected->size) {
                        selected = curr;  // atualiza o selecionado
                        prev = currPrev;  // atualiza o anterior
                    }
                }
                currPrev = curr;      
                curr = curr->next;   
            }
            break;
            
        case NEXT_FIT:  // continua de onde parou
            // começa da última posição
            curr = heap->freeList;  // reinicia do começo da lista
            currPrev = NULL;        // não há anterior ainda
            // percorre procurando área após nextFitPos
            while (curr != NULL) {
                // se a área começa após ou em nextFitPos E tem tamanho suficiente
                if (curr->start >= heap->nextFitPos && curr->size >= size) {
                    selected = curr;  
                    prev = currPrev;  
                    break;            
                }
                currPrev = curr;      
                curr = curr->next;    
            }
            // se não encontrou nada após nextFitPos, busca do início
            if (selected == NULL) {
                curr = heap->freeList;  // volta para o início da lista
                currPrev = NULL;        // não há anterior
                // percorre até nextFitPos
                while (curr != NULL && curr->start < heap->nextFitPos) {
                    // se tem tamanho suficiente
                    if (curr->size >= size) {
                        selected = curr;  
                        prev = currPrev;  
                        break;            
                    }
                    currPrev = curr;      
                    curr = curr->next;    
                }
            }
            break;
    }
    
    // se não encontrou nenhuma área livre suficiente
    if (selected == NULL) {
        printf("Erro: Memória insuficiente para alocar '%s' (%d blocos)\n", name, size);
        return;  
    }
    
    // aloca a memória no heap
    int allocStart = selected->start;  // guarda o endereço inicial da alocação
    // marca os blocos como ocupados
    for (int i = allocStart; i < allocStart + size; i++) {
        heap->memory[i] = true;  // true = bloco ocupado
    }
    
    // adiciona a variável ao vetor de alocadas
    strcpy(heap->vars[heap->varCount].name, name);  // copia o nome da variável
    heap->vars[heap->varCount].start = allocStart;  // guarda o endereço inicial
    heap->vars[heap->varCount].size = size;         // guarda o tamanho
    heap->varCount++;  // incrementa o contador de variáveis
    
    // atualiza a lista de áreas livres
    if (selected->size == size) {
        if (prev == NULL) {  
            heap->freeList = selected->next;  
        } else {  
            prev->next = selected->next;  
        }
        free(selected);  
    } else {  
        selected->start += size;  
        selected->size -= size;  
    }
    
    // se a estratégia é next fit
    if (heap->strategy == NEXT_FIT) {
        heap->nextFitPos = allocStart + size;  // atualiza posição para próxima busca
    }
    
    printf("Alocado '%s': %d blocos a partir do endereço %d\n", name, size, allocStart);
}

// desaloca memória
void delAlloc(Heap* heap, const char* name) {
    int idx = findAlocadas(heap, name);  // busca o índice da variável

    if (idx == -1) {
        printf("Erro: Variável '%s' não existe\n", name);
        return;
    }
    
    Alocadas* var = &heap->vars[idx];  // ponteiro para a variável encontrada
    
    // libera a memória no heap
    // percorre os blocos ocupados pela variável
    for (int i = var->start; i < var->start + var->size; i++) {
        heap->memory[i] = false;  // marca como livre (false)
    }
    
    printf("Desalocado '%s': %d blocos a partir do endereço %d\n", 
           name, var->size, var->start);
    
    // adiciona a área liberada à lista de áreas livres
    addfreeBlock(heap, var->start, var->size);
    
    // remove a variável do vetor de alocadas
    // desloca todas as variáveis após a removida uma posição para trás
    for (int i = idx; i < heap->varCount - 1; i++) {
        heap->vars[i] = heap->vars[i + 1];  // copia a próxima para a atual
    }
    heap->varCount--;  // decrementa o contador de variáveis
}

// atribui "referência" de uma variável a outra
void assign(Heap* heap, const char* dest, const char* fonte) {
    int fonteIdx = findAlocadas(heap, fonte);  // busca índice da variável fonte
    // se a fonte não existe
    if (fonteIdx == -1) {
        printf("Erro: Variável fonte '%s' não existe\n", fonte); 
        return;
    }
    
    int destIdx = findAlocadas(heap, dest);  // busca índice da variável destino
    // se o destino não existe
    if (destIdx == -1) {
        printf("Erro: Variável destino '%s' não existe\n", dest); 
        return; 
    }
    
    // copia os dados da variável fonte para a variável destino
    heap->vars[destIdx].start = heap->vars[fonteIdx].start;  // copia endereço inicial
    heap->vars[destIdx].size = heap->vars[fonteIdx].size;    // copia tamanho
    
    printf("'%s' agora aponta para a mesma região de '%s'\n", dest, fonte);
}

// exibe o estado atual do heap
void exibe(Heap* heap) {
    printf("\n========== ESTADO DO HEAP ==========\n"); 
    
    printf("Memória [0-%d]:\n", HEAP_SIZE - 1);  // mostra range de endereços
    // percorre todos os blocos
    for (int i = 0; i < HEAP_SIZE; i++) {
        // a cada 10 blocos, quebra linha e mostra o endereço
        if (i % 10 == 0) printf("\n%3d: ", i);
        // exibe # se ocupado, . se livre
        printf("%c ", heap->memory[i] ? '#' : '.');
    }
    printf("\n\n");  
    
    // exibe as variáveis alocadas
    printf("Variáveis alocadas:\n");
    // se não há variáveis
    if (heap->varCount == 0) {
        printf("  (nenhuma)\n");  // informa que está vazio
    } else {  // se há variáveis
        // percorre o vetor de variáveis
        for (int i = 0; i < heap->varCount; i++) {
            // exibe nome, range de endereços e quantidade de blocos
            printf("  %s: [%d-%d] (%d blocos)\n", 
                   heap->vars[i].name,                          
                   heap->vars[i].start,                         
                   heap->vars[i].start + heap->vars[i].size - 1,  
                   heap->vars[i].size);                         
        }
    }
    
    // exibe as áreas livres
    printf("\nÁreas livres:\n");
    freeBlock* curr = heap->freeList;  // ponteiro para percorrer a lista
    // se a lista está vazia
    if (curr == NULL) {
        printf("  (nenhuma)\n");  // informa que não há áreas livres
    } else {  // se há áreas livres
        // percorre a lista encadeada
        while (curr != NULL) {
            // exibe range de endereços e quantidade de blocos
            printf("  [%d-%d] (%d blocos)\n", 
                   curr->start,                  
                   curr->start + curr->size - 1,  
                   curr->size);                  
            curr = curr->next; 
        }
    }
    printf("====================================\n\n");  
}

// exibe o menu de opções
void showMenu() {
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║          SIMULADOR DE GERENCIAMENTO DE HEAP          ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    printf("║  1. Definir estratégia de alocação                   ║\n");
    printf("║  2. Alocar memória (new)                             ║\n");
    printf("║  3. Desalocar memória (del)                          ║\n");
    printf("║  4. Atribuir variável (=)                            ║\n");
    printf("║  5. Exibir estado do heap                            ║\n");
    printf("║  6. Reiniciar heap                                   ║\n");
    printf("║  0. Sair                                             ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");  
}