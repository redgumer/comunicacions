# ================== CONFIGURACIÓN ==================
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -DCLIENT

# Carpetas del proyecto
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
DATA_DIR = data

# Archivos ejecutables
CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server

# Archivos fuente
SERVER_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/funcions_servidor.c $(SRC_DIR)/fun_afegir_amic.c $(SRC_DIR)/notificacions.c
CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/funcions_client.c $(SRC_DIR)/fun_afegir_amic.c $(SRC_DIR)/notificacions.c 

# Archivos objeto
SERVER_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SRC))
CLIENT_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CLIENT_SRC))

# Variables para IP y Puerto
PORT = 10000
IP = 127.0.0.1

# ================== REGLAS PRINCIPALES ==================

# Compilar todo
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Ejecutable del cliente
$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $(CLIENT_OBJS)

# Ejecutable del servidor
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $(SERVER_OBJS)

# ================== REGLAS DE COMPILACIÓN ==================

# Compilar archivos objeto del cliente
$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(INCLUDE_DIR)/funcions_client.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/funcions_client.o: $(SRC_DIR)/funcions_client.c $(INCLUDE_DIR)/funcions_client.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/fun_afegir_amic.o: $(SRC_DIR)/fun_afegir_amic.c $(INCLUDE_DIR)/fun_afegir_amic.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/notificacions.o: $(SRC_DIR)/notificacions.c $(INCLUDE_DIR)/notificacions.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar archivos objeto del servidor
$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/funcions_servidor.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/funcions_servidor.o: $(SRC_DIR)/funcions_servidor.c $(INCLUDE_DIR)/funcions_servidor.h
	$(CC) $(CFLAGS) -c $< -o $@

# ================== REGLAS DE EJECUCIÓN ==================

# Ejecutar el servidor
servidor: $(SERVER_EXEC)
	./$(SERVER_EXEC) $(PORT)

# Ejecutar el cliente
client: $(CLIENT_EXEC)
	./$(CLIENT_EXEC) $(IP) $(PORT)

# ================== LIMPIEZA ==================

# Limpiar archivos de compilación
clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/*

# ================== REGLA DE DEPENDENCIAS ==================

# Generar dependencias automáticamente
dependencias:
	$(CC) -MM $(CFLAGS) $(SRC_DIR)/*.c > .depend

-include .depend
