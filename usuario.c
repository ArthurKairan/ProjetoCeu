#include "ceu.h"

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

    if (strchr(novo_usuario.email, '@') == NULL || strlen(novo_usuario.email) >= MAX_EMAIL)
    {
        printf("\nErro: Formato de e-mail invalido ou muito longo.\n");
        pausar();
        return;
    }

    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    if (arquivo != NULL)
    {
        // fscanf para pular os dois primeiros campos e ler apenas o email
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