#include <stdio.h>

void menu_principal()
{
    printf("\n");
    printf("============== Menu Principal ==============\n");
    printf("1. Cadastrar\n");
    printf("2. Editar\n");
    printf("3. Excluir\n");
    printf("4. Consultar\n");
    printf("5. Sair\n");
    printf("============================================\n");
    printf("Selecionar: ");
}

void menu_cadastrar()
{
    printf("\n");
    printf("============== Menu Cadastrar ==============\n");
    printf("1. Cadastrar Pessoa\n");
    printf("2. Cadastrar Telefone\n");
    printf("3. Retornar\n");
    printf("============================================\n");
    printf("Selecionar: ");
}

void menu_cadastrar_telefone()
{
    printf("\n");
    printf("-----------Cadastrar Telefones-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_editar()
{
    printf("\n");
    printf("============== Menu Editar ==============\n");
    printf("1. Editar Pessoa\n");
    printf("2. Editar Telefone\n");
    printf("3. Retornar\n");
    printf("============================================\n");
    printf("Selecionar: ");
}

void menu_editar_pessoa()
{
    printf("\n");
    printf("-----------Editar Pessoa-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_editar_telefone()
{
    printf("\n");
    printf("-----------Editar Telefone-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_editar_pessoa_campos()
{
    printf("\n");
    printf("-----------Campos Pessoa-------------\n");
    printf("1 - Nome\n");
    printf("2 - Data de Nascimento\n");
    printf("3 - CPF\n");
    printf("4 - E-mail\n");
    printf("5 - Retornar\n\n");
    printf("Selecionar campo para editar: ");
}

void menu_excluir()
{
    printf("\n");
    printf("============== Menu Excluir ==============\n");
    printf("1. Excluir Pessoa\n");
    printf("2. Excluir Telefone\n");
    printf("3. Retornar\n");
    printf("============================================\n");
    printf("Selecionar: ");
}

void menu_excluir_pessoa()
{
    printf("\n");
    printf("-----------Excluir Pessoa-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_excluir_telefone()
{
    printf("\n");
    printf("-----------Excluir Telefone-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_consultar()
{
    printf("\n");
    printf("============== Menu Consultar ==============\n");
    printf("1. Consultar Pessoa\n");
    printf("2. Consultar Telefone\n");
    printf("3. Retornar\n");
    printf("============================================\n");
    printf("Selecionar: ");
}

void menu_consultar_pessoa()
{
    printf("\n");
    printf("-----------Consultar Pessoa-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_consultar_telefone()
{
    printf("\n");
    printf("-----------Consultar Telefone-------------\n");
    printf("1 - Pesquisar por Nome\n");
    printf("2 - Pesquisar por ID\n");
    printf("3 - Retornar\n\n");
    printf("Selecionar: ");
}

void menu_invalido()
{
    printf("\n");
    printf("============================================\n");
    printf("!!! Opção inválida. Por favor, escolha uma opção válida do menu. !!!\n");
    printf("============================================\n");
}