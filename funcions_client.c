#include "funcions_client.h"
#include <stdio.h>

void demana_credencials(char *nom, char *contrasenya, int max_length)
{
    printf("Introdueix el teu nom: ");
    scanf("%s", nom);
    printf("Introdueix la teva contrasenya: ");
    scanf("%s", contrasenya);
}

int mostra_menu()
{
    int opcio;
    printf("\nMenu d'usuari:\n");
    printf("1. Veure perfil\n");
    printf("2. Veure els meus amics\n");
    printf("3. Afegir amics nous\n");
    printf("4. Tancar el programa\n");
    printf("Selecciona una opció: \n");
    scanf("%d", &opcio);
    return opcio;
}
