#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes para tamanhos de strings
#define MAX_NOME 50
#define MAX_SENHA 30
#define MAX_EMAIL 80
#define MAX_GENERO 30
#define MAX_CLASSIFICACAO 10
#define MAX_LINHA 256 // Tamanho máximo de uma linha nos arquivos

// Nomes dos arquivos
#define ARQUIVO_USUARIOS "usuarios.csv"
#define ARQUIVO_JOGOS "jogos.csv"

// STRUCTS
typedef struct
{
    char username[MAX_NOME];
    char password[MAX_SENHA];
    char email[MAX_EMAIL];
} Usuario;

typedef struct
{
    int id;
    char nome[MAX_NOME];
    char genero[MAX_GENERO];
    char classificacao[MAX_CLASSIFICACAO];
    int streamers_ativos;
} Jogo;

// PROTÓTIPOS DAS FUNÇÕES ---

void limparTela();
void limparBuffer();
void pausar();
int obterProximoIdJogo();

// Funções de Usuário e Login
void cadastrarUsuario();
int realizarLogin();
void menuInicial();

// Menu Principal
void menuPrincipal();

// Funções CRUD de Jogos
void cadastrarJogo();
void listarJogos();
void editarJogo();
void excluirJogo();

// Funções Específicas da Plataforma
void buscarJogoPorNome();
void filtrarPorGenero();
void verJogosEmAlta();
void simularStream();
void sugerirJogoAleatorio();
int compararJogos(const void *a, const void *b);

// FUNÇÃO PRINCIPAL

int main()
{
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    menuInicial();
    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

void limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pausar()
{
    printf("\nPressione Enter para continuar...");
    limparBuffer();
}

void menuInicial()
{
    int opcao;
    do
    {
        limparTela();
        printf("======================================\n");
        printf("   BEM-VINDO A PLATAFORMA CEU\n");
        printf("======================================\n");
        printf("[1] Realizar Login\n");
        printf("[2] Cadastrar Novo Usuario\n");
        printf("[0] Sair\n");
        printf("--------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
            if (realizarLogin())
            {
                menuPrincipal();
            }
            else
            {
                printf("\nFalha no login.\n");
                pausar();
            }
            break;
        case 2:
            cadastrarUsuario();
            break;
        case 0:
            printf("\nObrigado por visitar a Ceu. Ate mais!\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            pausar();
            break;
        }
    } while (opcao != 0);
}

// FUNÇÕES DE USUÁRIO E LOGIN
int realizarLogin()
{
    char username[MAX_NOME];
    char password[MAX_SENHA];
    Usuario usuario_lido;
    int login_sucesso = 0;

    limparTela();
    printf("--- LOGIN ---\n");
    printf("Usuario: ");
    scanf("%49s", username);
    limparBuffer();

    printf("Senha: ");
    scanf("%29s", password);
    limparBuffer();

    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    if (arquivo == NULL)
    {
        printf("\nNenhum usuario cadastrado. Por favor, cadastre-se primeiro.\n");
        return 0;
    }

    while (fscanf(arquivo, "%[^,],%[^,],%[^\n]\n", usuario_lido.username, usuario_lido.password, usuario_lido.email) != EOF)
    {
        if (strcmp(username, usuario_lido.username) == 0 && strcmp(password, usuario_lido.password) == 0)
        {
            login_sucesso = 1;
            break;
        }
    }

    fclose(arquivo);
    return login_sucesso;
}

void cadastrarUsuario()
{
    Usuario novo_usuario;
    Usuario usuario_existente;
    int email_valido = 1;

    limparTela();
    printf("--- CADASTRO DE NOVO USUARIO ---\n");
    printf("Digite o nome de usuario: ");
    scanf("%49s", novo_usuario.username);
    limparBuffer();

    printf("Digite a senha: ");
    scanf("%29s", novo_usuario.password);
    limparBuffer();

    printf("Digite o e-mail: ");
    scanf("%79s", novo_usuario.email);
    limparBuffer();

    // Validação do e-mail
    if (strchr(novo_usuario.email, '@') == NULL || strlen(novo_usuario.email) >= MAX_EMAIL)
    {
        printf("\nErro: Formato de e-mail invalido ou muito longo.\n");
        pausar();
        return;
    }

    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    if (arquivo != NULL)
    {
        while (fscanf(arquivo, "%*[^,],%*[^,],%[^\n]\n", usuario_existente.email) != EOF)
        {
            if (strcmp(novo_usuario.email, usuario_existente.email) == 0)
            {
                printf("\nErro: Este e-mail ja esta cadastrado!\n");
                email_valido = 0;
                break;
            }
        }
        fclose(arquivo);
    }

    if (!email_valido)
    {
        pausar();
        return;
    }

    // novo usuário
    arquivo = fopen(ARQUIVO_USUARIOS, "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de usuarios!\n");
        pausar();
        return;
    }
    fprintf(arquivo, "%s,%s,%s\n", novo_usuario.username, novo_usuario.password, novo_usuario.email);
    fclose(arquivo);

    printf("\nUsuario cadastrado com sucesso!\n");
    pausar();
}

// FUNÇÕES DO MENU PRINCIPAL E CRUD

void menuPrincipal()
{
    int opcao;
    do
    {
        limparTela();
        printf("\n--------------------------------------------------\n");
        printf("Bem-vindo(a) ao Ceu, sua plataforma de voo no mundo dos games!\n");
        printf("--------------------------------------------------\n\n");
        printf("MENU PRINCIPAL - PLATAFORMA CEU\n\n");
        printf("[1] Cadastrar novo jogo\n");
        printf("[2] Listar todos os jogos\n");
        printf("[3] Editar informacoes de um jogo\n");
        printf("[4] Excluir um jogo\n");
        printf("-------------------------------------------\n");
        printf("[5] Buscar um jogo por nome\n");
        printf("[6] Filtrar jogos por genero\n");
        printf("[7] Ver jogos \"Em Alta\"\n");
        printf("[8] Simular uma Stream\n");
        printf("[9] Descubra um Novo Ceu (Jogo Aleatorio)\n");
        printf("-------------------------------------------\n");
        printf("[0] Deslogar\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
            cadastrarJogo();
            break;
        case 2:
            listarJogos();
            break;
        case 3:
            editarJogo();
            break;
        case 4:
            excluirJogo();
            break;
        case 5:
            buscarJogoPorNome();
            break;
        case 6:
            filtrarPorGenero();
            break;
        case 7:
            verJogosEmAlta();
            break;
        case 8:
            simularStream();
            break;
        case 9:
            sugerirJogoAleatorio();
            break;
        case 0:
            printf("\nDesconectando do Ceu...\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            break;
        }
        if (opcao != 0)
            pausar();

    } while (opcao != 0);
}

int obterProximoIdJogo()
{
    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    if (arquivo == NULL)
        return 1;

    Jogo jogo;
    int max_id = 0;
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        if (jogo.id > max_id)
        {
            max_id = jogo.id;
        }
    }
    fclose(arquivo);
    return max_id + 1;
}

void cadastrarJogo()
{
    Jogo novo_jogo;
    novo_jogo.id = obterProximoIdJogo();

    limparTela();
    printf("--- CADASTRAR NOVO JOGO ---\n");
    printf("Nome do jogo: ");
    scanf("%49[^\n]", novo_jogo.nome);
    limparBuffer();
    printf("Genero (ex: RPG, FPS, Aventura - sem espacos): ");
    scanf("%29s", novo_jogo.genero);
    limparBuffer();
    printf("Classificacao Indicativa (ex: Livre, 12, 16): ");
    scanf("%9s", novo_jogo.classificacao);
    limparBuffer();
    novo_jogo.streamers_ativos = 0; // Jogo novo começa com 0 streamers

    FILE *arquivo = fopen(ARQUIVO_JOGOS, "a");
    if (arquivo == NULL)
    {
        printf("\nErro ao abrir o arquivo de jogos!\n");
        return;
    }

    fprintf(arquivo, "%d,%s,%s,%s,%d\n", novo_jogo.id, novo_jogo.nome, novo_jogo.genero, novo_jogo.classificacao, novo_jogo.streamers_ativos);
    fclose(arquivo);

    printf("\nJogo '%s' cadastrado com sucesso com o ID %d!\n", novo_jogo.nome, novo_jogo.id);
}

void listarJogos()
{
    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    limparTela();
    printf("--- CATALOGO DE JOGOS NA CEU ---\n\n");

    if (arquivo == NULL)
    {
        printf("Nenhum jogo cadastrado ainda.\n");
        return;
    }

    Jogo jogo;
    int encontrou_jogos = 0;
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        printf("ID: %-3d | Nome: %-25s | Genero: %-15s | Class.: %-5s | Streamers: %d\n",
               jogo.id, jogo.nome, jogo.genero, jogo.classificacao, jogo.streamers_ativos);
        encontrou_jogos = 1;
    }

    if (!encontrou_jogos)
    {
        printf("O catalogo esta vazio.\n");
    }

    fclose(arquivo);
}

void editarJogo()
{
    int id_editar, encontrado = 0;
    listarJogos();
    printf("\nDigite o ID do jogo que deseja editar: ");
    scanf("%d", &id_editar);
    limparBuffer();

    FILE *arquivo_original = fopen(ARQUIVO_JOGOS, "r");
    FILE *arquivo_temp = fopen("temp.csv", "w");

    if (arquivo_original == NULL || arquivo_temp == NULL)
    {
        printf("Erro ao processar arquivos.\n");
        return;
    }

    Jogo jogo;
    while (fscanf(arquivo_original, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        if (jogo.id == id_editar)
        {
            encontrado = 1;
            printf("\nEditando o jogo '%s'.\n", jogo.nome);
            printf("Novo nome: ");
            scanf("%49[^\n]", jogo.nome);
            limparBuffer();
            printf("Novo genero (sem espacos): ");
            scanf("%29s", jogo.genero);
            limparBuffer();
            printf("Nova classificacao: ");
            scanf("%9s", jogo.classificacao);
            limparBuffer();
        }
        fprintf(arquivo_temp, "%d,%s,%s,%s,%d\n", jogo.id, jogo.nome, jogo.genero, jogo.classificacao, jogo.streamers_ativos);
    }

    fclose(arquivo_original);
    fclose(arquivo_temp);

    remove(ARQUIVO_JOGOS);
    rename("temp.csv", ARQUIVO_JOGOS);

    if (encontrado)
    {
        printf("\nJogo atualizado com sucesso!\n");
    }
    else
    {
        printf("\nJogo com ID %d nao encontrado.\n", id_editar);
    }
}

void excluirJogo()
{
    int id_excluir, encontrado = 0;
    listarJogos();
    printf("\nDigite o ID do jogo que deseja excluir: ");
    scanf("%d", &id_excluir);
    limparBuffer();

    FILE *arquivo_original = fopen(ARQUIVO_JOGOS, "r");
    FILE *arquivo_temp = fopen("temp.csv", "w");

    if (arquivo_original == NULL || arquivo_temp == NULL)
    {
        printf("Erro ao processar arquivos.\n");
        return;
    }

    Jogo jogo;
    while (fscanf(arquivo_original, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        if (jogo.id == id_excluir)
        {
            encontrado = 1;
            printf("\nJogo '%s' sera excluido.\n", jogo.nome);
        }
        else
        {
            fprintf(arquivo_temp, "%d,%s,%s,%s,%d\n", jogo.id, jogo.nome, jogo.genero, jogo.classificacao, jogo.streamers_ativos);
        }
    }

    fclose(arquivo_original);
    fclose(arquivo_temp);

    remove(ARQUIVO_JOGOS);
    rename("temp.csv", ARQUIVO_JOGOS);

    if (encontrado)
    {
        printf("\nJogo excluido com sucesso!\n");
    }
    else
    {
        printf("\nJogo com ID %d nao encontrado.\n", id_excluir);
    }
}

// FUNÇÕES ESPECÍFICAS DA PLATAFORMA

void buscarJogoPorNome()
{
    char nome_busca[MAX_NOME];
    limparTela();
    printf("--- BUSCAR JOGO POR NOME ---\n");
    printf("Digite o nome ou parte do nome do jogo: ");
    scanf("%49[^\n]", nome_busca);
    limparBuffer();

    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    if (arquivo == NULL)
    {
        printf("\nCatalogo vazio.\n");
        return;
    }

    Jogo jogo;
    int encontrou = 0;
    printf("\nResultados da busca por '%s':\n\n", nome_busca);
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        // strstr encontra a primeira ocorrência de uma substring (case-sensitive)
        if (strstr(jogo.nome, nome_busca) != NULL)
        {
            printf("ID: %-3d | Nome: %-25s | Genero: %s\n", jogo.id, jogo.nome, jogo.genero);
            encontrou = 1;
        }
    }
    fclose(arquivo);

    if (!encontrou)
    {
        printf("Nenhum jogo encontrado com este nome.\n");
    }
}

void filtrarPorGenero()
{
    char genero_busca[MAX_GENERO];
    limparTela();
    printf("--- FILTRAR JOGOS POR GENERO ---\n");
    printf("Digite o genero (ex: RPG, FPS, Aventura - sem espacos): ");
    scanf("%29s", genero_busca);
    limparBuffer();

    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    if (arquivo == NULL)
    {
        printf("\nCatalogo vazio.\n");
        return;
    }

    Jogo jogo;
    int encontrou = 0;
    printf("\nJogos do genero '%s':\n\n", genero_busca);
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        if (strcmp(jogo.genero, genero_busca) == 0)
        {
            printf("ID: %-3d | Nome: %-25s | Streamers: %d\n", jogo.id, jogo.nome, jogo.streamers_ativos);
            encontrou = 1;
        }
    }
    fclose(arquivo);

    if (!encontrou)
    {
        printf("Nenhum jogo encontrado com este genero.\n");
    }
}

// Função de comparação para o qsort, ordena por streamers_ativos (decrescente)
int compararJogos(const void *a, const void *b)
{
    Jogo *jogoA = (Jogo *)a;
    Jogo *jogoB = (Jogo *)b;
    return (jogoB->streamers_ativos - jogoA->streamers_ativos);
}

void verJogosEmAlta()
{
    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    limparTela();
    printf("--- JOGOS EM ALTA NA CEU ---\n\n");

    if (arquivo == NULL)
    {
        printf("Catalogo vazio.\n");
        return;
    }

    Jogo jogos[100]; // Suporta até 100 jogos
    int count = 0;
    while (count < 100 && fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogos[count].id, jogos[count].nome, jogos[count].genero, jogos[count].classificacao, &jogos[count].streamers_ativos) != EOF)
    {
        count++;
    }
    fclose(arquivo);

    if (count == 0)
    {
        printf("Nenhum jogo para mostrar.\n");
        return;
    }

    qsort(jogos, count, sizeof(Jogo), compararJogos);

    // Exibe os 5 jogos mais populares (ou menos, se houver menos de 5 kkkkk)
    int limite = count < 5 ? count : 5;
    for (int i = 0; i < limite; i++)
    {
        printf("#%d - Nome: %-25s | Streamers Ativos: %d\n", i + 1, jogos[i].nome, jogos[i].streamers_ativos);
    }
}

void simularStream()
{
    int id_stream;
    listarJogos();
    printf("\nDigite o ID do jogo que deseja transmitir: ");
    scanf("%d", &id_stream);
    limparBuffer();

    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    if (arquivo == NULL)
    {
        printf("Catalogo vazio.\n");
        return;
    }

    Jogo jogo;
    int encontrado = 0;
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos) != EOF)
    {
        if (jogo.id == id_stream)
        {
            printf("\n-----------------------------------------\n");
            printf("Iniciando transmissao de '%s'...\n", jogo.nome);
            printf("3...\n2...\n1...\n");
            printf("VOCE ESTA AO VIVO NA CEU!\n");
            printf("-----------------------------------------\n");
            encontrado = 1;
            break;
        }
    }
    fclose(arquivo);

    if (!encontrado)
    {
        printf("\nJogo com ID %d nao encontrado.\n", id_stream);
    }
}

void sugerirJogoAleatorio()
{
    FILE *arquivo = fopen(ARQUIVO_JOGOS, "r");
    limparTela();
    printf("--- DESCUBRA UM NOVO CEU ---\n\n");
    if (arquivo == NULL)
    {
        printf("Catalogo vazio, impossivel sugerir.\n");
        return;
    }

    int count = 0;
    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        count++;
    }

    if (count == 0)
    {
        printf("Nenhum jogo no catalogo.\n");
        fclose(arquivo);
        return;
    }

    int indice_aleatorio = rand() % count;
    rewind(arquivo); // Vai voltar pro inicio do arquivo

    for (int i = 0; i < indice_aleatorio; i++)
    {
        fgets(linha, sizeof(linha), arquivo);
    }

    Jogo jogo;
    fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d\n", &jogo.id, jogo.nome, jogo.genero, jogo.classificacao, &jogo.streamers_ativos);

    printf("Que tal transmitir este jogo?\n\n");
    printf("Nome: %s\n", jogo.nome);
    printf("Genero: %s\n", jogo.genero);
    printf("Classificacao: %s\n", jogo.classificacao);

    fclose(arquivo);
}