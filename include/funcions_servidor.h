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

int verifica_usuari(const char *nom, const char *contrasenya);
int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio);
int obtenirIdUsuari(const char *nomUsuari);
char *veureAmics(const char *nomUsuari);
char *obtenirNomUsuari(int idUsuari);
void processa_opcio_menu(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, int opcio, const char *nom, const char *nouAmic);
void processa_peticio(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, char *paquet);
void registra_activitat(const char *tipus, const char *missatge);

#endif
