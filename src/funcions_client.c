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
#include <stdio.h>  // Per funcions d'entrada/sortida com printf, scanf, etc.
#include <stdlib.h> // Per funcions generals com malloc, free, etc.
#include <string.h> // Per funcions de manipulació de cadenes, com strcpy, strcmp, etc.

// ================================ LLIBRERIES DE XARXA ================================ //
#include <sys/socket.h> // Per funcions de sockets, com socket, connect, etc.
#include <sys/types.h>  // Per definicions de tipus de dades utilitzades en sockets
#include <netinet/in.h> // Per funcions i estructures de la família de protocols d'Internet
#include <arpa/inet.h>  // Per la conversió d'adreces d'Internet (ex: inet_addr)
#include <unistd.h>     // Per funcions del sistema Unix, com close, read, write, etc.

// ================================ LLIBRERIES PROPIES ================================ //
#include "../include/funcions_client.h" // Per funcions específiques definides per al client en el projecte

#define MIDA_PAQUET 1500
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
    printf("\n========================\n");
    printf("       Menu d'usuari:\n");
    printf("========================\n");
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

int verifica_usuari(int s, struct sockaddr_in contacte_servidor, char *nom)
{
    char contrasenya[MAX_CONTRASENYA];
    char paquet[MIDA_PAQUET];
    socklen_t contacte_servidor_mida = sizeof(contacte_servidor);

    // Demana nom d'usuari i contrasenya al client
    printf("Introdueix el teu nom d'usuari: ");
    scanf("%s", nom);
    printf("Nom d'usuari introduït: %s\n", nom);

    printf("Introdueix la teva contrasenya: ");
    scanf("%s", contrasenya);
    printf("Contrasenya introduïda: %s\n", contrasenya);

    // Empaqueta nom i contrasenya i els envia al servidor
    snprintf(paquet, sizeof(paquet), "%s %s", nom, contrasenya);
    printf("Paquet enviat al servidor: %s\n", paquet);

    // Enviar el paquet al servidor
    sendto(s, paquet, sizeof(paquet), 0, (struct sockaddr *)&contacte_servidor, contacte_servidor_mida);
    printf("Paquet enviat correctament al servidor.\n");

    // Rep la resposta del servidor
    recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, &contacte_servidor_mida);
    int codi_resposta;
    sscanf(paquet, "%d", &codi_resposta);
    printf("Codi de resposta rebut del servidor: %d\n", codi_resposta);

    // Retorna directament el codi de resposta del servidor
    return codi_resposta;
}

void gestiona_menu(int s, struct sockaddr_in contacte_servidor, char nom[MAX_USUARI])
{
    char paquet[MIDA_PAQUET];

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

void registre_usuari(char *nom, char *contrasenya, char *sexe, char *estat_civil, int *edat, char *ciutat, char *descripcio)
{
    printf("\n=====================================\n");
    printf("    Benvingut al registre d'Usuaris\n");
    printf("======================================\n");

    printf("Introdueix el teu nom: ");
    scanf("%s", nom);

    printf("Escriu la teva contrasenya: ");
    scanf("%s", contrasenya);

    printf("Escriu el teu sexe: ");
    scanf("%s", sexe);

    printf("Escriu el teu estat civil: ");
    scanf("%s", estat_civil);

    printf("Escriu la teva edat: ");
    scanf("%d", edat);

    printf("Escriu la teva ciutat: ");
    scanf("%s", ciutat);

    printf("Introdueix una breu descripció personal: ");
    scanf(" %[^\n]", descripcio);
}
