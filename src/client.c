/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : src/client.c                                                     |
 | Autors     : Xavi, Jadi, Ivette                                               |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

// Inclusión de librerías estándar
#include <stdio.h>   // Para operaciones de entrada/salida
#include <stdlib.h>  // Para funciones estándar como malloc, exit, etc.
#include <string.h>  // Para manipulación de cadenas de texto
#include <stdbool.h> // Para el uso de variables booleanas

// Inclusión de librerías de red y sockets
#include <unistd.h>     // Para funciones como close(), read(), write()
#include <sys/socket.h> // Para funciones de sockets
#include <arpa/inet.h>  // Para funciones de red como inet_addr()

// Inclusión de librerías personalizadas
#include "funcions_client.h" // Funciones específicas definidas para el cliente


// Definición de constantes
#define MIDA_PAQUET 1024 // Tamaño del paquete para la comunicación


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
        mostra_error_inici_sessio(nom) ? registra_nou_usuari(s, &contacte_servidor, contacte_servidor_mida, nom) : (void)0;
    }
    else
    {
        printf("Error en inici de sessió. Contrasenya incorrecta.\n");
        registra_activitat("Error inici de sessió", nom);
    }

    finalitza_sessio(s, nom);
    return 0;
}