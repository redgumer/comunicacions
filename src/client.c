/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.c                                              |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "funcions_client.h"

#define MIDA_PAQUET 1024
#define MAX_USUARI 50
#define MAX_CONTRASENYA 50

int main(int argc, char **argv) {
    if (argc == 3) {
        struct sockaddr_in contacte_servidor;
        char nom[50], contrasenya[50], paquet[MIDA_PAQUET];

        int s = inicializar_cliente(argv[1], atoi(argv[2]), &contacte_servidor);

        // Obtener credenciales y preparar el paquete
        demana_credencials(nom, contrasenya);
        snprintf(paquet, MIDA_PAQUET, "%s:%s", nom, contrasenya); // Formato "nombre:contrasena"
        
        // Enviar credenciales al servidor y procesar la respuesta
        if (enviar_paquete(s, &contacte_servidor, paquet) > 0) {
            printf("Credenciales enviadas. Esperando respuesta...\n");
            procesa_respuesta_servidor(s, &contacte_servidor);
        } else {
            perror("Error al enviar las credenciales");
        }
        close(s);
    } else {
        printf("Uso: %s <IP_servidor> <puerto>\n", argv[0]);
    }
    return 0;
}
