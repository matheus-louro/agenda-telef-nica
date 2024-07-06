#include <stdio.h>
#include <locale.h>
#include <signal.h>

#include "menus/menu_templates.h"
#include "funcoes_da_agenda/funcoes.h"

int main()
{
    setlocale(LC_ALL, "Portuguese");
    signal(SIGINT, limpar_memoria);
    int opcao = 0;

    if (!carregar_arquivos_na_memoria())
    {
        limpar_memoria();
        return 0;
    }

    do
    {
        menu_principal();
        scanf("%d", &opcao);

        switch (opcao)
        {
        // menu cadastrar
        case 1:
            while (opcao != 3)
            {
                menu_cadastrar();
                scanf("%d", &opcao);

                switch (opcao)
                {
                case 1:
                    cadastrar_pessoa();
                    break;
                case 2:
                    while (opcao != 3)
                    {
                        menu_cadastrar_telefone();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            cadastrar_telefone(opcao, -1);
                            break;
                        case 2:
                            cadastrar_telefone(opcao, -1);
                            break;
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;

                // retornar
                case 3:
                    break;
                default:
                    menu_invalido();
                    break;
                }
            }
            opcao = 0;
            continue;

        // Menu editar
        case 2:
            while (opcao != 3)
            {
                menu_editar();
                scanf("%d", &opcao);

                switch (opcao)
                {
                // Editar pessoa
                case 1:
                    while (opcao != 3)
                    {
                        menu_editar_pessoa();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            editar_pessoa(opcao);
                            break;
                        case 2:
                            editar_pessoa(opcao);
                            break;
                        // retornar
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;
                case 2:
                    while (opcao != 3)
                    {
                        menu_editar_telefone();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            editar_telefones(opcao);
                            break;
                        case 2:
                            editar_telefones(opcao);
                            break;
                        // retornar
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;
                // retornar
                case 3:
                    break;
                default:
                    menu_invalido();
                    break;
                }
            }
            opcao = 0;
            continue;
        case 3:
            do
            {
                menu_excluir();
                scanf("%d", &opcao);

                switch (opcao)
                {
                case 1:
                    while (opcao != 3)
                    {
                        menu_excluir_pessoa();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            excluir_pessoa(opcao);
                            break;
                        case 2:
                            excluir_pessoa(opcao);
                            break;
                        // retornar
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;
                case 2:
                    while (opcao != 3)
                    {
                        menu_excluir_telefone();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            excluir_telefone(opcao);
                            break;
                        case 2:
                            excluir_telefone(opcao);
                            break;
                        // retornar
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;
                case 3:
                    break;

                default:
                    menu_invalido();
                    break;
                }
            } while (opcao != 3);
            opcao = 0;
            continue;

        case 4:
            do
            {
                menu_consultar();
                scanf("%d", &opcao);

                switch (opcao)
                {
                case 1:
                    while (opcao != 3)
                    {
                        menu_consultar_pessoa();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            consultar_pessoa(opcao);
                            break;
                        case 2:
                            consultar_pessoa(opcao);
                            break;
                        // retornar
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;
                case 2:
                    while (opcao != 3)
                    {
                        menu_consultar_telefone();
                        scanf("%d", &opcao);

                        switch (opcao)
                        {
                        case 1:
                            consultar_telefones(opcao);
                            break;
                        case 2:
                            consultar_telefones(opcao);
                            break;
                        // retornar
                        case 3:
                            break;
                        default:
                            menu_invalido();
                            break;
                        }
                    }
                    opcao = 0;
                    continue;
                case 3:
                    break;

                default:
                    menu_invalido();
                    break;
                }
            } while (opcao != 3);
            opcao = 0;
            continue;
        case 5:
            break;

        default:
            menu_invalido();
            break;
        }

    } while (opcao != 5);

    limpar_memoria();

    return 0;
}