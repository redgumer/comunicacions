/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.h                                              |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Capçalera de les funcions del servidor per a la gestió           |
 |              d'usuaris i relacions d'amistat.                                 |
 =================================================================================
*/

#ifndef FUNCIONS_SERVIDOR_H
#define FUNCIONS_SERVIDOR_H

// Llibreries bàsiques
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"

// Llibreries de xarxa
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Constants
#define USER_FILE "users.txt"
#define MAX_USUARI 100
#define MAX_CONTRASENYA 100
#define MAX_AMICS 50
#define MAX_SUGGERIMENTS 5
#define MIDA_PAQUET 1500

// Estructura per a informació de l'usuari
typedef struct {
    char nom[MAX_USUARI];
    char contrasenya[MAX_CONTRASENYA];
    char sexe[10];
    char estat_civil[20];
    int edat;
    char ciutat[50];
    char descripcio[150];
} usuari_t;

// Estructura per emmagatzemar l'activitat de l'usuari
typedef struct {
    char tipus_activitat[30];
    char detalls[100];
    time_t timestamp;
} Activitat;

// Funcions d'autenticació i registre
int verifica_usuari(const char *nom, const char *contrasenya);
int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio);

// Funcions per a la gestió de perfils i amistats
const char *veure_perfil(const usuari_t *usuari);
const char *afegir_amic(const char *nom, const char *nou_amic);
const char *veure_amics(const char *nom);
const char *suggerir_amics(const char *nom);

// Funcions per a la gestió de la connexió
void gestiona_client(int s, struct sockaddr_in contacte_client);
int configura_socket(int port);

// Funcions d'activitat
void registre_activitat(const char *usuari, const char *accio, const char *detall);
char *veure_activitat(const char *nom);

// Funcions auxiliars per a la gestió de JSON
cJSON *cargar_json(const char *filename);
int guardar_json(const char *filename, cJSON *json);

// Funció per obtenir informació d'un usuari
usuari_t get_user_info(const char *nom);

// Funció per processar opcions de menú
const char *processa_opcio(int opcio, const char *nom, int *continuar);

#endif // FUNCIONS_SERVIDOR_H
