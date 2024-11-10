/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.h                                              |
 | Autors     : Xavi, Jadi, Ivette                                               |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

#ifndef FUNCIONS_SERVIDOR_H
#define FUNCIONS_SERVIDOR_H

#define MAX_USUARI 50
#define MAX_MISSATGE 256
#define MAX_NOTIFICACIONS 10

#include <arpa/inet.h>
typedef struct {
    char emissor[MAX_USUARI];
    char receptor[MAX_USUARI];
    char missatge[MAX_MISSATGE];
} Notificacio_t;

typedef struct {
    int id;
    char nom[50];
    char contrasenya[50];
    char sexe[10];
    char estat_civil[20];
    int edat;
    char ciutat[50];
    char descripcio[100];
    Notificacio_t notificacions[MAX_NOTIFICACIONS];
    int num_notificacions;
} Usuari_t;

void afegir_notificacio(Usuari_t *usuari, const char *emissor, const char *missatge);
void emmagatzema_notificacions(Usuari_t *usuari);
void carrega_notificacions(Usuari_t *usuari);
void consultar_notificacions(Usuari_t *usuari);
int tens_notificacions(Usuari_t *usuari);

#endif