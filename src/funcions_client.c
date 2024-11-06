/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_client.c                                                |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

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
#include "../include/funcions_client.h" // Per funcions específiques definides per al client en el projecte

#define MIDA_PAQUET 100
#define MAX_USUARI 30
#define MAX_CONTRASENYA 30

void demana_credencials(char *nom, char *contrasenya)
{
    printf("Introdueix el teu nom: ");
    scanf("%s", nom);
    printf("Introdueix la teva contrasenya: ");
    scanf("%s", contrasenya);
}

int mostra_menu()
{
    int opcio;
    printf("\nMenu d'usuari:\n");
    printf("1. Veure perfil\n");
    printf("2. Veure els meus amics\n");
    printf("3. Afegir amics nous\n");
    printf("4. Tancar el programa\n");
    printf("Selecciona una opció: \n");
    scanf("%d", &opcio);
    return opcio;
}

#include "funcions_client.h"

int inicialitza_connexio(struct sockaddr_in *contacte_servidor, const char *ip, const char *port)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    contacte_servidor->sin_family = AF_INET;
    contacte_servidor->sin_addr.s_addr = inet_addr(ip);
    contacte_servidor->sin_port = htons(atoi(port));
    return s;
}

int verifica_usuari(int s, struct sockaddr_in contacte_servidor)
{
    char paquet[MIDA_PAQUET];
    char nom[MAX_USUARI], contrasenya[MAX_CONTRASENYA];

    demana_credencials(nom, contrasenya);

    sprintf(paquet, "%s %s", nom, contrasenya);
    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));

    recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
    printf("Resposta del servidor: %s\n", paquet);

    return strcmp(paquet, "Usuari verificat\n") == 0 ? 1 : 0;
}

void gestiona_menu(int s, struct sockaddr_in contacte_servidor)
{
    char paquet[MIDA_PAQUET];
    char nom[MAX_USUARI];
    int opcio;

    do
    {
        opcio = mostra_menu();
        sprintf(paquet, "%d %s", opcio, nom);
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));

        recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
        printf("Resposta del servidor: %s\n", paquet);

    } while (opcio != 4);
}
