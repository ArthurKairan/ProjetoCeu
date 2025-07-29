#ifndef CEU_H
#define CEU_H

// Includes Padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes
#define MAX_NOME 50
#define MAX_SENHA 30
#define MAX_EMAIL 80
#define MAX_GENERO 30
#define MAX_CLASSIFICACAO 10
#define ARQUIVO_USUARIOS "usuarios.csv"
#define ARQUIVO_JOGOS "jogos.csv"

// Estruturas de Dados

// Struct para os dados do usuário
typedef struct
{
    char username[MAX_NOME];
    char password[MAX_SENHA];
    char email[MAX_EMAIL];
} Usuario;

// Struct para os dados do jogo
typedef struct
{
    int id;
    char nome[MAX_NOME];
    char genero[MAX_GENERO];
    char classificacao[MAX_CLASSIFICACAO];
    int streamers_ativos;
} Jogo;

// Nó da Lista Encadeada de Jogos
typedef struct Node
{
    Jogo dados;
    struct Node *next;
} Node;

// Protótipos das Funções

// Funções Utilitárias (para o main.c)
void limparTela();
void limparBuffer();
void pausar();

// Funções de Usuário (para o usuario.c)
void cadastrarUsuario();
int realizarLogin();

// Funções de Menu (main.c)
void menuInicial(Node **p_head);
void menuPrincipal(Node **p_head);

// Funções de Gerenciamento da Lista (jogo.c)
Node *inserirNaLista(Node *head, Jogo jogo);
void liberarLista(Node *head);
Node *carregarJogosParaLista();
void salvarListaParaArquivo(Node *head);
Node *removerDaLista(Node *head, int id_remover, int *encontrado);
int contarNos(Node *head);

// Funções CRUD sobre a Lista (jogo.c)
void cadastrarJogo(Node **p_head);
void listarJogos(Node *head);
void editarJogo(Node *head);
void excluirJogo(Node **p_head);

// Funções de Algoritmos e Específicas (jogo.c)
void merge(Jogo arr[], int l, int m, int r, int criterio);
void mergeSort(Jogo arr[], int l, int r, int criterio);
Jogo *buscaBinariaPorId(int id_busca, Jogo arr[], int n);
void operacaoVerJogosEmAlta(Node *head);
void operacaoBuscarJogoPorId(Node *head);
void buscarJogoPorNome(Node *head);
void filtrarPorGenero(Node *head);
void simularStream(Node *head);
void sugerirJogoAleatorio(Node *head);

#endif // CEU_H