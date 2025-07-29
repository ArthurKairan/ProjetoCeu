#include "ceu.h"

// Implementação das funções de menu e utilitárias

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

void menuInicial(Node **p_head)
{
    int opcao;
    do
    {
        limparTela();
        printf("======================================\n");
        printf("   BEM-VINDO A PLATAFORMA CEU (v2.0)\n");
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
                menuPrincipal(p_head);
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
            printf("\nObrigado por usar a Ceu. Ate mais!\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            pausar();
            break;
        }
    } while (opcao != 0);
}

void menuPrincipal(Node **p_head)
{
    int opcao;
    do
    {
        limparTela();
        printf("\nMENU PRINCIPAL - PLATAFORMA CEU\n\n");
        printf("[1] Cadastrar novo jogo\n");
        printf("[2] Listar todos os jogos\n");
        printf("[3] Editar informacoes de um jogo\n");
        printf("[4] Excluir um jogo\n");
        printf("-------------------------------------------\n");
        printf("[5] Buscar jogo por ID (Busca Binaria)\n");
        printf("[6] Ver jogos \"Em Alta\" (Merge Sort)\n");
        printf("[7] Buscar jogo por nome (Busca Linear)\n");
        printf("[8] Filtrar jogos por genero\n");
        printf("[9] Simular uma Stream\n");
        printf("[10] Sugerir Jogo Aleatorio\n");
        printf("-------------------------------------------\n");
        printf("[0] Deslogar\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao)
        {
        case 1:
            cadastrarJogo(p_head);
            break;
        case 2:
            listarJogos(*p_head);
            break;
        case 3:
            editarJogo(*p_head);
            break;
        case 4:
            excluirJogo(p_head);
            break;
        case 5:
            operacaoBuscarJogoPorId(*p_head);
            break;
        case 6:
            operacaoVerJogosEmAlta(*p_head);
            break;
        case 7:
            buscarJogoPorNome(*p_head);
            break;
        case 8:
            filtrarPorGenero(*p_head);
            break;
        case 9:
            simularStream(*p_head);
            break;
        case 10:
            sugerirJogoAleatorio(*p_head);
            break;
        case 0:
            printf("\nDesconectando...\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            break;
        }
        if (opcao != 0)
            pausar();

    } while (opcao != 0);
}

int main()
{
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    Node *lista_de_jogos = NULL;

    // Carrega os jogos do arquivo para a lista em memória ao iniciar
    lista_de_jogos = carregarJogosParaLista();

    menuInicial(&lista_de_jogos);

    // Libera a memória alocada pela lista antes de sair
    liberarLista(lista_de_jogos);

    return 0;
}