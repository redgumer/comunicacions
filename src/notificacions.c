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
#include <stdio.h>       // Para operaciones de entrada/salida
#include <string.h>      // Para manipulación de cadenas de texto

// Inclusión de librerías personalizadas
#include "funcions_servidor.h" // Archivo de cabecera con funciones específicas para el servidor

// Definición de constantes
#define MAX_MISSATGE 256                   // Tamaño máximo para un mensaje de notificación
#define MAX_NOTIFICACIONS 10               // Número máximo de notificaciones almacenadas por usuario
#define FITXER_NOTIFICACIONS "data/notificacions.txt" // Ruta del archivo de notificaciones
#define MAX_USUARI 50                      // Número máximo de caracteres para un nombre de usuario


// Funció per afegir una notificació a la bandeja d'entrada
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

// Funció per emmagatzemar notificacions al fitxer
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

// Funció per carregar notificacions des del fitxer
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

// Funció per consultar notificacions pendents
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

// Funció per saber si hi ha notificacions pendents
int tens_notificacions(Usuari_t *usuari) {
    return usuari->num_notificacions > 0;
}