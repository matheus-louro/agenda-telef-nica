#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>

typedef struct p
{
    int IdPessoa;
    char nome[30];
    char email[100];
    char cpf[12];
    char data_de_nascimento[11];
} PESSOA;

typedef struct t
{
    int IdTelefone;
    int IdPessoa;
    char telefone[14];
} TELEFONE;

extern PESSOA *LISTA_PESSOAS;
extern TELEFONE *LISTA_TELEFONES;
extern int QTD_PESSOAS;
extern int QTD_TELEFONES;

bool carregar_arquivos_na_memoria();
void limpar_memoria();
void cadastrar_pessoa();
void cadastrar_telefone(int metodo_busca, int index_pessoa_existente);
void editar_pessoa(int metodo_busca);
void editar_telefones(int metodo_busca);
void excluir_pessoa(int metodo_busca);
void excluir_telefone(int metodo_busca);
void consultar_pessoa(int metodo_busca);
void consultar_telefones(int metodo_busca);

#endif
