/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : src/funcions_servidor.c                                          |
 | Autors     : Xavi,                                                            |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

// Inclusión de librerías estándar
#include <stdio.h>  // Para operaciones de entrada/salida
#include <stdlib.h> // Para funciones de control de procesos y gestión de memoria
#include <string.h> // Para manipulación de cadenas de texto
#include <time.h>   // Para funciones de tiempo (registro de eventos)

// Inclusión de librerías de red y sockets
#include <unistd.h>     // Para funciones del sistema como close(), read(), write()
#include <sys/socket.h> // Para creación y manipulación de sockets
#include <arpa/inet.h>  // Para funciones de red como inet_addr(), htons()

// Inclusión de librerías personalizadas
#include "funcions_servidor.h" // Funciones específicas para la lógica del servidor
#include "fun_afegir_amic.h"   // Funciones para gestión de amistades
#include "notificacions.h"     // Funciones para gestión de amistades

// Definición de constantes
#define MIDA_PAQUET 1024 // Tamaño del paquete para la comunicación
#define MAX_USUARIS 50   // Número máximo de usuarios permitidos
#define MAX_LINE 256     // Tamaño máximo para leer líneas del archivo

// Definición de rutas de archivos
#define LOG_FILE "data/registre.log"      // Archivo de registro de eventos
#define FILE_USUARIS "data/usuaris.txt"   // Archivo con datos de usuarios
#define FILE_AMISTATS "data/amistats.txt" // Archivo con datos de amistades

// variables globales
extern Usuari_t usuaris[MAX_USUARIS];
extern int num_usuaris;
extern int id_max;

int verifica_usuari(const char *nom, const char *contrasenya)
{
    FILE *fitxer;
    char linia[MAX_LINE];
    char id[10], nom_fitxer[50], contrasenya_fitxer[20];
    char sexe[10], estatCivil[20], ciutat[50], descripcio[100];
    int edat;

    // Obrim el fitxer d'usuaris
    fitxer = fopen(FILE_USUARIS, "r");
    if (fitxer == NULL)
    {
        perror("Error al obrir el fitxer");
        return -1; // Error general
    }

    // Llegim línia per línia
    while (fgets(linia, sizeof(linia), fitxer) != NULL)
    {
        // Extraurem les dades de la línia
        sscanf(linia, "%s %s %s %s %s %d %s \"%[^\"]\"",
               id, nom_fitxer, contrasenya_fitxer, sexe, estatCivil, &edat, ciutat, descripcio);

        // Comprovem si el nom coincideix
        if (strcmp(nom_fitxer, nom) == 0)
        {
            // Comprovem si la contrasenya coincideix
            fclose(fitxer);
            return strcmp(contrasenya_fitxer, contrasenya) == 0 ? 1 : -2;
        }
    }

    // Tanquem el fitxer i retornem que l'usuari no existeix
    fclose(fitxer);
    return 0;
}

int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio)
{
    char linia[MAX_LINE];
    int id_max = 0;

    if (num_usuaris >= MAX_USUARIS)
    {
        printf("Error: Llista d'usuaris plena.\n");
        return -1;
    }

    // Obre els fitxers
    FILE *fitxer_usuaris = fopen(FILE_USUARIS, "a+");
    FILE *fitxer_amistats = fopen(FILE_AMISTATS, "a+");
    if (!fitxer_usuaris || !fitxer_amistats)
    {
        printf("Error: No s'han pogut obrir els fitxers.\n");
        if (fitxer_usuaris)
            fclose(fitxer_usuaris);
        if (fitxer_amistats)
            fclose(fitxer_amistats);
        return -1;
    }

    // Llegeix el fitxer d'usuaris per trobar l'ID màxim i comprovar si l'usuari ja existeix
    while (fgets(linia, sizeof(linia), fitxer_usuaris))
    {
        int id_exist;
        char nom_exist[50];
        if (sscanf(linia, "%d %49s", &id_exist, nom_exist) != 2)
            continue;

        // Actualitza l'ID màxim
        if (id_exist > id_max)
            id_max = id_exist;

        // Comprova si l'usuari ja existeix
        if (strcmp(nom, nom_exist) == 0)
        {
            printf("Error: L'usuari ja existeix.\n");
            fclose(fitxer_usuaris);
            fclose(fitxer_amistats);
            return 0;
        }
    }

    // Assigna un nou ID
    int nou_id = id_max + 1;

    // Copia les dades a la llista d'usuaris
    usuaris[num_usuaris].id = nou_id;
    strncpy(usuaris[num_usuaris].nom, nom, sizeof(usuaris[num_usuaris].nom) - 1);
    usuaris[num_usuaris].nom[sizeof(usuaris[num_usuaris].nom) - 1] = '\0';
    strncpy(usuaris[num_usuaris].contrasenya, contrasenya, sizeof(usuaris[num_usuaris].contrasenya) - 1);
    usuaris[num_usuaris].contrasenya[sizeof(usuaris[num_usuaris].contrasenya) - 1] = '\0';
    strncpy(usuaris[num_usuaris].sexe, sexe, sizeof(usuaris[num_usuaris].sexe) - 1);
    usuaris[num_usuaris].sexe[sizeof(usuaris[num_usuaris].sexe) - 1] = '\0';
    strncpy(usuaris[num_usuaris].estat_civil, estat_civil, sizeof(usuaris[num_usuaris].estat_civil) - 1);
    usuaris[num_usuaris].estat_civil[sizeof(usuaris[num_usuaris].estat_civil) - 1] = '\0';
    usuaris[num_usuaris].edat = edat;
    strncpy(usuaris[num_usuaris].ciutat, ciutat, sizeof(usuaris[num_usuaris].ciutat) - 1);
    usuaris[num_usuaris].ciutat[sizeof(usuaris[num_usuaris].ciutat) - 1] = '\0';
    strncpy(usuaris[num_usuaris].descripcio, descripcio, sizeof(usuaris[num_usuaris].descripcio) - 1);
    usuaris[num_usuaris].descripcio[sizeof(usuaris[num_usuaris].descripcio) - 1] = '\0';

    num_usuaris++;

    // Escriu al fitxer d'usuaris
    fprintf(fitxer_usuaris, "%d %s %s %s %s %d %s %s\n", nou_id, nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);

    // Afegir l'usuari al fitxer d'amistats amb el nou format
    fprintf(fitxer_amistats, "%d;\n", nou_id);

    // Tanca els fitxers
    fclose(fitxer_usuaris);
    fclose(fitxer_amistats);

    printf("Nou usuari registrat: %s amb ID %d\n", nom, nou_id);
    return 1;
}

char *obtenirPerfilUsuari(const char *nomUsuari)
{
    FILE *fitxer;
    char linia[MAX_LINE];
    Usuari_t usuari;
    char *perfil = (char *)malloc(512 * sizeof(char));

    if (perfil == NULL)
    {
        printf("Error d'assignació de memòria.\n");
        return "";
    }

    // Obrir el fitxer d'usuaris en mode lectura
    fitxer = fopen(FILE_USUARIS, "r");
    if (fitxer == NULL)
    {
        printf("Error obrint el fitxer d'usuaris.\n");
        free(perfil);
        return "";
    }

    // Llegir línia per línia
    while (fgets(linia, sizeof(linia), fitxer) != NULL)
    {
        // Extreure els camps del perfil i emmagatzemar-los a la `struct Usuari`
        sscanf(linia, "%d %s %s %s %s %d %s %[^\n]",
               &usuari.id, usuari.nom, usuari.contrasenya,
               usuari.sexe, usuari.estat_civil, &usuari.edat,
               usuari.ciutat, usuari.descripcio);

        // Comprovar si el nom coincideix amb l'usuari cercat
        if (strcmp(usuari.nom, nomUsuari) == 0)
        {
            // Construir el perfil formatat
            sprintf(perfil,
                    "\n"
                    "Nom: %s\n"
                    "Sexe: %s\n"
                    "Estat civil: %s\n"
                    "Edat: %d\n"
                    "Ciutat: %s\n"
                    "Descripció: %s\n",
                    usuari.nom,         // char *
                    usuari.sexe,        // char *
                    usuari.estat_civil, // char *
                    usuari.edat,        // int
                    usuari.ciutat,      // char *
                    usuari.descripcio   // char *
            );
            fclose(fitxer);
            return perfil;
        }
    }

    // Tancar el fitxer i alliberar memòria si no es troba l'usuari
    fclose(fitxer);
    free(perfil);
    return "";
}

int buscarIdUsuari(const char *nomUsuari)
{
    for (int i = 0; i < num_usuaris; i++)
    {
        if (strcmp(usuaris[i].nom, nomUsuari) == 0)
        {
            return usuaris[i].id;
        }
    }
    return -1; // Usuari no trobat
}

char *obtenirNomUsuari(int idUsuari)
{
    for (int i = 0; i < num_usuaris; i++)
    {
        if (usuaris[i].id == idUsuari)
        {
            return usuaris[i].nom;
        }
    }
    return NULL; // ID no trobat
}

char *veureAmics(const char *nomUsuari)
{
    int idUsuari = buscarIdUsuari(nomUsuari);
    if (idUsuari == -1)
    {
        return strdup("Usuari no trobat.");
    }

    FILE *fitxerAmistats = fopen(FILE_AMISTATS, "r");
    if (!fitxerAmistats)
    {
        perror("Error obrint el fitxer d'amistats");
        return NULL;
    }

    char linia[MAX_LINE];
    char *resultat = malloc(2048 * sizeof(char));
    resultat[0] = '\0';

    while (fgets(linia, MAX_LINE, fitxerAmistats))
    {
        int id1;
        char amics[1024];
        // Llegir l'ID de l'usuari i la llista d'amics
        if (sscanf(linia, "%d; %[^\n]", &id1, amics) != 2)
        {
            continue;
        }

        // Si la línia correspon a l'usuari, processar la llista d'amics
        if (id1 == idUsuari)
        {
            char *amic = strtok(amics, " ");
            while (amic)
            {
                int idAmic = atoi(amic);
                char *nomAmic = obtenirNomUsuari(idAmic);
                if (nomAmic)
                {
                    char *perfilAmic = obtenirPerfilUsuari(nomAmic);
                    if (perfilAmic)
                    {
                        strcat(resultat, perfilAmic);
                        strcat(resultat, "\n");
                        free(perfilAmic);
                    }
                }
                amic = strtok(NULL, " ");
            }
        }
    }
    fclose(fitxerAmistats);

    if (strlen(resultat) == 0)
    {
        strcpy(resultat, "No s'han trobat amics per aquest usuari.");
    }

    return resultat;
}

void processa_opcio_menu(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, int opcio, const char *nom, const char *nouAmic)
{
    char resposta[MIDA_PAQUET];
    char missatge_log[150];
    switch (opcio)
    {
    case 1:
        // Obtenir el perfil de l'usuari
        char *perfil = obtenirPerfilUsuari(nom);
        if (perfil)
        {
            snprintf(resposta, sizeof(resposta), "\n\nPerfil de l'usuari: %s", perfil);
            snprintf(missatge_log, sizeof(missatge_log), "Usuari %s ha consultat el seu perfil.", nom);
            registra_activitat("INFO", missatge_log);
            free(perfil);
        }
        else
        {
            snprintf(resposta, sizeof(resposta), "Error: No s'ha pogut obtenir el perfil de l'usuari %s.", nom);
            registra_activitat("ERROR", resposta);
        }
        break;

    case 2:
        // Veure amics de l'usuari
        char *amics = veureAmics(nom);
        if (amics)
        {
            snprintf(resposta, sizeof(resposta), "\n\nAmics de l'usuari: %s", amics);
            snprintf(missatge_log, sizeof(missatge_log), "Usuari %s ha consultat la seva llista d'amics.", nom);
            registra_activitat("INFO", missatge_log);
            free(amics);
        }
        else
        {
            snprintf(resposta, sizeof(resposta), "Error: No s'han pogut obtenir els amics de l'usuari %s.", nom);
            registra_activitat("ERROR", resposta);
        }
        break;

    case 3:
        // Afegir un nou amic
        int idUsuari = buscarIdUsuari(nom);
        int idNouAmic = buscarIdUsuari(nouAmic);

        if (idUsuari >= 0 && idNouAmic >= 0)
        {
            llegirAmistats(FILE_AMISTATS);
            afegirAmic(idUsuari, idNouAmic);
            afegirAmic(idNouAmic, idUsuari);
            ordenarAmistats();
            guardarAmistats(FILE_AMISTATS);
            snprintf(resposta, sizeof(resposta), "\n\n%s i %s ara són amics", nom, nouAmic);
            snprintf(missatge_log, sizeof(missatge_log), "Usuari %s ha afegit %s com a nou amic.", nom, nouAmic);
            registra_activitat("INFO", missatge_log);
        }
        else
        {
            snprintf(resposta, sizeof(resposta), "Error: No s'ha pogut afegir %s com a amic de %s.", nouAmic, nom);
            registra_activitat("ERROR", resposta);
        }
        break;

    case 4:
        // Tancar sessió
        snprintf(resposta, sizeof(resposta), "Sessió tancada per l'usuari %s.", nom);
        snprintf(missatge_log, sizeof(missatge_log), "Usuari %s ha tancat la sessió.", nom);
        registra_activitat("INFO", missatge_log);
        break;

    default:
        // Opció no vàlida
        snprintf(resposta, sizeof(resposta), "Opció no vàlida.");
        snprintf(missatge_log, sizeof(missatge_log), "Usuari %s ha seleccionat una opció no vàlida (%d).", nom, opcio);
        registra_activitat("WARNING", missatge_log);
        break;
    }

    // Enviar la resposta al client
    sendto(s, resposta, strlen(resposta), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
    snprintf(missatge_log, sizeof(missatge_log), "Resposta enviada al client %s:%d - %.100s",
             inet_ntoa(contacte_client.sin_addr), ntohs(contacte_client.sin_port), resposta);
    registra_activitat("SERVER", missatge_log);
}

void processa_peticio(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, char *paquet)
{
    int codi_operacio, opcio;
    char nom[50], contrasenya[50], sexe[10], estat_civil[20], ciutat[50], descripcio[100], nouAmic[50];
    int edat;
    printf("Paquet rebut: %s\n", paquet);
    if (sscanf(paquet, "%d", &codi_operacio) != 1)
    {
        printf("Error de format en el paquet rebut.\n");
        return;
    }
    printf("Codi d'operació: %d\n", codi_operacio);
    switch (codi_operacio)
    {
    case 1:
        if (sscanf(paquet, "%d %s %s", &codi_operacio, nom, contrasenya) == 3)
        {
            int codi_resposta = verifica_usuari(nom, contrasenya);
            snprintf(paquet, MIDA_PAQUET, "%d", codi_resposta);
            sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        }
        else
        {
            printf("Error de format en inici de sessió.\n");
        }
        break;
    case 2:
        if (sscanf(paquet, "%d %s %s %s %s %d %s %[^\n]", &codi_operacio, nom, contrasenya, sexe, estat_civil, &edat, ciutat, descripcio) == 8)
        {
            printf("Registre d'usuari case 2: %s %s %s %s %d %s %s\n", nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);
            int codi_resposta = registra_usuari(nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);
            snprintf(paquet, MIDA_PAQUET, "Codi resposta: %d", codi_resposta);
            sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        }
        else
        {
            printf("Error de format en registre d'usuari.\n");
        }
        break;
    case 3:
        int num_elements = sscanf(paquet, "%d %d %s %s", &codi_operacio, &opcio, nom, nouAmic);
        if (num_elements == 3 || num_elements == 4)
        {
            printf("Processant opció de menú: %d %d %s %s\n", codi_operacio, opcio, nom, nouAmic);
            processa_opcio_menu(s, contacte_client, contacte_client_mida, opcio, nom, nouAmic);
        }
        else
        {
            printf("Error de format en opcions del menú.\n");
        }
        break;
    case 4:
        printf("Consultar notificacions\n");
        gestiona_notificacions_servidor(paquet, s, contacte_client, contacte_client_mida, usuaris);
        break;
    default:
        printf("Operació desconeguda: %d\n", codi_operacio);
    }
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
