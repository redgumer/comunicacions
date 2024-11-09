#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USUARIS 50
#define MAX_AMICS 500
#define FILE_AMISTATS "data/amistats.txt"

#include "fun_afegir_amic.h"

char amistats[MAX_USUARIS][MAX_AMICS];

// Funció per llegir les amistats des del fitxer
void llegirAmistats(const char *nom_fitxer){
    FILE *fitxer = fopen(nom_fitxer, "r");
    if (!fitxer)
    {
        printf("Error: No s'ha pogut obrir el fitxer d'amistats.\n");
        return;
    }

    char linea[1024];
    int id;
    char amics[MAX_AMICS];

    // Inicialitza l'array d'amistats
    for (int i = 0; i < MAX_USUARIS; i++)
    {
        amistats[i][0] = '\0';
    }

    // Llegeix cada línia d'amistats
    while (fgets(linea, sizeof(linea), fitxer))
    {
        // Salta línies de comentaris
        if (linea[0] == '#' || strlen(linea) < 2)
            continue;

        // Llegeix l'ID i la llista d'amics
        if (sscanf(linea, "%d; %[^\n]", &id, amics) == 2)
        {
            strcpy(amistats[id], amics);
        }
        else if (sscanf(linea, "%d;", &id) == 1)
        {
            amistats[id][0] = '\0'; // Usuari sense amics
        }
    }
    fclose(fitxer);
}

// Funció per comprovar si dos usuaris ja són amics
int jaSonAmics(int idUsuari, int idNouAmic)
{
    char idNouAmicStr[10];
    sprintf(idNouAmicStr, "%d", idNouAmic);

    // Comprova si l'ID del nou amic ja està a la llista
    return (strstr(amistats[idUsuari], idNouAmicStr) != NULL);
}

// Funció per afegir un amic a la llista d'un usuari
void afegirAmic(int idUsuari, int idNouAmic)
{
    if (!jaSonAmics(idUsuari, idNouAmic))
    {
        char idNouAmicStr[10];
        sprintf(idNouAmicStr, "%d", idNouAmic);

        // Afegir el nou amic a la llista
        if (strlen(amistats[idUsuari]) > 0)
        {
            strcat(amistats[idUsuari], " ");
        }
        strcat(amistats[idUsuari], idNouAmicStr);
    }
}

// Funció per guardar les amistats al fitxer
void guardarAmistats(const char *nom_fitxer)
{
    FILE *fitxer = fopen(nom_fitxer, "w");
    if (!fitxer)
    {
        printf("Error: No s'ha pogut obrir el fitxer d'amistats per escriptura.\n");
        return;
    }

    fprintf(fitxer, "# Llista d'amistats (ID usuari; ID usuari amic amic...)\n");
    for (int i = 1; i < MAX_USUARIS; i++)
    {
        if (strlen(amistats[i]) > 0)
        {
            fprintf(fitxer, "%d; %s\n", i, amistats[i]);
        }
        else
        {
            fprintf(fitxer, "%d;\n", i);
        }
    }

    fclose(fitxer);
    printf("Amistats guardades correctament.\n");
}

// Funció de comparació per a qsort
int comparar(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Funció per ordenar la llista d'amics de cada usuari
void ordenarAmistats()
{
    for (int i = 1; i < MAX_USUARIS; i++)
    {
        if (strlen(amistats[i]) == 0)
            continue;

        // Convertir la cadena d'amics a un array d'enters
        int amics[MAX_AMICS];
        int numAmics = 0;
        char *token = strtok(amistats[i], " ");
        while (token != NULL)
        {
            amics[numAmics++] = atoi(token);
            token = strtok(NULL, " ");
        }

        // Ordenar l'array d'enters
        qsort(amics, numAmics, sizeof(int), comparar);

        // Reconstruir la cadena ordenada
        char llistaOrdenada[MAX_AMICS] = "";
        char amicStr[10];
        for (int j = 0; j < numAmics; j++)
        {
            sprintf(amicStr, "%d", amics[j]);
            strcat(llistaOrdenada, amicStr);
            if (j < numAmics - 1)
                strcat(llistaOrdenada, " ");
        }

        // Actualitzar la llista d'amics ordenada
        strcpy(amistats[i], llistaOrdenada);
    }
}

