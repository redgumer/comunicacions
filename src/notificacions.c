

#include <stdio.h>
#include <string.h>
#include "funcions_servidor.h"


#define MAX_MISSATGE 256
#define MAX_NOTIFICACIONS 10
#define FITXER_NOTIFICACIONS "data/notificacions.txt"
#define MAX_USUARI 50  // Define MAX_USUARI with an appropriate value

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