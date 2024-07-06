#ifndef VALIDACOES
#define VALIDACOES

#include <stdbool.h>

bool nome_valido(const char *nome);
bool telefone_valido(const char *telefone);
bool email_valido(const char *email);
bool cpf_valido(const char *cpf);
bool data_valida(const char *data);

#endif