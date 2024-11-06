<<<<<<< HEAD
// ================================ LLIBRERIES ESTÀNDARD ================================ //
#include <stdio.h>          // Per funcions d'entrada/sortida com printf, scanf, etc.
#include <stdlib.h>         // Per funcions generals com malloc, free, etc.
#include <string.h>         // Per funcions de manipulació de cadenes, com strcpy, strcmp, etc.

// ================================ LLIBRERIES DE XARXA ================================ //
#include <sys/socket.h>     // Per funcions de sockets, com socket, connect, etc.
#include <sys/types.h>      // Per definicions de tipus de dades utilitzades en sockets
#include <netinet/in.h>     // Per funcions i estructures de la família de protocols d'Internet
#include <arpa/inet.h>      // Per la conversió d'adreces d'Internet (ex: inet_addr)
#include <unistd.h>         // Per funcions del sistema Unix, com close, read, write, etc.

// ================================ LLIBRERIES PROPIES ================================ //
#include "funcions_client.h" // Per funcions específiques definides per al client en el projecte


#define MIDA_PAQUET 100
#define MAX_USUARI 30
#define MAX_CONTRASENYA 30

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
=======
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
>>>>>>> 7a45e98ae33727134f6a194d7d1aea31be3f7b85
    }
    return 0;
}
