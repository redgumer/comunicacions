
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "funcions_servidor.h"

#define MIDA_PAQUET 1024
#define MAX_USUARIS 10
#define FILE_USUARIS "data/usuaris.txt"
#define FILE_AMISTATS "data/amistats.txt"

Usuari usuaris[MAX_USUARIS];
int num_usuaris = 0;

int verifica_usuari(const char *nom, const char *contrasenya)
{
    FILE *fitxer;
    char linia[256];
    char nom_fitxer[50], contrasenya_fitxer[20];
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
        sscanf(linia, "%s %s %s %s %d %s %[^\n]",
               nom_fitxer, contrasenya_fitxer, sexe, estatCivil, &edat, ciutat, descripcio);

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
    char linia[256];

    if (num_usuaris >= MAX_USUARIS)
    {
        printf("Error: Llista d'usuaris plena.\n");
        return -1;
    }

    // Obre els fitxers
    FILE *fitxer_usuaris = fopen(FILE_USUARIS, "a");
    FILE *fitxer_amistats = fopen(FILE_AMISTATS, "a+");
    if (!fitxer_usuaris || !fitxer_amistats)
    {
        printf("Error: No s'han pogut obrir els fitxers.\n");
        if (fitxer_usuaris) fclose(fitxer_usuaris);
        if (fitxer_amistats) fclose(fitxer_amistats);
        return -1;
    }

    // Comprova si l'usuari ja existeix
    while (fgets(linia, sizeof(linia), fitxer_usuaris))
    {
        char nom_exist[50];
        sscanf(linia, "%s", nom_exist);
        if (strcmp(nom, nom_exist) == 0)
        {
            printf("Error: L'usuari ja existeix.\n");
            fclose(fitxer_usuaris);
            fclose(fitxer_amistats);
            return 0;
        }
    }

    // Assigna un nou ID
    int nou_id = num_usuaris + 1;

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
    fprintf(fitxer_usuaris, "%s %s %s %s %d %s %s\n", nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);

    // Escriu al fitxer d'amistats amb un salt de línia previ
    fseek(fitxer_amistats, 0, SEEK_END);
    fprintf(fitxer_amistats, "\n%d; %s", nou_id, nom);

    // Tanca els fitxers
    fclose(fitxer_usuaris);
    fclose(fitxer_amistats);

    printf("Nou usuari registrat: %s amb ID %d\n", nom, nou_id);
    return 1;
}

void processa_opcio_menu(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, int opcio, const char *nom)
{
    char resposta[MIDA_PAQUET];
    switch (opcio)
    {
    case 1:
        snprintf(resposta, sizeof(resposta), "Perfil de l'usuari: %s", nom);
        break;
    case 2:
        snprintf(resposta, sizeof(resposta), "Amics de l'usuari %s: No implementat encara.", nom);
        break;
    case 3:
        snprintf(resposta, sizeof(resposta), "Afegir nous amics: Funció no implementada encara.");
        break;
    case 4:
        snprintf(resposta, sizeof(resposta), "Sessió tancada per l'usuari %s.", nom);
        break;
    default:
        snprintf(resposta, sizeof(resposta), "Opció no vàlida.");
    }
    sendto(s, resposta, strlen(resposta), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
}

void processa_peticio(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, char *paquet)
{
    int codi_operacio, opcio;
    char nom[50], contrasenya[50], sexe[10], estat_civil[20], ciutat[50], descripcio[100];
    int edat;

    if (sscanf(paquet, "%d", &codi_operacio) != 1)
    {
        printf("Error de format en el paquet rebut.\n");
        return;
    }

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
        if (sscanf(paquet, "%d %d %s", &codi_operacio, &opcio, nom) == 3)
        {
            processa_opcio_menu(s, contacte_client, contacte_client_mida, opcio, nom);
        }
        else
        {
            printf("Error de format en opcions del menú.\n");
        }
        break;
    default:
        printf("Operació desconeguda: %d\n", codi_operacio);
    }
}
