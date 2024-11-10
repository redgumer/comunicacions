/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.h                                              |
 | Autors     : Xavi, Jadi, Ivette                                               |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

// Inclusión de librerías estándar
#include <stdio.h>  // Para operaciones de entrada/salida
#include <stdlib.h> // Para gestión de memoria dinámica y control de procesos
#include <string.h> // Para manipulación de cadenas de texto
#include <unistd.h> // Para funciones del sistema como close(), sleep(), etc.
#include <stdbool.h>// Para el uso de variables booleanas
#include <time.h>   // Para funciones de manejo de tiempo (registro de eventos)

// Inclusión de librerías para manejo de sockets
#include <sys/socket.h> // Para la creación y manipulación de sockets
#include <arpa/inet.h>  // Para funciones de red como inet_addr(), htons()

// Inclusión de librerías personalizadas
#include "funcions_client.h" // Archivo de cabecera con funciones específicas del cliente
#include "notificacions.h"   // Archivo de cabecera con funciones de notificaciones


// Definición de constantes
#define LOG_FILE "data/registre.log" // Ruta del archivo de registro de eventos
#define MIDA_PAQUET 1024             // Tamaño del paquete de datos para la comunicación

void mostra_menu(){
    printf("\n--- MENÚ D'OPCIONS ---\n");
    printf("1. Veure perfil\n");
    printf("2. Veure amics\n");
    printf("3. Afegir nous amics\n");
    printf("4. Notificaions\n");
    printf("5. Tancar sessió\n");
    printf("Selecciona una opció: ");
}

void not_menu(char *nom)
{
    printf("\n--- MENÚ DE NOTIFICACIONS ---\n");
    printf("1. Consultar notificacions [%d]\n", tens_notificacions(nom));
    printf("2. Eliminar notificacions\n");
    printf("3. Envia notificaio\n");
    printf("4. Tornar al menú principal\n");
    printf("Selecciona una opció: ");
}

int envia_paquet(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, const char *paquet)
{
    if (sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)contacte_servidor, contacte_servidor_mida) < 0)
    {
        perror("Error enviant paquet");
        return -1;
    }
    return 0;
}

int rep_paquet(int s, char *paquet, struct sockaddr_in *contacte_servidor, socklen_t *contacte_servidor_mida)
{
    int bytes_rebuts = recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)contacte_servidor, contacte_servidor_mida);
    if (bytes_rebuts < 0)
    {
        perror("Error rebent el paquet");
        return -1;
    }
    paquet[bytes_rebuts] = '\0';
    return 0;
}

int inicia_sessio(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom)
{
    char paquet[MIDA_PAQUET], contrasenya[50];
    printf("Introdueix el teu nom d'usuari: ");
    fgets(nom, 50, stdin);
    nom[strcspn(nom, "\n")] = '\0';

    printf("Introdueix la teva contrasenya: ");
    fgets(contrasenya, 50, stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    snprintf(paquet, sizeof(paquet), "1 %s %s", nom, contrasenya);
    if (envia_paquet(s, contacte_servidor, contacte_servidor_mida, paquet) < 0)
    {
        return -1;
    }

    if (rep_paquet(s, paquet, contacte_servidor, &contacte_servidor_mida) < 0)
    {
        return -1;
    }

    return atoi(paquet);
}

int registra(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio)
{
    char paquet[MIDA_PAQUET];
    snprintf(paquet, sizeof(paquet), "2 %s %s %s %s %d %s \"%s\"", nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);

    if (envia_paquet(s, contacte_servidor, contacte_servidor_mida, paquet) < 0)
    {
        return -1;
    }

    if (rep_paquet(s, paquet, contacte_servidor, &contacte_servidor_mida) < 0)
    {
        return -1;
    }

    printf("Resposta del servidor: %s\n", paquet);
    return 0;
}

void registra_activitat(const char *tipus, const char *missatge)
{
    FILE *fitxer_log = fopen(LOG_FILE, "a");
    if (!fitxer_log)
    {
        printf("Error: No s'ha pogut obrir el fitxer de registre.\n");
        return;
    }

    // Obtenir el timestamp actual
    time_t temps_actual = time(NULL);
    struct tm *tm_info = localtime(&temps_actual);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // Escriure al fitxer de registre
    fprintf(fitxer_log, "[%s] [%s] %s\n", timestamp, tipus, missatge);

    // Tancar el fitxer
    fclose(fitxer_log);
}

int inicialitza_socket(struct sockaddr_in *contacte_servidor, char *ip, char *port)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        perror("Error creant el socket");
        return -1;
    }

    contacte_servidor->sin_family = AF_INET;
    contacte_servidor->sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &contacte_servidor->sin_addr);

    return s;
}

void processar_opcio(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom)
{
    char paquet[MIDA_PAQUET], nom_amic[50];
    int opcio;

    while (1)
    {
        mostra_menu();
        scanf("%d", &opcio);
        getchar(); // Consumir el salt de línea

        if (opcio == 3)
        {
            printf("Introdueix el teu nou amic: ");
            fgets(nom_amic, sizeof(nom_amic), stdin);
            nom_amic[strcspn(nom_amic, "\n")] = '\0';
            snprintf(paquet, sizeof(paquet), "3 %d %s %s", opcio, nom, nom_amic);
            printf("paquet: %s\n", paquet);
            registra_activitat("Afegint amic", nom_amic);
        }
        else if (opcio == 4)
        {
            gestionar_notificacions_client(nom, paquet, sizeof(paquet));
        }
        else if (opcio == 5)
        {
            printf("Tancant sessió...\n");
            registra_activitat("Tancant sessió", nom);
            break;
        }
        else
        {
            snprintf(paquet, sizeof(paquet), "3 %d %s", opcio, nom);
        }

        // Comunicacio amb el servidor
        if (envia_paquet(s, contacte_servidor, contacte_servidor_mida, paquet) < 0)
        {
            break;
        }

        if (rep_paquet(s, paquet, contacte_servidor, &contacte_servidor_mida) < 0)
        {
            registra_activitat("Error rebent resposta", nom);
            break;
        }
        printf("Resposta del servidor: %s\n", paquet);
    }
}

void registra_nou_usuari(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom)
{
    char contrasenya[50], sexe[10], estat_civil[20], ciutat[50], descripcio[100];
    int edat;

    printf("Introdueix la teva contrasenya %s: ", nom);
    fgets(contrasenya, sizeof(contrasenya), stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    printf("Introdueix el teu sexe (Home/Dona): ");
    fgets(sexe, sizeof(sexe), stdin);
    sexe[strcspn(sexe, "\n")] = '\0';

    printf("Introdueix el teu estat civil (Solter/Casat): ");
    fgets(estat_civil, sizeof(estat_civil), stdin);
    estat_civil[strcspn(estat_civil, "\n")] = '\0';

    printf("Introdueix la teva edat: ");
    scanf("%d", &edat);
    getchar(); // Consumir el salt de línia

    printf("Introdueix la teva ciutat: ");
    fgets(ciutat, sizeof(ciutat), stdin);
    ciutat[strcspn(ciutat, "\n")] = '\0';

    printf("Introdueix una breu descripció personal: ");
    fgets(descripcio, sizeof(descripcio), stdin);
    descripcio[strcspn(descripcio, "\n")] = '\0';

    registra(s, contacte_servidor, contacte_servidor_mida, nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);
    registra_activitat("Usuari registrat", nom);
}

bool mostra_error_inici_sessio(char *nom)
{
    bool tornar_menu = false;
     printf("Usuari no trobat. Vols registrar un nou usuari? (s/n): ");
        registra_activitat("Usuari no trobat", nom);
        char opcio_registre;
        scanf(" %c", &opcio_registre);
        getchar(); // Consumir el salt de línia

        if (opcio_registre == 's' || opcio_registre == 'S')
        {
           tornar_menu = true;
        }
    return tornar_menu;    
}

void finalitza_sessio(int s, char *nom)
{
    close(s);
    printf("Sessió finalitzada.\n");
    registra_activitat("Sessió finalitzada", nom);
}

int gestionar_notificacions_client(char *nom, char *paquet, size_t mida_paquet)
{
    char destinatari[50] = "";
    char missatge[256] = "";
    int not_opcio;

    while (1)
    {
        not_opcio = notificacions_menu(destinatari, missatge, nom);

        switch (not_opcio)
        {
            case 1: // Consultar notificacions
                snprintf(paquet, mida_paquet, "4 %s CONSULTAR_NOTIFICACIONS", nom);
                return 1; // Paquet preparat per ser enviat

            case 2: // Eliminar notificacions
                snprintf(paquet, mida_paquet, "4 %s ELIMINAR_NOTIFICACIONS", nom);
                return 1; // Paquet preparat per ser enviat

            case 3: // Enviar notificació
                if (strlen(destinatari) == 0 || strlen(missatge) == 0)
                {
                    printf("El destinatari o el missatge no poden estar buits.\n");
                    continue;
                }
                snprintf(paquet, mida_paquet, "4 %s ENVIAR_NOTIFICACIO %s %s", nom, destinatari, missatge);
                return 1; // Paquet preparat per ser enviat

            case 4: // Tornar al menú principal
                return 1; // Indica a `processar_opcio()` que no hi ha paquet a enviar

            default:
                printf("Opció no vàlida, torna-ho a intentar.\n");
                break;
        }
    }
}

int notificacions_menu(char *destinatari, char *missatge, char *nom)
{

    int opcio;
    not_menu(nom);
    scanf("%d", &opcio);
    getchar(); // Consumir el salt de línia

    if (opcio == 3) // Si l'opció és enviar notificació
    {
        printf("Introdueix el nom del destinatari: ");
        fgets(destinatari, 50, stdin);
        destinatari[strcspn(destinatari, "\n")] = '\0'; // Eliminar el salt de línia

        printf("Introdueix el missatge: ");
        fgets(missatge, 256, stdin);
        missatge[strcspn(missatge, "\n")] = '\0'; // Eliminar el salt de línia
    }

    return opcio;
}
