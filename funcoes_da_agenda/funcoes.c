#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../validacoes/validacoes.h"
#include "../menus/menu_templates.h"
// structs
typedef struct p
{
    int IdPessoa;
    char nome[31];
    char data_de_nascimento[11];
    char cpf[12];
    char email[100];
} PESSOA;

typedef struct t
{
    int IdTelefone;
    int IdPessoa;
    char telefone[14];
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
int buscar_pessoa_nome(char nome[31]);
int buscar_pessoa_IdPessoa(int id);
int gerar_IdPessoa();
int gerar_IdTelefone();
bool salvar_pessoa(PESSOA pessoa);
void cadastrar_telefone(int metodo_busca, int index_pessoa_existente);
bool salvar_telefones(TELEFONE telefones[], int qtd_numeros_novos);

// funções
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
    fseek(arquivo, 0, SEEK_SET);
    while ((caracter = fgetc(arquivo)) != EOF)
    {
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

bool carregar_pessoas()
{
    FILE *arquivo_pessoas = fopen("pessoas.txt", "r");
    if (arquivo_pessoas == NULL)
    {
        printf("Erro ao abrir pessoas.txt\n");
        limpar_memoria();
        return false;
    }

    for (int i = 0; i < QTD_PESSOAS; i++)
    {
        int itens_lidos = fscanf(arquivo_pessoas, "%d;%29[^;];%10[^;];%11[^;];%99[^\n]\n",
                                 &LISTA_PESSOAS[i].IdPessoa,
                                 LISTA_PESSOAS[i].nome,
                                 LISTA_PESSOAS[i].data_de_nascimento,
                                 LISTA_PESSOAS[i].cpf,
                                 LISTA_PESSOAS[i].email);
        if (itens_lidos != 5)
        {
            if (i != 0) // Caso o arquivo esteja vazio, a lista iniciará vazia, caso contrário, irá emitir esse erro
            {
                printf("Erro ao ler linha %d do arquivo pessoas.txt\n", i + 1);
                limpar_memoria();
                fclose(arquivo_pessoas);
                return false;
            }
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

    for (int i = 0; i < QTD_TELEFONES; i++)
    {
        fscanf(arquivo_telefones, "%d;%d;%13[^\n]\n",
               &LISTA_TELEFONES[i].IdTelefone,
               &LISTA_TELEFONES[i].IdPessoa,
               LISTA_TELEFONES[i].telefone);
    }
    fclose(arquivo_telefones);

    return true;
}

bool carregar_arquivos_na_memoria()
{
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

int buscar_pessoa_nome(char nome[31])
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
        char nome[31];
        printf("Digite o nome do contato: ");
        limpar_buffer_entrada();
        fgets(nome, sizeof(nome), stdin);
        // remove caractere da nova linha, se presente
        size_t len = strlen(nome);
        if (len > 0 && nome[len - 1] == '\n')
            nome[len - 1] = '\0';

        return buscar_pessoa_nome(nome);
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
    do
    {
        printf("Nome: ");
        limpar_buffer_entrada();
        fgets(nova_pessoa.nome, sizeof(nova_pessoa.nome), stdin);
        // Remove o caractere de nova linha, se presente
        size_t len = strlen(nova_pessoa.nome);
        if (len > 0 && nova_pessoa.nome[len - 1] == '\n')
            nova_pessoa.nome[len - 1] = '\0';
    } while (!nome_valido(nova_pessoa.nome) || buscar_pessoa_nome(nova_pessoa.nome) != -1);

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

    int confirmar = 0;
    printf("Confirmar cadastro de %s (1 - Confirmar / 2 - Cancelar): ", nova_pessoa.nome);
    scanf("%d", &confirmar);

    if (confirmar == 1)
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
        printf("\ncancelado!\n");
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

    char *nome_contato = LISTA_PESSOAS[index_pessoa].nome;
    int id_pessoa = LISTA_PESSOAS[index_pessoa].IdPessoa;

    printf("\nCadastrar Telefone: \n");
    int qtd_novos_telefones = 0;
    printf("insira quantos telefones você irá cadastrar para %s: ", nome_contato);
    scanf("%d", &qtd_novos_telefones);

    TELEFONE novos_telefones[qtd_novos_telefones];

    int numeros_cadastrados = 0;
    char continuar_cadastro;
    do
    {
        printf("Insiria o número de telefone (apenas números e DDD incluso): ");
        scanf("%s", novos_telefones[numeros_cadastrados].telefone);

        if (!telefone_valido(novos_telefones[numeros_cadastrados].telefone))
            continue;

        novos_telefones[numeros_cadastrados].IdPessoa = id_pessoa;
        novos_telefones[numeros_cadastrados].IdTelefone = gerar_IdTelefone();
        numeros_cadastrados++;

        if (salvar_telefones(novos_telefones, numeros_cadastrados))
            printf("\nTelefone cadastrado com sucesso!\n");
        else
        {
            printf("\nErro ao salvar telefones\n");
            return;
        }

        if (numeros_cadastrados >= qtd_novos_telefones)
            break;
        // Limpar o buffer antes de ler continuar_cadastro
        limpar_buffer_entrada();

        printf("Deseja continuar o cadastro de telefones?\ns - SIM\nn - NÃO\n: ");
        scanf("%c", &continuar_cadastro);

    } while (continuar_cadastro == 's');
}

bool salvar_telefones(TELEFONE telefones[], int qtd_numeros_novos)
{
    // redimensionar a lista de telefones
    LISTA_TELEFONES = (TELEFONE *)realloc(LISTA_TELEFONES, (QTD_TELEFONES + qtd_numeros_novos) * sizeof(TELEFONE));
    if (LISTA_TELEFONES == NULL)
    {
        printf("Erro ao realocar memória para LISTA_TELEFONES\n");
        return false;
    }
    for (int i = 0; i < qtd_numeros_novos; i++)
    {
        LISTA_TELEFONES[QTD_TELEFONES + i] = telefones[i];
    }
    QTD_TELEFONES += qtd_numeros_novos;

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
            char novo_nome[31];
            do
            {
                printf("Digite o novo nome para %s: ", LISTA_PESSOAS[index_pessoa].nome);
                limpar_buffer_entrada();
                fgets(novo_nome, sizeof(novo_nome), stdin);
                // remove caractere da nova linha, se presente
                size_t len = strlen(novo_nome);
                if (len > 0 && novo_nome[len - 1] == '\n')
                    novo_nome[len - 1] = '\0';
                if (!nome_valido(novo_nome))
                    printf("Nome inválido\n");
            } while (!nome_valido(novo_nome));

            strcpy(LISTA_PESSOAS[index_pessoa].nome, novo_nome);
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

        limpar_buffer_entrada();
        printf("Deseja editar outro telefone de %s?\ns - SIM\nn - NÃO\ncontinuar? : ", LISTA_PESSOAS[index_pessoa].nome);
        scanf("%c", &continuar_edicao);
    } while (continuar_edicao == 's');
}

void excluir_pessoa(int metodo_busca)
{
    int index_pessoa = buscar_pessoa(metodo_busca);
    if (index_pessoa == -1)
    {
        printf("Contato não encontrado\n");
        return;
    }

    int confirmar;
    printf("Deseja excluir %s?\n1 - SIM\n2 - NÃO\nexcluir? : ", LISTA_PESSOAS[index_pessoa].nome);
    scanf("%d", &confirmar);

    if (confirmar == 1)
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

    int confirmar;
    printf("Deseja excluir o número %s de %s?\n1 - SIM\n2 - NÃO\nexcluir? : ", LISTA_TELEFONES[index_telefone].telefone, LISTA_PESSOAS[index_pessoa].nome);
    scanf("%d", &confirmar);

    for (int i = index_telefone; i < QTD_TELEFONES - 1; i++)
    {
        LISTA_TELEFONES[i] = LISTA_TELEFONES[i + 1];
    }
    QTD_TELEFONES--;

    reescrever_telefones();
    printf("Telefone excluído com sucesso!\n");
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
    printf("           Informações de %s\n", LISTA_PESSOAS[index_pessoa].nome);
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
    {
        printf(" Nenhum telefone cadastrado.\n");
    }
    printf("============================================\n");
}