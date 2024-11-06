// ================================ LLIBRERIES ESTÀNDARD ================================ //
#include <stdio.h>          // Per funcions d'entrada/sortida com printf, scanf, etc.
#include <stdlib.h>         // Per funcions generals com malloc, free, etc.
#include <string.h>         // Per funcions de manipulació de cadenes, com strcpy, strcmp, etc.

// ================================ LLIBRERIES DE XARXA ================================ //
#include <sys/socket.h>     // Per funcions de sockets, com socket, connect, etc.
#include <sys/types.h>      // Per definicions de tipus de dades utilitzades en sockets
#include <netinet/in.h>     // Per funcions i estructures de la família de protocols d'Internet
#include <arpa/inet.h>      // Per la conversió d'adreces d'Internet (ex: inet_addr)
#include <unistd.h>         // Per funcions del sistema Unix, com close, read, write, etc.

// ================================ LLIBRERIES PROPIES ================================ //
#include "funcions_client.h" // Per funcions específiques definides per al client en el projecte


int main(int argc, char **argv)
{
    if (argc == 3)
    {
        int s;
        struct sockaddr_in contacte_servidor;

        // Inicialització de connexió amb el servidor
        s = inicialitza_connexio(&contacte_servidor, argv[1], argv[2]);

        // Verificació de l'usuari
        if (verifica_usuari(s, contacte_servidor) == 1)
        {
            // Mostra el menú d'opcions
            gestiona_menu(s, contacte_servidor);
        }
    }
    return 0;
}
