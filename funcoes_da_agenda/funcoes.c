#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../validacoes/validacoes.h"
#include "../menus/menu_templates.h"

#define TAMANHO_NOME 31
#define TAMANHO_DATA_DE_NASCIMENTO 11
#define TAMANHO_CPF 12
#define TAMANHO_EMAIL 100
#define TAMANHO_TELEFONE 14

// structs
typedef struct p
{
    int IdPessoa;
    char nome[TAMANHO_NOME];
    char data_de_nascimento[TAMANHO_DATA_DE_NASCIMENTO];
    char cpf[TAMANHO_CPF];
    char email[TAMANHO_EMAIL];
} PESSOA;

typedef struct t
{
    int IdTelefone;
    int IdPessoa;
    char telefone[TAMANHO_TELEFONE];
} TELEFONE;

// vetores de structs dinamicamente alocados
PESSOA *LISTA_PESSOAS = NULL;
TELEFONE *LISTA_TELEFONES = NULL;
int QTD_PESSOAS = 0;
int QTD_TELEFONES = 0;

// Headers de funções auxiliares que serão usadas por outras funções
int contar_registros(const char *nome_arquivo);
void limpar_memoria();
void reescrever_pessoas();
void reescrever_telefones();
int buscar_pessoa(int metodo);
int buscar_pessoa_nome(const char *nome);
int buscar_pessoa_IdPessoa(int id);
int gerar_IdPessoa();
int gerar_IdTelefone();
bool salvar_pessoa(PESSOA pessoa);
void cadastrar_telefone(int metodo_busca, int index_pessoa_existente);
bool salvar_telefone(TELEFONE telefone);

// funções
char *input()
{
    char buffer[1024];
    char *string = NULL;
    size_t tamanho = 0;

    if (fgets(buffer, sizeof(buffer), stdin))
    {
        tamanho = strlen(buffer);

        if (buffer[tamanho - 1] == '\n')
        {
            buffer[tamanho - 1] = '\0';
            tamanho--;
        }

        string = malloc(tamanho + 1);
        if (string)
        {
            strcpy(string, buffer);
        }
    }

    return string;
}

int contar_registros(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return -1;
    }

    int linhas = 0;
    char caracter;
    bool tem_dados = false;
    fseek(arquivo, 0, SEEK_SET);
    while ((caracter = fgetc(arquivo)) != EOF)
    {
        tem_dados = true;
        if (caracter == '\n')
            linhas++;
    }

    fseek(arquivo, -1, SEEK_END);
    caracter = fgetc(arquivo);
    if (caracter != '\n')
        linhas++;

    fclose(arquivo);
    return linhas;
}

void limpar_memoria()
{
    free(LISTA_PESSOAS);
    free(LISTA_TELEFONES);
    LISTA_PESSOAS = NULL;
    LISTA_TELEFONES = NULL;
    printf("\n");
    printf("============================================\n");
    printf("Finalizando...\n");
    printf("Programa encerrado!\n");
    printf("============================================\n");

    exit(0);
}

void inicializar_arquivos()
{
    FILE *arquivo_pessoas = fopen("pessoas.txt", "a");
    if (arquivo_pessoas == NULL)
    {
        printf("Erro ao criar pessoas.txt\n");
        exit(EXIT_FAILURE);
    }
    fclose(arquivo_pessoas);

    FILE *arquivo_telefones = fopen("telefones.txt", "a");
    if (arquivo_telefones == NULL)
    {
        printf("Erro ao criar telefones.txt\n");
        exit(EXIT_FAILURE);
    }
    fclose(arquivo_telefones);
}

bool carregar_pessoas()
{
    FILE *arquivo_pessoas = fopen("pessoas.txt", "r");
    if (arquivo_pessoas == NULL)
    {
        printf("Erro ao abrir pessoas.txt\n");
        limpar_memoria();
        return false;
    }

    if (QTD_PESSOAS == 1) 
    {
        LISTA_PESSOAS[0].IdPessoa = 0;
        strcpy(LISTA_PESSOAS[0].nome, "nome");
        strcpy(LISTA_PESSOAS[0].data_de_nascimento, "data");
        strcpy(LISTA_PESSOAS[0].cpf, "CPF");
        strcpy(LISTA_PESSOAS[0].email, "E-mail");

        fclose(arquivo_pessoas);
        return true;
    }

    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        int itens_lidos = fscanf(arquivo_pessoas, "%d;%30[^;];%10[^;];%11[^;];%99[^\n]\n",
                                 &LISTA_PESSOAS[i].IdPessoa,
                                 LISTA_PESSOAS[i].nome,
                                 LISTA_PESSOAS[i].data_de_nascimento,
                                 LISTA_PESSOAS[i].cpf,
                                 LISTA_PESSOAS[i].email);
        if (itens_lidos != 5)
        {
            printf("ITENS LIDOS: %d\n", itens_lidos);
            printf("Erro ao ler linha %d do arquivo pessoas.txt\n", i + 1);
            limpar_memoria();
            fclose(arquivo_pessoas);
            return false;
        }
    }
    fclose(arquivo_pessoas);

    return true;
}

bool carregar_telefones()
{
    FILE *arquivo_telefones = fopen("telefones.txt", "r");
    if (arquivo_telefones == NULL)
    {
        printf("Erro ao abrir telefones.txt\n");
        limpar_memoria();
        return false;
    }

    if (QTD_TELEFONES == 1)
    {
        LISTA_TELEFONES[0].IdTelefone = 0;
        LISTA_TELEFONES[0].IdPessoa = 0;
        strcpy(LISTA_TELEFONES[0].telefone, "telefone");

        fclose(arquivo_telefones);
        return true;
    }

    for (int i = 0; i < QTD_TELEFONES; i++)
    {
        int itens_lidos = fscanf(arquivo_telefones, "%d;%d;%13[^\n]\n",
               &LISTA_TELEFONES[i].IdTelefone,
               &LISTA_TELEFONES[i].IdPessoa,
               LISTA_TELEFONES[i].telefone);

        if (itens_lidos != 3) 
        {
            printf("Erro ao ler linha %d do arquivo telefones.txt\n", i + 1);
            limpar_memoria();
            fclose(arquivo_telefones);
            return false;
        }
    }
    fclose(arquivo_telefones);

    return true;
}

bool carregar_arquivos_na_memoria()
{
    inicializar_arquivos();

    QTD_PESSOAS = contar_registros("pessoas.txt");
    QTD_TELEFONES = contar_registros("telefones.txt");

    LISTA_PESSOAS = (PESSOA *)malloc(QTD_PESSOAS * sizeof(PESSOA));
    if (LISTA_PESSOAS == NULL)
    {
        printf("erro ao alocar vetor de pessoas na memória\n");
        return false;
    }

    LISTA_TELEFONES = (TELEFONE *)malloc(QTD_TELEFONES * sizeof(TELEFONE));
    if (LISTA_TELEFONES == NULL)
    {
        printf("erro ao alocar vetor de telefones na memória\n");
        return false;
    }

    if (!carregar_pessoas())
    {
        printf("Erro ao carregar arquivo pessoas.txt na memória\n");
        return false;
    }

    if (!carregar_telefones())
    {
        printf("Erro ao carregar arquivo telefones.txt na memória\n");
        return false;
    }

    return true;
}

void reescrever_pessoas()
{
    FILE *arq_pessoas = fopen("pessoas.txt", "w");
    if (arq_pessoas == NULL)
    {
        printf("Erro ao abrir o arquivo pessoas.txt\n");
        return;
    }
    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        fprintf(arq_pessoas, "%d;%s;%s;%s;%s\n",
                LISTA_PESSOAS[i].IdPessoa,
                LISTA_PESSOAS[i].nome,
                LISTA_PESSOAS[i].data_de_nascimento,
                LISTA_PESSOAS[i].cpf,
                LISTA_PESSOAS[i].email);
    }
    fclose(arq_pessoas);
}

void reescrever_telefones()
{
    FILE *arq_telefones = fopen("telefones.txt", "w");
    if (arq_telefones == NULL)
    {
        printf("Erro ao abrir o arquivo telefones.txt\n");
        return;
    }
    for (int i = 0; i < QTD_TELEFONES; i++)
    {
        fprintf(arq_telefones, "%d;%d;%s\n", LISTA_TELEFONES[i].IdTelefone, LISTA_TELEFONES[i].IdPessoa, LISTA_TELEFONES[i].telefone);
    }
    fclose(arq_telefones);
}

void limpar_buffer_entrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int buscar_pessoa_nome(const char *nome)
{
    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        if (strcmp(nome, LISTA_PESSOAS[i].nome) == 0)
        {
            return i;
        }
    }
    return -1;
}

int buscar_pessoa_IdPessoa(int id)
{
    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        if (id == LISTA_PESSOAS[i].IdPessoa)
            return i;
    }

    return -1;
}

int buscar_pessoa(int metodo)
{
    // busca por nome
    if (metodo == 1)
    {
        printf("Digite o nome do contato: ");
        limpar_buffer_entrada();
        char *nome = input();
        int index = buscar_pessoa_nome(nome);
        free(nome);
        return index;
    }
    // busca por ID
    if (metodo == 2)
    {
        int id;
        printf("Digite o ID do contato: ");
        scanf("%d", &id);

        return buscar_pessoa_IdPessoa(id);
    }
}

int gerar_IdPessoa()
{
    return LISTA_PESSOAS[QTD_PESSOAS - 1].IdPessoa + 1;
}

int gerar_IdTelefone()
{
    return LISTA_TELEFONES[QTD_TELEFONES - 1].IdTelefone + 1;
}

void cadastrar_pessoa()
{
    printf("Cadastrar Pessoa: \n");
    PESSOA nova_pessoa;
    limpar_buffer_entrada();

    char *nome;
    do
    {
        printf("Nome: ");
        nome = input();
    } while (!nome_valido(nome) || buscar_pessoa_nome(nome) != -1);
    strncpy(nova_pessoa.nome, nome, sizeof(nova_pessoa.nome) - 1);
    nova_pessoa.nome[sizeof(nova_pessoa.nome) - 1] = '\0'; // coloca o caractere '\0' no final do nome que foi cadastrado
    free(nome);

    do
    {
        printf("Data De Nascimento (formato dd/mm/YYYY): ");
        scanf("%s", nova_pessoa.data_de_nascimento);
    } while (!data_valida(nova_pessoa.data_de_nascimento));

    do
    {
        printf("CPF (digite apenas os números): ");
        scanf("%s", nova_pessoa.cpf);
    } while (!cpf_valido(nova_pessoa.cpf));

    do
    {
        printf("E-mail: ");
        scanf("%s", nova_pessoa.email);
    } while (!email_valido(nova_pessoa.email));

    nova_pessoa.IdPessoa = gerar_IdPessoa();

    char confirmar;
    limpar_buffer_entrada();
    printf("Confirmar cadastro de %s?\n", nova_pessoa.nome);
    printf("[s] Sim\n");
    printf("[n] Não\n");
    printf("Escolha uma opção: ");
    scanf("%c", &confirmar);

    if (confirmar == 's' || confirmar == 'S')
    {
        if (salvar_pessoa(nova_pessoa))
            printf("\n%s salvo com sucesso!\n", nova_pessoa.nome);
        else
        {
            printf("\nErro ao salvar %s!\n", nova_pessoa.nome);
            return;
        }
    }
    else
    {
        printf("\nCancelado!\n");
        return;
    }

    //-----------------cadastro do telefone------------------
    int index_nova_pessoa = buscar_pessoa_IdPessoa(nova_pessoa.IdPessoa);
    if (index_nova_pessoa == -1)
    {
        printf("Erro ao cadastrar telefone para o novo contato\n");
        return;
    }

    cadastrar_telefone(0, index_nova_pessoa);
}

bool salvar_pessoa(PESSOA pessoa)
{
    // Redimensionar a lista de pessoas
    LISTA_PESSOAS = (PESSOA *)realloc(LISTA_PESSOAS, (QTD_PESSOAS + 1) * sizeof(PESSOA));
    if (LISTA_PESSOAS == NULL)
    {
        printf("Erro ao realocar memória para LISTA_PESSOAS\n");
        return false;
    }
    LISTA_PESSOAS[QTD_PESSOAS] = pessoa;
    QTD_PESSOAS++;

    reescrever_pessoas();
    return true;
}

void cadastrar_telefone(int metodo_busca, int index_pessoa_existente)
{
    int index_pessoa;
    if (index_pessoa_existente == -1)
    {
        index_pessoa = buscar_pessoa(metodo_busca);
        if (index_pessoa == -1)
        {
            printf("Contato não encontrado\n");
            return;
        }
    }
    else
        index_pessoa = index_pessoa_existente;

    printf("\nCadastrar Telefone: \n");

    TELEFONE novo_telefone;
    printf("Cadastrando telefone para %s\n", LISTA_PESSOAS[index_pessoa].nome);
    char continuar_cadastro;
    do
    {
        printf("Insiria o número de telefone (apenas números e DDD incluso): ");
        scanf("%s", novo_telefone.telefone);

        if (!telefone_valido(novo_telefone.telefone))
            continue;

        novo_telefone.IdPessoa = LISTA_PESSOAS[index_pessoa].IdPessoa;
        novo_telefone.IdTelefone = gerar_IdTelefone();

        if (salvar_telefone(novo_telefone))
            printf("\nTelefone cadastrado com sucesso!\n");
        else
        {
            printf("\nErro ao salvar telefones\n");
            return;
        }

        // Limpar o buffer antes de ler continuar_cadastro
        limpar_buffer_entrada();

        printf("Deseja continuar o cadastro de telefones?\n");
        printf("[s] Sim\n");
        printf("[n] Não\n");
        printf("Escolha uma opção: ");
        scanf("%c", &continuar_cadastro);

    } while (continuar_cadastro == 's' || continuar_cadastro == 'S');
}

bool salvar_telefone(TELEFONE telefone)
{
    // redimensionar a lista de telefones
    LISTA_TELEFONES = (TELEFONE *)realloc(LISTA_TELEFONES, (QTD_TELEFONES + 1) * sizeof(TELEFONE));
    if (LISTA_TELEFONES == NULL)
    {
        printf("Erro ao realocar memória para LISTA_TELEFONES\n");
        return false;
    }
    LISTA_TELEFONES[QTD_TELEFONES] = telefone;
    QTD_TELEFONES++;
    reescrever_telefones();

    return true;
}

void editar_pessoa(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }

    int campo;
    do
    {
        menu_editar_pessoa_campos();
        scanf("%d", &campo);
        switch (campo)
        {
        case 1:
        {
            char *novo_nome;
            limpar_buffer_entrada();
            do
            {
                printf("Digite o novo nome para %s: ", LISTA_PESSOAS[index_pessoa].nome);
                novo_nome = input();
                if (!nome_valido(novo_nome))
                    printf("Nome inválido\n");
            } while (!nome_valido(novo_nome));

            strncpy(LISTA_PESSOAS[index_pessoa].nome, novo_nome, sizeof(LISTA_PESSOAS[index_pessoa].nome));
            LISTA_PESSOAS[index_pessoa].nome[sizeof(LISTA_PESSOAS[index_pessoa].nome) - 1] = '\0'; // coloca o caractere '\0' no final do nome que foi cadastrado
            reescrever_pessoas();
            printf("Nome alterado!\n");
            campo = 0;
            break;
        }
        case 2:
        {
            char nova_data_de_nascimento[11];
            do
            {
                printf("Informe a nova data de nascimento (dd/mm/YYYY): ");
                scanf("%s", nova_data_de_nascimento);
                if (!data_valida(nova_data_de_nascimento))
                    printf("Data inválida\n");
            } while (!data_valida(nova_data_de_nascimento));

            strcpy(LISTA_PESSOAS[index_pessoa].data_de_nascimento, nova_data_de_nascimento);
            reescrever_pessoas();
            printf("Data de Nascimento alterada!\n");
            campo = 0;
            break;
        }
        case 3:
        {
            char novo_cpf[12];
            do
            {
                printf("Informe o novo CPF: ");
                scanf("%s", novo_cpf);
                if (!cpf_valido(novo_cpf))
                    printf("CPF inválido\n");
            } while (!cpf_valido(novo_cpf));

            strcpy(LISTA_PESSOAS[index_pessoa].cpf, novo_cpf);
            reescrever_pessoas();
            printf("CPF alterado\n");
            campo = 0;
            break;
        }
        case 4:
        {
            char novo_email[100];
            do
            {
                printf("Informe o novo email: ");
                scanf("%s", novo_email);
                if (!email_valido(novo_email))
                    printf("E-mail inválido\n");
            } while (!email_valido(novo_email));

            strcpy(LISTA_PESSOAS[index_pessoa].email, novo_email);
            reescrever_pessoas();
            printf("E-mail alterado!\n");
            campo = 0;
            break;
        }
        case 5:
            break;
        default:
            printf("Campo inválido!\n");
            break;
        }
    } while (campo != 5);
}

void editar_telefones(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }

    printf("Telefones de %s:\n", LISTA_PESSOAS[index_pessoa].nome);
    for (int i = 0; i < QTD_TELEFONES; i++)
    {
        if (LISTA_PESSOAS[index_pessoa].IdPessoa == LISTA_TELEFONES[i].IdPessoa)
            printf("%s --> ID: %d\n", LISTA_TELEFONES[i].telefone, i);
    }

    int index_telefone;
    char novo_telefone[14];
    char continuar_edicao;
    do
    {
        do
        {
            printf("Digite o ID do telefone que você quer editar: ");
            scanf("%d", &index_telefone);
            if (LISTA_TELEFONES[index_telefone].IdPessoa != LISTA_PESSOAS[index_pessoa].IdPessoa)
                printf("ID do telefone inválido. Tente novamente.\n");
            else
                break;
        } while (1);

        do
        {
            printf("Informe o novo número de telefone (apenas números e DDD incluso): ");
            scanf("%s", novo_telefone);
        } while (!telefone_valido(novo_telefone));

        strcpy(LISTA_TELEFONES[index_telefone].telefone, novo_telefone);
        printf("Telefone alterado com sucesso!\n");
        reescrever_telefones();

        // Limpar o buffer antes de ler continuar_cadastro
        limpar_buffer_entrada();

        printf("Deseja editar outro telefone?\n");
        printf("[s] Sim\n");
        printf("[n] Não\n");
        printf("Escolha uma opção: ");
        scanf("%c", &continuar_edicao);
    } while (continuar_edicao == 's' || continuar_edicao == 'S');

    printf("Telefones editados com sucesso!\n");
}

void excluir_pessoa(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }

    char confirmar;
    printf("Deseja excluir %s?\n", LISTA_PESSOAS[index_pessoa].nome);
    printf("[s] Sim\n");
    printf("[n] Não\n");
    printf("Escolha uma opção: ");
    if (metodo_busca == 1)
        scanf("%c", &confirmar);
    else
    {
        limpar_buffer_entrada();
        scanf("%c", &confirmar);
    }

    if (confirmar == 's' || confirmar == 'S')
    {
        // remover telefones
        for (int i = 0; i < QTD_TELEFONES; i++)
        {
            if (LISTA_TELEFONES[i].IdPessoa == LISTA_PESSOAS[index_pessoa].IdPessoa)
            {
                for (int j = i; j < QTD_TELEFONES - 1; j++)
                {
                    LISTA_TELEFONES[j] = LISTA_TELEFONES[j + 1];
                }
                QTD_TELEFONES--;
                i--;
            }
        }

        // remover pessoa
        for (int i = index_pessoa; i < QTD_PESSOAS - 1; i++)
        {
            LISTA_PESSOAS[i] = LISTA_PESSOAS[i + 1];
        }
        QTD_PESSOAS--;

        reescrever_pessoas();
        reescrever_telefones();
        printf("Contato excluído com sucesso!\n");
    }
    else
        printf("Exclusão cancelada!\n");
}

void excluir_telefone(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }

    printf("Telefones de %s:\n", LISTA_PESSOAS[index_pessoa].nome);
    for (int i = 0; i < QTD_TELEFONES; i++)
    {
        if (LISTA_PESSOAS[index_pessoa].IdPessoa == LISTA_TELEFONES[i].IdPessoa)
            printf("%s --> ID: %d\n", LISTA_TELEFONES[i].telefone, i);
    }

    int index_telefone;
    do
    {
        printf("Digite o ID do telefone que você quer excluir: ");
        scanf("%d", &index_telefone);
        if (LISTA_TELEFONES[index_telefone].IdPessoa != LISTA_PESSOAS[index_pessoa].IdPessoa)
            printf("ID do telefone inválido. Tente novamente.\n");
        else
            break;
    } while (1);

    char confirmar;
    limpar_buffer_entrada();
    printf("Deseja excluir o número %s de %s?\n", LISTA_TELEFONES[index_telefone].telefone, LISTA_PESSOAS[index_pessoa].nome);
    printf("[s] Sim\n");
    printf("[n] Não\n");
    printf("Escolha uma opção: ");
    scanf("%c", &confirmar);

    if (confirmar == 's' || confirmar == 'S')
    {
        for (int i = index_telefone; i < QTD_TELEFONES - 1; i++)
        {
            LISTA_TELEFONES[i] = LISTA_TELEFONES[i + 1];
        }
        QTD_TELEFONES--;

        reescrever_telefones();
        printf("Telefone excluído com sucesso!\n");
    }
    else
        printf("Exclusão cancelada!\n");
}

void consultar_pessoa(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }
    printf("\n");
    printf("============================================\n");
    printf(" Informações de %s\n", LISTA_PESSOAS[index_pessoa].nome);
    printf("============================================\n");
    printf(" - ID:                 %d\n", LISTA_PESSOAS[index_pessoa].IdPessoa);
    printf(" - Nome:               %s\n", LISTA_PESSOAS[index_pessoa].nome);
    printf(" - Data de Nascimento: %s\n", LISTA_PESSOAS[index_pessoa].data_de_nascimento);
    printf(" - CPF:                %s\n", LISTA_PESSOAS[index_pessoa].cpf);
    printf(" - E-mail:             %s\n", LISTA_PESSOAS[index_pessoa].email);
    printf("============================================\n");
}

void consultar_telefones(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }
    printf("\n");
    printf("============================================\n");
    printf("          Telefones de %s\n", LISTA_PESSOAS[index_pessoa].nome);
    printf("============================================\n");
    int contador_telefones = 0;
    for (int i = 0; i < QTD_TELEFONES; i++)
    {
        if (LISTA_PESSOAS[index_pessoa].IdPessoa == LISTA_TELEFONES[i].IdPessoa)
        {
            printf(" - Telefone %d: %s\n", ++contador_telefones, LISTA_TELEFONES[i].telefone);
        }
    }
    if (contador_telefones == 0)
        printf(" Nenhum telefone cadastrado.\n");
    printf("============================================\n");
}