#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "heap.h"

int main() {
    Heap heap;
    initHeap(&heap);
    
    int opcao;
    char nome[5], nome2[5];
    int tamanho;
    
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║     BEM-VINDO AO SIMULADOR DE GERENCIAMENTO DE HEAP  ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\nHeap inicializado com %d blocos (estratégia: First Fit)\n", HEAP_SIZE);
    
    do {
        showMenu();
        scanf("%d", &opcao);
        clearBuffer();
        
        switch (opcao) {
            case 1: {
                printf("\n┌────────────────────────────────┐\n");
                printf("│  Estratégias de Alocação      │\n");
                printf("├────────────────────────────────┤\n");
                printf("│  1. First Fit                  │\n");
                printf("│  2. Best Fit                   │\n");
                printf("│  3. Worst Fit                  │\n");
                printf("│  4. Next Fit                   │\n");
                printf("└────────────────────────────────┘\n");
                printf("Escolha: ");
                
                int strat;
                scanf("%d", &strat);
                clearBuffer();
                
                switch (strat) {
                    case 1: setStrategy(&heap, FIRST_FIT); break;
                    case 2: setStrategy(&heap, BEST_FIT); break;
                    case 3: setStrategy(&heap, WORST_FIT); break;
                    case 4: setStrategy(&heap, NEXT_FIT); break;
                    default: printf("\n Opção inválida!\n");
                }
                break;
            }
            
            case 2: {
                printf("\n=== Alocar Memória ===\n");
                printf("Nome da variável: ");
                scanf("%s", nome);
                printf("Tamanho (blocos): ");
                scanf("%d", &tamanho);
                clearBuffer();
                
                newAlloc(&heap, nome, tamanho);
                break;
            }
            
            case 3: {
                printf("\n=== Desalocar Memória ===\n");
                
                if (heap.varCount == 0) {
                    printf("\n Não há variáveis alocadas!\n");
                    break;
                }
                
                printf("Variáveis disponíveis: ");
                for (int i = 0; i < heap.varCount; i++) {
                    printf("%s", heap.vars[i].name);
                    if (i < heap.varCount - 1) printf(", ");
                }
                printf("\n");
                
                printf("Nome da variável: ");
                scanf("%s", nome);
                clearBuffer();
                
                delAlloc(&heap, nome);
                break;
            }
            
            case 4: {
                printf("\n=== Atribuir Variável ===\n");
                
                if (heap.varCount < 2) {
                    printf("\n É necessário ter pelo menos 2 variáveis alocadas!\n");
                    break;
                }
                
                printf("Variáveis disponíveis: ");
                for (int i = 0; i < heap.varCount; i++) {
                    printf("%s", heap.vars[i].name);
                    if (i < heap.varCount - 1) printf(", ");
                }
                printf("\n");
                
                printf("Variável destino: ");
                scanf("%s", nome);
                printf("Variável fonte: ");
                scanf("%s", nome2);
                clearBuffer();
                
                assign(&heap, nome, nome2);
                break;
            }
            
            case 5: {
                exibe(&heap);
                break;
            }
            
            case 6: {
                printf("\nTem certeza que deseja reiniciar o heap? (s/n): ");
                char resp;
                scanf("%c", &resp);
                clearBuffer();
                
                if (resp == 's' || resp == 'S') {
                    // libera lista antiga
                    freeBlock* curr = heap.freeList;
                    while (curr != NULL) {
                        freeBlock* temp = curr;
                        curr = curr->next;
                        free(temp);
                    }
                    
                    initHeap(&heap);
                    printf("\nHeap reinicializado!\n");
                }
                break;
            }
            
            case 0: {
                printf("\n╔═══════════════════════════════════════════════════════╗\n");
                printf("║           Encerrando o simulador... Até logo!        ║\n");
                printf("╚═══════════════════════════════════════════════════════╝\n\n");
                break;
            }
            
            default: {
                printf("\nOpção inválida! Tente novamente.\n");
                break;
            }
        }
        
        if (opcao != 0 && opcao != 5) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
        
    } while (opcao != 0);
    
    // libera memória alocada
    freeBlock* curr = heap.freeList;
    while (curr != NULL) {
        freeBlock* temp = curr;
        curr = curr->next;
        free(temp);
    }
    
    return 0;
}