
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "funcions_client.h"

#define MIDA_PAQUET 1024

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Ús: %s <IP_SERVIDOR> <PORT>\n", argv[0]);
        return -1;
    }

    int s;
    struct sockaddr_in contacte_servidor;
    socklen_t contacte_servidor_mida = sizeof(contacte_servidor);
    char paquet[MIDA_PAQUET], nom[50];
    int opcio, sessio_iniciada;

    // Crear socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("Error creant el socket");
        return -1;
    }

    contacte_servidor.sin_family = AF_INET;
    contacte_servidor.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &contacte_servidor.sin_addr);

    // Iniciar sessió
    sessio_iniciada = inicia_sessio(s, &contacte_servidor, contacte_servidor_mida, nom);
    if (sessio_iniciada == 1) {
        printf("Inici de sessió correcte.\n");

        // Mostra el menú d'opcions
        while (1) {
            mostra_menu();
            scanf("%d", &opcio);
            getchar(); // Consumir el salt de línea

            snprintf(paquet, sizeof(paquet), "3 %d %s", opcio, nom);
            if (envia_paquet(s, &contacte_servidor, contacte_servidor_mida, paquet) < 0) {
                break;
            }

            if (opcio == 4) {
                printf("Tancant sessió...\n");
                break;
            }

            // Rebre resposta del servidor
            if (rep_paquet(s, paquet, &contacte_servidor, &contacte_servidor_mida) < 0) {
                break;
            }
            printf("Resposta del servidor: %s\n", paquet);
        }
    } else if (sessio_iniciada == 0) {
        printf("Usuari no trobat. Vols registrar un nou usuari? (s/n): ");
        char opcio_registre;
        scanf(" %c", &opcio_registre);
        getchar(); // Consumir el salt de línea

        if (opcio_registre == 's' || opcio_registre == 'S') {
            printf("Introdueix la teva contrasenya: ");
            fgets(paquet, 50, stdin);
            paquet[strcspn(paquet, "\n")] = '\0';
            registra_usuari(s, &contacte_servidor, contacte_servidor_mida, nom, paquet);
        }
    } else {
        printf("Error en inici de sessió. Credencials incorrectes.\n");
    }

    close(s);
    return 0;
}
