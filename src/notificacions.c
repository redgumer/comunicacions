/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : src/notificacions.c                                              |
 | Autors     : Xavi, Jadi, Ivette                                               |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

// Inclusión de librerías estándar
#include <stdio.h>  // Para operaciones de entrada/salida
#include <string.h> // Para manipulación de cadenas de texto
#include <stdlib.h> // Para funciones estándar, como manejo de memoria
#include <ctype.h>  // Para funciones de manipulación de caracteres
#include <unistd.h> // Per a la funció unlink()
#include <dirent.h> // Para operaciones de directorios

// Inclusión de librerías personalizadas
#include "funcions_servidor.h" // Archivo de cabecera con funciones específicas para el servidor
#include "notificacions.h"     // Archivo de cabecera con funciones de notificaciones
#include "tipus.h"             // Archivo de cabecera con definiciones de tipos de datos

// Definición de constantes
#define MIDA_PAQUET 1024                                            // Tamaño del paquete para la comunicación
#define MAX_MISSATGE 256                                            // Tamaño máximo para un mensaje de notificación
#define MAX_NOTIFICACIONS 10                                        // Número máximo de notificaciones almacenadas por usuario
#define FITXER_NOTIFICACIONS "data/notificacions/notificacions.txt" // Ruta del archivo de notificaciones
#define MAX_USUARIS 50                                              // Número máximo de usuarios permitidos
#define MAX_LINE 256                                                // Tamaño máximo para leer líneas del archivo
#define MAX_USUARI 50                                               // Tamaño máximo para el nombre de usuario

extern Usuari_t usuaris[MAX_USUARIS];
extern int num_usuaris;

void carrega_notificacions(Usuari_t *usuaris, int num_usuaris)
{
    // Obre el directori de notificacions
    DIR *directori = opendir("data/notificacions/");
    if (directori == NULL)
    {
        printf("No s'ha pogut obrir el directori de notificacions.\n");
        return;
    }

    struct dirent *entrada;
    int usuaris_trobats = 0;

    // Itera per cada fitxer del directori
    while ((entrada = readdir(directori)) != NULL)
    {
        // Ometre els fitxers especials "." i ".."
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        // Busca l'usuari corresponent pel nom del fitxer
        for (int i = 0; i < num_usuaris; i++)
        {
            char fitxer_notificacions[MAX_LINE];
            snprintf(fitxer_notificacions, sizeof(fitxer_notificacions), "%s_not.txt", usuaris[i].nom);

            // Comprova si el fitxer correspon a l'usuari
            if (strcmp(entrada->d_name, fitxer_notificacions) == 0)
            {
                char ruta_fitxer[512];
                snprintf(ruta_fitxer, sizeof(ruta_fitxer), "data/notificacions/%s", entrada->d_name);

                // Obre el fitxer de notificacions en mode lectura
                FILE *fitxer = fopen(ruta_fitxer, "r");
                if (fitxer == NULL)
                {
                    printf("No s'ha pogut obrir el fitxer de notificacions: %s\n", ruta_fitxer);
                    usuaris[i].num_notificacions = 0;
                    continue;
                }

                // Inicialitza el comptador de notificacions
                int num_notificacions = 0;
                char linia[512];
                char emissor[MAX_USUARI];
                char missatge[MAX_MISSATGE];

                // Llegeix cada línia del fitxer i emmagatzema les notificacions
                while (fgets(linia, sizeof(linia), fitxer) != NULL && num_notificacions < MAX_NOTIFICACIONS)
                {
                    // Parseja la línia en format "emissor;missatge"
                    if (sscanf(linia, "%[^;];%[^\n]", emissor, missatge) == 2)
                    {
                        strncpy(usuaris[i].notificacions[num_notificacions].emissor, emissor, MAX_USUARI);
                        strncpy(usuaris[i].notificacions[num_notificacions].receptor, usuaris[i].nom, MAX_USUARI);
                        strncpy(usuaris[i].notificacions[num_notificacions].missatge, missatge, MAX_MISSATGE);
                        num_notificacions++;
                    }
                }

                // Tanquem el fitxer
                fclose(fitxer);

                // Actualitzem el camp num_notificacions de l'usuari
                usuaris[i].num_notificacions = num_notificacions;
                usuaris_trobats++;
                printf("Usuari: %s, Notificacions emmagatzemades: %d\n", usuaris[i].nom, usuaris[i].num_notificacions);
            }
        }
    }

    // Tanquem el directori
    closedir(directori);

    printf("Usuaris trobats amb notificacions: %d de %d\n", usuaris_trobats, num_usuaris);
}

char *consultar_notificacions(char *nom)
{
    static char resultat[MIDA_PAQUET]; // Cadena per emmagatzemar el resultat (buffer estàtic)
    resultat[0] = '\0';                // Inicialitzar la cadena

    // Buscar l'usuari a la llista d'usuaris
    for (int i = 0; i < num_usuaris; i++)
    {
        if (strcmp(usuaris[i].nom, nom) == 0)
        {
            // Usuari trobat, recorrem les notificacions
            if (usuaris[i].num_notificacions == 0)
            {
                snprintf(resultat, sizeof(resultat), "No hi ha notificacions per l'usuari: %s\n", nom);
                return resultat;
            }

            // Afegir les notificacions a la cadena de resultat
            for (int j = 0; j < usuaris[i].num_notificacions; j++)
            {
                char notificacio[512]; // Incrementem la mida del buffer per evitar truncació
                snprintf(notificacio, sizeof(notificacio), "Emissor: %.50s, Missatge: %.200s\n",
                         usuaris[i].notificacions[j].emissor,
                         usuaris[i].notificacions[j].missatge);

                // Concatenar la notificació al resultat
                strncat(resultat, notificacio, sizeof(resultat) - strlen(resultat) - 1);
            }

            return resultat; // Retornar la cadena amb les notificacions
        }
    }

    // Si no es troba l'usuari
    snprintf(resultat, sizeof(resultat), "Usuari no trobat: %s\n", nom);
    return resultat;
}

void afegir_notificacio(const char *nom, const char *emissor, const char *missatge)
{
    // Construïm el nom del fitxer de notificacions per l'usuari
    char fitxer_notificacions[100];
    snprintf(fitxer_notificacions, sizeof(fitxer_notificacions), "data/notificacions/%s_not.txt", nom);

    // Obrim el fitxer en mode "a" (append), que afegeix sense eliminar contingut
    FILE *fitxer = fopen(fitxer_notificacions, "a");
    if (fitxer == NULL)
    {
        printf("Error en crear o obrir el fitxer de notificacions per l'usuari: %s\n", nom);
        return;
    }

    // Escriure la notificació al fitxer en format "emissor;missatge"
    fprintf(fitxer, "%s;%s\n", emissor, missatge);

    // Tanquem el fitxer
    fclose(fitxer);

    printf("Notificació afegida per a l'usuari: %s\n", nom);
}

void elimina_notificacions(char *nom)
{
    // Buscar l'usuari a la llista d'usuaris
    for (int i = 0; i < num_usuaris; i++)
    {
        if (strcmp(usuaris[i].nom, nom) == 0)
        {
            // Comprovem si l'usuari té notificacions
            if (usuaris[i].num_notificacions == 0)
            {
                printf("L'usuari %s no té notificacions per eliminar.\n", nom);
                return;
            }

            // L'usuari té notificacions, així que procedim a eliminar-les
            char fitxer_notificacions[100];
            snprintf(fitxer_notificacions, sizeof(fitxer_notificacions), "data/notificacions/%s_not.txt", nom);

            // Intentem eliminar el fitxer de notificacions
            if (unlink(fitxer_notificacions) == 0)
            {
                printf("Fitxer de notificacions eliminat per a l'usuari: %s\n", nom);
            }
            else
            {
                printf("No s'ha pogut eliminar el fitxer de notificacions per a l'usuari: %s\n", nom);
            }

            // Buidem l'estructura de notificacions de l'usuari
            for (int j = 0; j < MAX_NOTIFICACIONS; j++)
            {
                usuaris[i].notificacions[j].emissor[0] = '\0';
                usuaris[i].notificacions[j].receptor[0] = '\0';
                usuaris[i].notificacions[j].missatge[0] = '\0';
            }
            usuaris[i].num_notificacions = 0;

            printf("Les notificacions de l'usuari %s han estat buidades correctament.\n", nom);
            return;
        }
    }

    // Si l'usuari no es troba
    printf("No s'ha trobat cap usuari amb el nom: %s\n", nom);
}

int tens_notificacions(char *nom)
{
    // Buscar l'usuari a la llista d'usuaris
    for (int i = 0; i < num_usuaris; i++)
    {
         printf("Comparant '%s' amb '%s'\n", usuaris[i].nom, nom);
        if (strcmp(usuaris[i].nom, nom) == 0)
        {
            // Retornem el nombre de notificacions de l'usuari
            return usuaris[i].num_notificacions;
        }
    }

    // Si no es troba l'usuari, retornem -1
    printf("Usuari no trobat: %s\n", nom);
    return -1;
}


void gestiona_notificacions_servidor(char *paquet, int s, struct sockaddr_in contacte_client, int contacte_client_mida, Usuari_t *usuaris)
{
    char accio[20], receptor[50], missatge[MAX_MISSATGE], nom[MAX_USUARI];
    int codi_operacio;

    // Analitzar l'acció de notificacions (ex: "4 nom CONSULTAR_NOTIFICACIONS")
    if (sscanf(paquet, "%d %s %s", &codi_operacio, nom, accio) < 3)
    {
        printf("Error de format en gestió de notificacions.\n");
        return;
    }

    printf("Gestionant notificacions per a l'usuari %s, acció: %s\n", nom, accio);

    if (strcmp(accio, "CONSULTAR_NOTIFICACIONS") == 0)
    {
        // Consultar notificacions
        char *paquet = consultar_notificacions(nom);
        printf("Notificacions: %s\n", paquet);
        printf("Enviat a %s\n", nom);
        sendto(s, paquet, sizeof(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        printf("Notificacions enviades: %s\n", paquet);
    }
    else if (strcmp(accio, "ELIMINAR_NOTIFICACIONS") == 0)
    {
        // Eliminar notificacions i desar els canvis
        usuaris[0].num_notificacions = 0;
        elimina_notificacions(nom);
        sendto(s, "Notificacions eliminades", 24, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
    }
    else if (strcmp(accio, "ENVIAR_NOTIFICACIO") == 0)
    {
        // Analitzar el paquet per obtenir el receptor i el missatge
        if (sscanf(paquet, "%d %s ENVIAR_NOTIFICACIO %s %[^\n]", &codi_operacio, nom, receptor, missatge) == 4)
        {
            afegir_notificacio(receptor, nom, missatge);
            sendto(s, "Notificació enviada", 20, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        }
        else
        {
            printf("Error de format en enviar notificació.\n");
        }
    }
    else if (strcmp(accio, "SURT_DE_NOTIFICAIONS") == 0)
    {
        sendto(s, "Surto de notificacions", 20, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
    }
    else
    {
        printf("Acció de notificacions desconeguda: %s\n", accio);
    }
}
