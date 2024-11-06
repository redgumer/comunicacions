/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : server.c                                                         |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../include/funcions_servidor.h"


int main(int argc, char **argv) {
    if (argc == 2) {
        struct sockaddr_in socket_servidor, contacte_client;

        // Inicializar el servidor
        int s = inicializar_servidor(atoi(argv[1]), &socket_servidor);

        while (1) {
            // Manejar la conexión
            manejar_conexion(s, &contacte_client);
        }
        close(s);
    } else {
        printf("Uso: %s <puerto>\n", argv[0]);
    }
    return 0;
}