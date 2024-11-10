/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : src/server.c                                                     |
 | Autors     : Xavi, Jadi, Ivette                                               |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/
// Inclusión de librerías estándar
#include <stdio.h>      // Para operaciones de entrada y salida
#include <stdlib.h>     // Para funciones de control de procesos y manejo de memoria
#include <string.h>     // Para manipulación de cadenas de texto

// Inclusión de librerías de red y sockets
#include <unistd.h>     // Para funciones del sistema como close(), read(), write()
#include <sys/socket.h> // Para creación y manipulación de sockets
#include <arpa/inet.h>  // Para funciones de red como inet_addr(), htons()

// Inclusión de librerías personalizadas
#include "funcions_servidor.h" // Archivo de cabecera con funciones específicas para el servidor
#include "notificacions.h"     // Archivo de cabecera con funciones de notificaciones

// Definición de constantes
#define MIDA_PAQUET 1024 // Tamaño del paquete de datos para la comunicación
#define FILE_USUARIS "data/usuaris.txt"   // Archivo con datos de usuarios

void carregar_usuaris(const char *nom_fitxer) {
    // Obrim el fitxer en mode lectura
    FILE *fitxer = fopen(nom_fitxer, "r");
    if (fitxer == NULL) {
        printf("No s'ha pogut obrir el fitxer: %s\n", nom_fitxer);
        return;
    }

    // Inicialitzem el nombre d'usuaris carregats
    num_usuaris = 0;
    char linia[512];

    // Llegim el fitxer línia per línia
    while (fgets(linia, sizeof(linia), fitxer) != NULL) {
        // Comprovem que no superem el límit màxim d'usuaris
        if (num_usuaris >= MAX_USUARI) {
            printf("Nombre màxim d'usuaris (%d) assolit. No es carregaran més usuaris.\n", MAX_USUARI);
            break;
        }

        // Parsejar la línia del fitxer
        Usuari_t usuari;
        char ciutat[50], descripcio[100];

        int camps_llegits = sscanf(linia, "%d %49s %49s %9s %19s %d %49s \"%99[^\"]\"",
                                   &usuari.id,
                                   usuari.nom,
                                   usuari.contrasenya,
                                   usuari.sexe,
                                   usuari.estat_civil,
                                   &usuari.edat,
                                   ciutat,
                                   descripcio);

        // Comprovem si la línia s'ha llegit correctament
        if (camps_llegits == 8) {
            // Omplim els camps de l'estructura
            strncpy(usuari.ciutat, ciutat, sizeof(usuari.ciutat) - 1);
            strncpy(usuari.descripcio, descripcio, sizeof(usuari.descripcio) - 1);
            usuari.num_notificacions = 0;

            // Afegim l'usuari a l'array global
            usuaris[num_usuaris] = usuari;
            num_usuaris++;
        } else {
            printf("Error al llegir la línia: %s\n", linia);
        }
    }

    // Tanquem el fitxer
    fclose(fitxer);

    printf("S'han carregat %d usuaris del fitxer %s\n", num_usuaris, nom_fitxer);
}

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
    carregar_usuaris(FILE_USUARIS);
    carrega_notificacions(usuaris, num_usuaris);
    printf("Usuaris carregats:\n");
    for(int i = 0; i < num_usuaris; i++) {
        printf("Usuari %d: %d %s %s %s %s %d %s %s %d\n",
               i,
               usuaris[i].id,
               usuaris[i].nom,
               usuaris[i].contrasenya,
               usuaris[i].sexe,
               usuaris[i].estat_civil,
               usuaris[i].edat,
               usuaris[i].ciutat,
               usuaris[i].descripcio,
               usuaris[i].num_notificacions);
    }
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
