#include "ceu.h"

// --- FUNÇÕES DE GERENCIAMENTO DA LISTA ---

Node *inserirNaLista(Node *head, Jogo jogo)
{
    Node *novoNode = (Node *)malloc(sizeof(Node));
    if (novoNode == NULL)
    {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    novoNode->dados = jogo;
    novoNode->next = head;
    return novoNode;
}

void liberarLista(Node *head)
{
    Node *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

Node *carregarJogosParaLista()
{
    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    if (arquivo == NULL)
    {
        return NULL;
    }
    Node *head = NULL;
    Jogo jogo;
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        head = inserirNaLista(head, jogo);
    }
    fclose(arquivo);
    return head;
}

void salvarListaParaArquivo(Node *head)
{
    FILE *arquivo = fopen(ARQUIVO_JOGOS, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao salvar os dados!\n");
        return;
    }
    Node *atual = head;

    while (atual != NULL)
    {
        fprintf(arquivo, "%d,%s,%s,%s,%d\n", atual->dados.id, atual->dados.nome, atual->dados.genero, atual->dados.classificacao, atual->dados.streamers_ativos);
        atual = atual->next;
    }
    fclose(arquivo);
}

int obterProximoId(Node *head)
{
    if (head == NULL)
        return 1;
    int max_id = 0;
    Node *atual = head;
    while (atual != NULL)
    {
        if (atual->dados.id > max_id)
        {
            max_id = atual->dados.id;
        }
        atual = atual->next;
    }
    return max_id + 1;
}

int contarNos(Node *head)
{
    int count = 0;
    Node *atual = head;
    while (atual != NULL)
    {
        count++;
        atual = atual->next;
    }
    return count;
}

void cadastrarJogo(Node **p_head)
{
    Jogo novo_jogo;
    novo_jogo.id = obterProximoId(*p_head);

    limparTela();
    printf("--- CADASTRAR NOVO JOGO ---\n");
    printf("Nome do jogo: ");
    scanf("%49[^\n]", novo_jogo.nome);
    limparBuffer();
    printf("Genero (ex: RPG, FPS - sem espacos): ");
    scanf("%29s", novo_jogo.genero);
    limparBuffer();
    printf("Classificacao Indicativa (ex: Livre, 12, 16): ");
    scanf("%9s", novo_jogo.classificacao);
    limparBuffer();
    novo_jogo.streamers_ativos = 0;

    *p_head = inserirNaLista(*p_head, novo_jogo);
    salvarListaParaArquivo(*p_head);
    printf("\nJogo '%s' cadastrado com sucesso com o ID %d!\n", novo_jogo.nome, novo_jogo.id);
}

void listarJogos(Node *head)
{
    limparTela();
    printf("--- CATALOGO DE JOGOS NA CEU ---\n\n");
    if (head == NULL)
    {
        printf("Nenhum jogo cadastrado ainda.\n");
        return;
    }
    Node *atual = head;
    while (atual != NULL)
    {
        printf("ID: %-3d | Nome: %-25s | Genero: %-15s | Class.: %-5s\n",
               atual->dados.id, atual->dados.nome, atual->dados.genero, atual->dados.classificacao);
        atual = atual->next;
    }
}

void editarJogo(Node *head)
{
    int id_editar, encontrado = 0;
    listarJogos(head);
    if (head == NULL)
        return;

    printf("\nDigite o ID do jogo que deseja editar: ");
    scanf("%d", &id_editar);
    limparBuffer();

    Node *atual = head;
    while (atual != NULL)
    {
        if (atual->dados.id == id_editar)
        {
            encontrado = 1;
            printf("\nEditando o jogo '%s'.\n", atual->dados.nome);
            printf("Novo nome: ");
            scanf("%49[^\n]", atual->dados.nome);
            limparBuffer();
            printf("Novo genero (sem espacos): ");
            scanf("%29s", atual->dados.genero);
            limparBuffer();
            printf("Nova classificacao: ");
            scanf("%9s", atual->dados.classificacao);
            limparBuffer();

            salvarListaParaArquivo(head);
            printf("\nJogo atualizado com sucesso!\n");
            break;
        }
        atual = atual->next;
    }

    if (!encontrado)
    {
        printf("\nJogo com ID %d nao encontrado.\n", id_editar);
    }
}

Node *removerDaLista(Node *head, int id_remover, int *p_encontrado)
{
    Node *atual = head, *prev = NULL;
    *p_encontrado = 0;

    if (atual != NULL && atual->dados.id == id_remover)
    {
        head = atual->next;
        free(atual);
        *p_encontrado = 1;
        return head;
    }

    while (atual != NULL && atual->dados.id != id_remover)
    {
        prev = atual;
        atual = atual->next;
    }

    if (atual == NULL)
        return head;

    prev->next = atual->next;
    free(atual);
    *p_encontrado = 1;
    return head;
}

void excluirJogo(Node **p_head)
{
    int id_excluir, encontrado = 0;
    listarJogos(*p_head);
    if (*p_head == NULL)
        return;

    printf("\nDigite o ID do jogo que deseja excluir: ");
    scanf("%d", &id_excluir);
    limparBuffer();

    *p_head = removerDaLista(*p_head, id_excluir, &encontrado);

    if (encontrado)
    {
        salvarListaParaArquivo(*p_head);
        printf("\nJogo excluido com sucesso!\n");
    }
    else
    {
        printf("\nJogo com ID %d nao encontrado.\n", id_excluir);
    }
}

// --- ALGORITMOS DE ORDENAÇÃO E BUSCA ---

// critério 0 para ID, 1 para streamers
int compararJogos(const Jogo *a, const Jogo *b, int criterio)
{
    if (criterio == 1)
    { // streamers decrescente
        return b->streamers_ativos - a->streamers_ativos;
    }
    return a->id - b->id; // ID crescente
}

void merge(Jogo arr[], int l, int m, int r, int criterio)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Jogo *L = (Jogo *)malloc(n1 * sizeof(Jogo));
    Jogo *R = (Jogo *)malloc(n2 * sizeof(Jogo));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (compararJogos(&L[i], &R[j], criterio) <= 0)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(Jogo arr[], int l, int r, int criterio)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, criterio);
        mergeSort(arr, m + 1, r, criterio);
        merge(arr, l, m, r, criterio);
    }
}

Jogo *buscaBinariaPorId(int id_busca, Jogo arr[], int n)
{
    int esquerda = 0, direita = n - 1;
    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;
        if (arr[meio].id == id_busca)
            return &arr[meio];
        if (arr[meio].id < id_busca)
            esquerda = meio + 1;
        else
            direita = meio - 1;
    }
    return NULL;
}

// --- FUNÇÕES DE OPERAÇÃO ESPECÍFICAS ---

// Função que encapsula a lógica de criar um vetor, ordenar e buscar
void operacaoBuscarJogoPorId(Node *head)
{
    if (head == NULL)
    {
        printf("\nNenhum jogo no catalogo para buscar.\n");
        return;
    }

    int id_busca;
    printf("Digite o ID do jogo a buscar: ");
    scanf("%d", &id_busca);
    limparBuffer();

    int n = contarNos(head);
    Jogo *arr = (Jogo *)malloc(n * sizeof(Jogo));
    if (arr == NULL)
    {
        printf("Erro de memoria!\n");
        return;
    }

    // Copia da lista para o vetor
    Node *atual = head;
    for (int i = 0; i < n; i++)
    {
        arr[i] = atual->dados;
        atual = atual->next;
    }

    // Ordena o vetor por ID para a busca binária
    mergeSort(arr, 0, n - 1, 0);

    Jogo *resultado = buscaBinariaPorId(id_busca, arr, n);

    limparTela();
    printf("--- RESULTADO DA BUSCA BINARIA ---\n\n");
    if (resultado != NULL)
    {
        printf("Jogo Encontrado!\n");
        printf("ID: %d\nNome: %s\nGenero: %s\n", resultado->id, resultado->nome, resultado->genero);
    }
    else
    {
        printf("Jogo com ID %d nao encontrado.\n", id_busca);
    }

    free(arr);
}

void operacaoVerJogosEmAlta(Node *head)
{
    if (head == NULL)
    {
        printf("\nNenhum jogo no catalogo.\n");
        return;
    }

    int n = contarNos(head);
    Jogo *arr = (Jogo *)malloc(n * sizeof(Jogo));
    if (arr == NULL)
    {
        printf("Erro de memoria!\n");
        return;
    }

    Node *atual = head;
    for (int i = 0; i < n; i++)
    {
        arr[i] = atual->dados;
        atual = atual->next;
    }

    // Ordena por streamers (critério 1)
    mergeSort(arr, 0, n - 1, 1);

    limparTela();
    printf("--- JOGOS EM ALTA NA CEU ---\n\n");
    int limite = n < 5 ? n : 5;
    for (int i = 0; i < limite; i++)
    {
        printf("#%d - Nome: %-25s | Streamers Ativos: %d\n", i + 1, arr[i].nome, arr[i].streamers_ativos);
    }

    free(arr);
}

// Funções restantes
void buscarJogoPorNome(Node *head) { /* Percorre a lista e usa strstr */ }
void filtrarPorGenero(Node *head) { /* Percorre a lista e usa strcmp */ }
void simularStream(Node *head) { /* Encontra o jogo na lista e imprime a msg */ }
void sugerirJogoAleatorio(Node *head) { /* Conta os nós, gera numero aleatorio e percorre a lista até ele */ }