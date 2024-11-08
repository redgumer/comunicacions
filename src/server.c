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
#include <sys/socket.h>
#include "../include/funcions_servidor.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("El nombre de paràmetres no és el correcte!\n");
        return -1;
    }
    int port = atoi(argv[1]);
    int s = configura_socket(port);
    if (s < 0)
        return -1;
    struct sockaddr_in contacte_client;
    while (1)
    {
        printf("Esperant connexió d'un client...\n");
        gestiona_client(s, contacte_client);
    }
        close(s);
    return 0;
}