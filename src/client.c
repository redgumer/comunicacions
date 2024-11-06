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
#include "funcions_client.h" // Per funcions específiques definides per al client en el projecte

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        int s;
        struct sockaddr_in contacte_servidor;

        // Inicialització de connexió amb el servidor
        s = inicialitza_connexio(&contacte_servidor, argv[1], argv[2]);

        // Verificació de l'usuari
        if (verifica_usuari(s, contacte_servidor) == 1)
        {
            // Mostra el menú d'opcions
            gestiona_menu(s, contacte_servidor);
        }
        if (verifica_usuari(s, contacte_servidor) == 0)
        {
            char nom[MAX_USUARI], contrasenya[MAX_CONTRASENYA], sexe[MAX_SEXE], estat_civil[MAX_ESTAT_CIVIL], ciutat[MAX_CIUTAT], descripcio[MAX_DESC], paquet[MIDA_PAQUET];
            int edat;
            // Enviar nom d'usuari i contrasenya
            printf("Introdueix el teu nom d'usuari: ");
            scanf("%s", nom);

            printf("Introdueix la teva contrasenya: ");
            scanf("%s", contrasenya);

            // Enviar informació addicional per al registre
            printf("Introdueix el teu sexe (M/F): ");
            scanf("%s", sexe);

            printf("Introdueix el teu estat civil: ");
            scanf("%s", estat_civil);

            printf("Introdueix la teva edat: ");
            scanf("%d", &edat);

            printf("Introdueix la teva ciutat: ");
            scanf("%s", ciutat);

            printf("Introdueix una breu descripció personal: ");
            scanf(" %[^\n]", descripcio); // Permet que la descripció tingui espais

            // Empaquetar totes les dades en un únic paquet i enviar-lo al servidor
            snprintf(paquet, sizeof(paquet), "%s %s %s %s %d %s %s", nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);
            sendto(s, paquet, sizeof(paquet), 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));
        }
        else
        {
            printf("Error: No s'ha pogut verificar l'usuari.\n");
        }
        return 0;
    }
}
