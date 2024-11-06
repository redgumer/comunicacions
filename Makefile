# Definició del compilador i opcions de compilació
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude 
# Carpetes del projecte
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
DATA_DIR = data

# Executables a generar
CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server

# Fitxers objecte
CLIENT_OBJS = $(BUILD_DIR)/client.o $(BUILD_DIR)/funcions_client.o
SERVER_OBJS = $(BUILD_DIR)/server.o $(BUILD_DIR)/funcions_servidor.o

# Definició de valors per defecte de les variables
PORT = 10000
IP = 127.0.0.0

# Objectiu per compilar-ho tot
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Compilar i enllaçar l'executable del client
$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $(CLIENT_OBJS) 

# Compilar i enllaçar l'executable del servidor
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $(SERVER_OBJS) 

# Compilar els fitxers objecte per al client
$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(INCLUDE_DIR)/funcions_client.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o

$(BUILD_DIR)/funcions_client.o: $(SRC_DIR)/funcions_client.c $(INCLUDE_DIR)/funcions_client.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/funcions_client.c -o $(BUILD_DIR)/funcions_client.o

# Compilar els fitxers objecte per al servidor
$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/funcions_servidor.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/funcions_servidor.o: $(SRC_DIR)/funcions_servidor.c $(INCLUDE_DIR)/funcions_servidor.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/funcions_servidor.c -o $(BUILD_DIR)/funcions_servidor.o

# Executar el servidor amb el port predefinit o personalitzat
servidor: $(SERVER_EXEC)
	./$(SERVER_EXEC) $(PORT)

# Executar el client amb IP i port predefinits o personalitzats
client: $(CLIENT_EXEC)
	./$(CLIENT_EXEC) $(IP) $(PORT)

# Netejar fitxers de compilació
clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/*
