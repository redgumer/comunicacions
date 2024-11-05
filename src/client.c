/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : client.c                                                         |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../include/funcions_client.h"



#define MIDA_PAQUET 1500
#define MAX_USUARI 30
#define MAX_CONTRASENYA 30

/**
 * @brief Funció principal del client per connectar-se al servidor, verificar credencials i mostrar el menú
 *
 * @param argc Nombre d'arguments passats per línia de comandes (haurien de ser 3)
 * @param argv Vector de cadenes amb els arguments (IP i port del servidor)
 * @return int Retorna 0 al final de l'execució correcte
 *
 * Aquesta funció crea un socket de connexió amb el servidor, demana les credencials de l'usuari
 * i les envia per a la verificació. Si la verificació és correcta, mostra un menú que permet a l'usuari
 * interactuar amb diferents opcions fins que decideixi sortir.
 */
int main(int argc, char **argv)
{
    if (argc == 3)
    {
        int s;
        struct sockaddr_in contacte_servidor;
        char paquet[MIDA_PAQUET];

        // Configuració de connexió amb el servidor
        s = socket(AF_INET, SOCK_DGRAM, 0);
        contacte_servidor.sin_family = AF_INET;
        contacte_servidor.sin_addr.s_addr = inet_addr(argv[1]);
        contacte_servidor.sin_port = htons(atoi(argv[2]));

        // Demana les credencials de l'usuari
        char nom[MAX_USUARI], contrasenya[MAX_CONTRASENYA];
        demana_credencials(nom, contrasenya);

        // Envia les credencials al servidor per a verificació
        sprintf(paquet, "%s %s", nom, contrasenya);
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));

        // Rep la resposta de verificació del servidor
        recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
        printf("Resposta del servidor: %s\n", paquet);

        // Si la verificació és correcta, mostra el menú d'opcions
        if (strcmp(paquet, "Usuari verificat\n") == 0)
        {
            int opcio;
            do
            {
                // Mostra el menú i envia l'opció seleccionada al servidor
                opcio = mostra_menu();
                sprintf(paquet, "%d %s", opcio, nom);
                sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));

                // Rep la resposta del servidor per cada opció seleccionada
                recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
                printf("Resposta del servidor:\n%s\n", paquet);

            } while (opcio != 5);  // Repeteix fins que l'usuari decideixi sortir
        }
    }
    return 0;
}