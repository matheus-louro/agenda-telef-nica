# Variáveis
CC = gcc
CFLAGS = -pedantic -std=c11
LDFLAGS = -lm
TARGET = main

# Diretórios
SRC_DIR = .
MENUS_DIR = menus
FUNCOES_DIR = funcoes_da_agenda
VALIDACOES_DIR = validacoes

# Fontes
SRCS = $(SRC_DIR)/main.c \
       $(MENUS_DIR)/menu_templates.c \
       $(FUNCOES_DIR)/funcoes.c \
       $(VALIDACOES_DIR)/validacoes.c

# Regras
all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
