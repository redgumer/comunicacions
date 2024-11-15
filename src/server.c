#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "funcions_servidor.h"

#define MIDA_PAQUET 1024

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Ús: %s <PORT>\n", argv[0]);
        return -1;
    }

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serv_addr, contacte_client;
    socklen_t contacte_client_mida = sizeof(contacte_client);
    char paquet[MIDA_PAQUET];

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error en el bind");
        registra_activitat("ERROR", "Error en el bind del servidor.");
        return -1;
    }
    printf("Servidor UDP configurat correctament al port %d.\n", atoi(argv[1]));
    registra_activitat("INFO", "Servidor UDP configurat correctament.");

    while (1) {
        int bytes_rebuts = recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
        if (bytes_rebuts < 0) {
            perror("Error rebent el paquet");
            registra_activitat("ERROR", "Error rebent el paquet del client.");
            continue;
        }

        paquet[bytes_rebuts] = '\0';

        // Registra el paquet rebut
        char missatge[150];
        snprintf(missatge, sizeof(missatge), "Paquet rebut de %s:%d - %.100s",
                 inet_ntoa(contacte_client.sin_addr), ntohs(contacte_client.sin_port), paquet);
        registra_activitat("CLIENT", missatge);

        // Processa la petició
        processa_peticio(s, contacte_client, contacte_client_mida, paquet);

        // Registra que s'ha processat la petició
        snprintf(missatge, sizeof(missatge), "Peticio processada per a %s:%d",
                 inet_ntoa(contacte_client.sin_addr), ntohs(contacte_client.sin_port));
        registra_activitat("SERVER", missatge);
    }

    close(s);
    registra_activitat("INFO", "Servidor tancat correctament.");
    return 0;
}
