/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.h                                              |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#ifndef FUNCIONS_SERVIDOR_H
#define FUNCIONS_SERVIDOR_H

#include <arpa/inet.h>
typedef struct {
    int id;
    char nom[50];
    char contrasenya[50];
    char sexe[10];
    char estat_civil[20];
    int edat;
    char ciutat[50];
    char descripcio[100];
} Usuari;

int verifica_usuari(const char *nom, const char *contrasenya);
int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio);
int obtenirIdUsuari(const char *nomUsuari);
char *veureAmics(const char *nomUsuari);
void processa_opcio_menu(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, int opcio, const char *nom);
void processa_peticio(int s, struct sockaddr_in contacte_client, socklen_t contacte_client_mida, char *paquet);

#endif
