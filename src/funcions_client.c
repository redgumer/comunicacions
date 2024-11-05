/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_client.c                                                |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#include "../include/funcions_client.h"
#include <stdio.h>
#include <string.h>

#define MAX_NOM 50
#define MAX_CONTRASENYA 50

void demana_credencials(char *nom, char *contrasenya)
{
    printf("Introdueix el teu nom: ");
    scanf("%49s", nom); // Limita la longitud para evitar desbordamientos

    printf("Introdueix la teva contrasenya: ");
    scanf("%49s", contrasenya); // Limita la longitud para la contrasenya

    // Opcional: Verificar si la longitud de los datos es la esperada
    if (strlen(nom) >= MAX_NOM || strlen(contrasenya) >= MAX_CONTRASENYA)
    {
        printf("Error: El nom o la contrasenya superen la longitud màxima permesa.\n");
    }
}

int mostra_menu(int tipus_menu)
{
    int opcio;

    if (tipus_menu == 1)
    { // Menú principal d'usuari
        printf("\nMenu d'usuari:\n");
        printf("1. Veure perfil\n");
        printf("2. Veure els meus amics\n");
        printf("3. Afegir amics nous\n");
        printf("4. Consultar l'activitat de l'usuari\n");
        printf("5. Tancar el programa\n");
    }
    else if (tipus_menu == 2)
    { // Exemple de menú d'administrador
        printf("\nMenu d'administrador:\n");
        printf("1. Gestionar usuaris\n");
        printf("2. Veure totes les activitats\n");
        printf("3. Consultar estadístiques\n");
        printf("4. Tancar sessió\n");
    }
    else if (tipus_menu == 3)
    { // Menú per suggeriments d'amics
        printf("\nMenu de suggeriments d'amics:\n");
        printf("1. Veure suggeriments d'amics\n");
        printf("2. Afegir amic suggerit\n");
        printf("3. Tornar al menú principal\n");
    }

    printf("Selecciona una opció: ");
    scanf("%d", &opcio);
    return opcio;
}
