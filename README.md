# Simulador de Gerenciamento de Heap

Um simulador interativo em C projetado para demonstrar e comparar diferentes estratégias de alocação de memória dinâmica (heap). Este projeto permite visualizar o comportamento de cada algoritmo frente à fragmentação de memória, alocação e desalocação de blocos.

## Funcionalidades Principais

* **Simulação Interativa:** Utilize um menu de comandos para manipular o heap em tempo real.
* **Múltiplas Estratégias:** Compare o desempenho de quatro algoritmos clássicos de alocação.
* **Visualização Clara:** Acompanhe o estado atual do heap após cada operação.

## Estratégias de Alocação Suportadas

O simulador implementa e permite alternar entre as seguintes estratégias:

| Estratégia | Descrição | Foco Principal |
| :--- | :--- | :--- |
| **First Fit** | Aloca no **primeiro bloco livre** que seja grande o suficiente para a requisição. | Velocidade de busca. |
| **Best Fit** | Aloca no **menor bloco livre** que seja grande o suficiente. | Minimiza o tamanho do *split* (fragmentação) no bloco selecionado. |
| **Worst Fit** | Aloca no **maior bloco livre** disponível. | Tenta gerar um bloco livre restante (*split*) grande o suficiente para futuras requisições. |
| **Next Fit** | Inicia a busca pelo bloco livre a partir do **local da alocação anterior**. | Reduz o tempo de busca ao distribuir a pesquisa pelo heap. |

## Operações Disponíveis

O menu interativo oferece as seguintes opções para manipulação do heap:

| Operação | Descrição |
| :--- | :--- |
| **Alocar Memória (`new`)** | Solicita um bloco de memória de um determinado tamanho, usando a estratégia de alocação atual. |
| **Desalocar Memória (`del`)** | Libera um bloco alocado por nome. Inclui a **coalescência** (fusão) de blocos livres adjacentes. |
| **Atribuir Referência (`=`)** | Simula a atribuição de ponteiros: a variável de destino passa a **apontar para a mesma região** do heap que a variável fonte. |
| **Visualizar Estado do Heap** | Imprime o mapa atual da memória, mostrando blocos livres (`.`) e ocupados (`#`), além das listas de variáveis alocadas e áreas livres. |
| **Reiniciar Heap** | Zera o estado da simulação. |

## Como Compilar e Executar

O projeto é escrito em C e utiliza o compilador GCC.

### Pré-requisitos

* Compilador **GCC**

### Passos de Compilação

1.  Navegue até o diretório do projeto.
2.  Execute o comando de compilação, incluindo todos os arquivos `.c`:

```bash
gcc main.c heap.c -o heap
````

### Execução

Após a compilação, execute o binário gerado:

```bash
./heap
```

Siga o menu de comandos para escolher a estratégia desejada e começar a simulação.

## Estrutura do Projeto

| Arquivo | Descrição |
| :--- | :--- |
| `heap.h` | Contém as definições de estruturas (`structs`) de dados para os blocos de memória e os protótipos de todas as funções de gerenciamento de heap. |
| `heap.c` | A implementação principal das funções de gerenciamento de heap, incluindo os algoritmos de alocação, a desalocação com coalescência, e a cópia de referência. |
| `main.c` | O arquivo principal que contém o menu de interação com o usuário, o *loop* do programa e a chamada das funções de `heap.c`. |

## Contribuições

Este projeto foi inicialmente desenvolvido como trabalho acadêmico para fins educacionais e de demonstração de algoritmos de alocação de memória. Contribuições são bem-vindas!
