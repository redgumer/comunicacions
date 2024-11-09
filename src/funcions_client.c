
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "funcions_client.h"

#define MIDA_PAQUET 1024

void mostra_menu() {
    printf("\n--- MENÚ D'OPCIONS ---\n");
    printf("1. Veure perfil\n");
    printf("2. Veure amics\n");
    printf("3. Afegir nous amics\n");
    printf("4. Tancar sessió\n");
    printf("Selecciona una opció: ");
}

int envia_paquet(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, const char *paquet) {
    if (sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)contacte_servidor, contacte_servidor_mida) < 0) {
        perror("Error enviant paquet");
        return -1;
    }
    return 0;
}

int rep_paquet(int s, char *paquet, struct sockaddr_in *contacte_servidor, socklen_t *contacte_servidor_mida) {
    int bytes_rebuts = recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)contacte_servidor, contacte_servidor_mida);
    if (bytes_rebuts < 0) {
        perror("Error rebent el paquet");
        return -1;
    }
    paquet[bytes_rebuts] = '\0';
    return 0;
}

int inicia_sessio(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom) {
    char paquet[MIDA_PAQUET], contrasenya[50];
    printf("Introdueix el teu nom d'usuari: ");
    fgets(nom, 50, stdin);
    nom[strcspn(nom, "\n")] = '\0';

    printf("Introdueix la teva contrasenya: ");
    fgets(contrasenya, 50, stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    snprintf(paquet, sizeof(paquet), "1 %s %s", nom, contrasenya);
    if (envia_paquet(s, contacte_servidor, contacte_servidor_mida, paquet) < 0) {
        return -1;
    }

    if (rep_paquet(s, paquet, contacte_servidor, &contacte_servidor_mida) < 0) {
        return -1;
    }

    return atoi(paquet);
}

int registra_usuari(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom, const char *contrasenya, char *sexe, char *estat_civil, int edat, char *ciutat, char *descripcio) {
    char paquet[MIDA_PAQUET];
    snprintf(paquet, sizeof(paquet), "2 %s %s %s %s %d %s \"%s\"", nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);

    if (envia_paquet(s, contacte_servidor, contacte_servidor_mida, paquet) < 0) {
        return -1;
    }

    if (rep_paquet(s, paquet, contacte_servidor, &contacte_servidor_mida) < 0) {
        return -1;
    }

    printf("Resposta del servidor: %s\n", paquet);
    return 0;
}
