
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "funcions_servidor.h"

#define MIDA_PAQUET 1024
#define MAX_USUARIS 10

Usuari usuaris[MAX_USUARIS];
int num_usuaris = 0;

int verifica_usuari(const char *nom, const char *contrasenya) {
    for (int i = 0; i < num_usuaris; i++) {
        if (strcmp(usuaris[i].nom, nom) == 0) {
            return strcmp(usuaris[i].contrasenya, contrasenya) == 0 ? 1 : -2;
        }
    }
    return 0;
}

int registra_usuari(const char *nom, const char *contrasenya) {
    if (num_usuaris < MAX_USUARIS) {
        strncpy(usuaris[num_usuaris].nom, nom, sizeof(usuaris[num_usuaris].nom) - 1);
        strncpy(usuaris[num_usuaris].contrasenya, contrasenya, sizeof(usuaris[num_usuaris].contrasenya) - 1);
        num_usuaris++;
        printf("Nou usuari registrat: %s\n", nom);
        return 1;
    } else {
        printf("Error: Llista d'usuaris plena.\n");
        return -1;
    }
}

void processa_opcio_menu(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, int opcio, const char *nom) {
    char resposta[MIDA_PAQUET];
    switch (opcio) {
        case 1:
            snprintf(resposta, sizeof(resposta), "Perfil de l'usuari: %s", nom);
            break;
        case 2:
            snprintf(resposta, sizeof(resposta), "Amics de l'usuari %s: No implementat encara.", nom);
            break;
        case 3:
            snprintf(resposta, sizeof(resposta), "Afegir nous amics: Funció no implementada encara.");
            break;
        case 4:
            snprintf(resposta, sizeof(resposta), "Sessió tancada per l'usuari %s.", nom);
            break;
        default:
            snprintf(resposta, sizeof(resposta), "Opció no vàlida.");
    }
    sendto(s, resposta, strlen(resposta), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
}

void processa_peticio(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, char *paquet) {
    int codi_operacio, opcio;
    char nom[50], contrasenya[50];

    if (sscanf(paquet, "%d", &codi_operacio) != 1) {
        printf("Error de format en el paquet rebut.\n");
        return;
    }

    switch (codi_operacio) {
        case 1:
            if (sscanf(paquet, "%d %s %s", &codi_operacio, nom, contrasenya) == 3) {
                int codi_resposta = verifica_usuari(nom, contrasenya);
                snprintf(paquet, MIDA_PAQUET, "%d", codi_resposta);
                sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
            } else {
                printf("Error de format en inici de sessió.\n");
            }
            break;
        case 2:
            if (sscanf(paquet, "%d %s %s", &codi_operacio, nom, contrasenya) == 3) {
                int codi_resposta = registra_usuari(nom, contrasenya);
                snprintf(paquet, MIDA_PAQUET, "Codi resposta: %d", codi_resposta);
                sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
            } else {
                printf("Error de format en registre d'usuari.\n");
            }
            break;
        case 3:
            if (sscanf(paquet, "%d %d %s", &codi_operacio, &opcio, nom) == 3) {
                processa_opcio_menu(s, contacte_client, contacte_client_mida, opcio, nom);
            } else {
                printf("Error de format en opcions del menú.\n");
            }
            break;
        default:
            printf("Operació desconeguda: %d\n", codi_operacio);
    }
}
