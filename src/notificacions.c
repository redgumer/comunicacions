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
#include <stdio.h>                                      // Para operaciones de entrada/salida
#include <string.h>                                     // Para manipulación de cadenas de texto

// Inclusión de librerías personalizadas
#include "funcions_servidor.h"                          // Archivo de cabecera con funciones específicas para el servidor

// Definición de constantes
#define MAX_MISSATGE 256                                // Tamaño máximo para un mensaje de notificación
#define MAX_NOTIFICACIONS 10                            // Número máximo de notificaciones almacenadas por usuario
#define FITXER_NOTIFICACIONS "data/notificacions.txt"   // Ruta del archivo de notificaciones
#define MAX_USUARI 50                                   // Número máximo de caracteres para un nombre de usuario


void afegir_notificacio(Usuari_t *usuari, const char *emissor, const char *missatge) {
    if (usuari->num_notificacions >= MAX_NOTIFICACIONS) {
        printf("No es poden afegir més notificacions. Bandeja plena!\n");
        return;
    }
    strcpy(usuari->notificacions[usuari->num_notificacions].emissor, emissor);
    strcpy(usuari->notificacions[usuari->num_notificacions].receptor, usuari->nom);
    strcpy(usuari->notificacions[usuari->num_notificacions].missatge, missatge);
    usuari->num_notificacions++;
}

void emmagatzema_notificacions(Usuari_t *usuari) {
    FILE *fitxer = fopen(FITXER_NOTIFICACIONS, "w");
    if (!fitxer) {
        perror("Error obrint el fitxer de notificacions");
        return;
    }
    for (int i = 0; i < usuari->num_notificacions; i++) {
        fprintf(fitxer, "%s;%s;%s\n",
                usuari->notificacions[i].emissor,
                usuari->notificacions[i].receptor,
                usuari->notificacions[i].missatge);
    }
    fclose(fitxer);
}

void carrega_notificacions(Usuari_t *usuari) {
    FILE *fitxer = fopen(FITXER_NOTIFICACIONS, "r");
    if (!fitxer) {
        perror("Error obrint el fitxer de notificacions");
        return;
    }
    char linia[MAX_MISSATGE + MAX_USUARI * 2];
    usuari->num_notificacions = 0;
    while (fgets(linia, sizeof(linia), fitxer) && usuari->num_notificacions < MAX_NOTIFICACIONS) {
        sscanf(linia, "%[^;];%[^;];%[^\n]", 
               usuari->notificacions[usuari->num_notificacions].emissor,
               usuari->notificacions[usuari->num_notificacions].receptor,
               usuari->notificacions[usuari->num_notificacions].missatge);
        usuari->num_notificacions++;
    }
    fclose(fitxer);
}

void consultar_notificacions(Usuari_t *usuari) {
    if (usuari->num_notificacions == 0) {
        printf("No tens notificacions pendents.\n");
        return;
    }
    for (int i = 0; i < usuari->num_notificacions; i++) {
        printf("De: %s - Missatge: %s\n",
               usuari->notificacions[i].emissor,
               usuari->notificacions[i].missatge);
    }
}

int tens_notificacions(Usuari_t *usuari) {
    return usuari->num_notificacions > 0;
}

void elimina_notificacions(Usuari_t *usuari) {
    // Esborrar les notificacions de l'usuari
    usuari->num_notificacions = 0;
    memset(usuari->notificacions, 0, sizeof(usuari->notificacions));

    // Desa els canvis a l'arxiu (igual que a emmagatzema_notificacions)
    FILE *fitxer = fopen("notificacions.dat", "wb");
    if (fitxer == NULL) {
        perror("Error obrint fitxer de notificacions");
        return;
    }

    // Escriure l'usuari actualitzat al fitxer
    if (fwrite(usuari, sizeof(Usuari_t), 1, fitxer) != 1) {
        perror("Error esborrant notificacions al fitxer");
    }

    fclose(fitxer);
    printf("Totes les notificacions han estat eliminades correctament.\n");
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
        consultar_notificacions(&usuaris[0]);
        sendto(s, usuaris[0].notificacions, sizeof(usuaris[0].notificacions), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
    }
    else if (strcmp(accio, "ELIMINAR_NOTIFICACIONS") == 0)
    {
        // Eliminar notificacions i desar els canvis
        usuaris[0].num_notificacions = 0;
        emmagatzema_notificacions(&usuaris[0]);
        sendto(s, "Notificacions eliminades", 24, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
    }
    else if (strcmp(accio, "ENVIAR_NOTIFICACIO") == 0)
    {
        // Analitzar el paquet per obtenir el receptor i el missatge
        if (sscanf(paquet, "%d %s ENVIAR_NOTIFICACIO %s %[^\n]", &codi_operacio, nom, receptor, missatge) == 4)
        {
            afegir_notificacio(&usuaris[0], nom, missatge);
            emmagatzema_notificacions(&usuaris[0]);
            sendto(s, "Notificació enviada", 20, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        }
        else
        {
            printf("Error de format en enviar notificació.\n");
        }
    }
    else
    {
        printf("Acció de notificacions desconeguda: %s\n", accio);
    }
}

