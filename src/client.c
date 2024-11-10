
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "funcions_client.h"

#define MIDA_PAQUET 1024

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Ús: %s <IP_SERVIDOR> <PORT>\n", argv[0]);
        return -1;
    }

    int s;
    struct sockaddr_in contacte_servidor;
    socklen_t contacte_servidor_mida = sizeof(contacte_servidor);
    char nom[50];
    int sessio_iniciada;

    // Inicialitzar el socket
    s = inicialitza_socket(&contacte_servidor, argv[1], argv[2]);
    if (s < 0)
    {
        return -1;
    }

    // Iniciar sessió
    sessio_iniciada = inicia_sessio(s, &contacte_servidor, contacte_servidor_mida, nom);
    registra_activitat("Inici de sessió", nom);

    if (sessio_iniciada == 1)
    {
        printf("Inici de sessió correcte.\n");
        processar_opcio(s, &contacte_servidor, contacte_servidor_mida, nom);
    }
    else if (sessio_iniciada == 0)
    {
        mostra_error_inici_sessio(nom);
        registra_nou_usuari(s, &contacte_servidor, contacte_servidor_mida, nom);
    }
    else
    {
        printf("Error en inici de sessió. Contrasenya incorrecta.\n");
        registra_activitat("Error inici de sessió", nom);
    }

    finalitza_sessio(s, nom);
    return 0;
}