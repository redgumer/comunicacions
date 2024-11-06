/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.c                                              |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

// Llibreries bàsiques
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"

// Llibreries de xarxa
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Fitxer de funcions del servidor
#include "../include/funcions_client.h"

#define MIDA_PAQUET 1500

void demana_credencials(char *nom, char *contrasenya)
{
    printf("Introdueix el teu nom: ");
    scanf("%49s", nom); // Limita la longitud para evitar desbordamientos

    printf("Introdueix la teva contrasenya: ");
    scanf("%49s", contrasenya); // Limita la longitud para la contrasenya
}

int mostra_menu(int tipus_menu)
{
    int opcio;

    if (tipus_menu == 1)
    { // Menú principal d'usuari
        printf("\nMenu d'usuari:\n");
        printf("1. Veure perfil\n");
        printf("2. Veure els meus amics\n");
        printf("3. Afegir amics nous\n");
        printf("4. Consultar l'activitat de l'usuari\n");
        printf("5. Tancar el programa\n");
    }
    else if (tipus_menu == 2)
    { // Exemple de menú d'administrador
        printf("\nMenu d'administrador:\n");
        printf("1. Gestionar usuaris\n");
        printf("2. Veure totes les activitats\n");
        printf("3. Consultar estadístiques\n");
        printf("4. Tancar sessió\n");
    }
    else if (tipus_menu == 3)
    { // Menú per suggeriments d'amics
        printf("\nMenu de suggeriments d'amics:\n");
        printf("1. Veure suggeriments d'amics\n");
        printf("2. Afegir amic suggerit\n");
        printf("3. Tornar al menú principal\n");
    }

    printf("Selecciona una opció: ");
    scanf("%d", &opcio);
    return opcio;
}

int inicializar_cliente(const char *ip, int port, struct sockaddr_in *contacte_servidor) {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("Error al crear el socket");
        exit(1);
    }
    contacte_servidor->sin_family = AF_INET;
    contacte_servidor->sin_addr.s_addr = inet_addr(ip);
    contacte_servidor->sin_port = htons(port);
    return s;
}

// Función para enviar un paquete
int enviar_paquete(int s, struct sockaddr_in *contacte_servidor, char *paquet) {
    return sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)contacte_servidor, sizeof(*contacte_servidor));
}

// Función para procesar la respuesta del servidor
void procesa_respuesta_servidor(int s, struct sockaddr_in *contacte_servidor) {
    int respuesta;
    socklen_t contacte_servidor_mida = sizeof(*contacte_servidor);

    if (recvfrom(s, &respuesta, sizeof(respuesta), 0, (struct sockaddr *)contacte_servidor, &contacte_servidor_mida) > 0) {
        // Interpretar el código de respuesta
        switch (respuesta) {
            case -1:
                printf("Error general en la operación.\n");
                break;
            case 0:
                printf("Usuario no encontrado o ya existe, según la función ejecutada.\n");
                break;
            case 1:
                printf("Operación exitosa: usuario autenticado o registrado correctamente.\n");
                break;
            case -2:
                printf("Error de autenticación: contraseña incorrecta o formato inválido.\n");
                break;
            default:
                printf("Código de respuesta desconocido: %d\n", respuesta);
                break;
        }
    } else {
        perror("Error al recibir la respuesta del servidor");
    }
}


