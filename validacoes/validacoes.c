#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

bool nome_valido(const char *nome)
{
    return strlen(nome) > 2;
}

bool telefone_valido(const char *telefone)
{
    int tel_tamanho = strlen(telefone);
    if (tel_tamanho != 10 && tel_tamanho != 11)
        return false;

    for (int i = 0; i < tel_tamanho; i++)
    {
        if (!isdigit(telefone[i]))
            return false;
    }

    return true;
}

bool email_valido(const char *email)
{
    int contador_arroba = 0;
    char caracteres_invalidos[] = " ()<>[]:;,/";

    for (int i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@')
            contador_arroba++;

        for (int j = 0; j < strlen(caracteres_invalidos); j++)
        {
            if (email[i] == caracteres_invalidos[j])
                return false;
        }
    }

    if (contador_arroba != 1 || email[0] == '@')
        return false;

    char *ponto = strchr(email, '.');
    if (ponto == NULL || ponto < strchr(email, '@'))
        return false;

    return true;
}

bool cpf_valido(const char *cpf)
{
    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(cpf[i]))
            return false;
    }

    int iguais = 1;
    for (int i = 0; i < 11; i++)
    {
        if (cpf[i] != cpf[0])
        {
            iguais = 0;
            break;
        }
    }

    if (iguais)
        return false;

    int soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (cpf[i] - 48) * (10 - i);
    }

    int digito_verificador_1 = 11 - (soma % 11);

    if (digito_verificador_1 >= 10)
        digito_verificador_1 = 0;

    if (digito_verificador_1 != cpf[9] - 48)
        return false;

    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (cpf[i] - 48) * (11 - i);
    }

    int digito_verificador_2 = 11 - (soma % 11);
    if (digito_verificador_2 >= 10)
        digito_verificador_2 = 0;

    if (digito_verificador_2 != cpf[10] - 48)
        return false;

    return true;
}

bool data_valida(const char *data)
{
    int dia, mes, ano;

    time_t t;
    time(&t);
    struct tm *data_atual = localtime(&t);
    int dia_atual = data_atual->tm_mday;
    int mes_atual = data_atual->tm_mon + 1;
    int ano_atual = data_atual->tm_year + 1900;

    if (sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano) != 3)
        return false;

    if (dia > 31 || dia < 1 || mes > 12 || mes < 1)
        return false;

    if (ano > ano_atual)
        return false;

    if (ano == ano_atual)
    {
        if (mes > mes_atual)
            return false;

        if (mes == mes_atual)
        {
            if (dia > dia_atual)
                return false;
        }
    }

    return true;
}