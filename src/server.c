
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
        return -1;
    }
    printf("Servidor UDP configurat correctament al port %d.\n", atoi(argv[1]));

    while (1) {
        int bytes_rebuts = recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
        if (bytes_rebuts < 0) {
            perror("Error rebent el paquet");
            continue;
        }

        paquet[bytes_rebuts] = '\0';
        printf("Paquet rebut de %s:%d - %s\n", inet_ntoa(contacte_client.sin_addr), ntohs(contacte_client.sin_port), paquet);

        processa_peticio(s, contacte_client, contacte_client_mida, paquet);
    }

    close(s);
    return 0;
}
