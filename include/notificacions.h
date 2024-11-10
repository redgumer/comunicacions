/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : include/notificacions.h                                          |
 | Autors     : Xavi, Jadi, Ivette                                               |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

#ifndef NOTIFICACIONS_H
#define NOTIFICACIONS_H

#include "tipus.h"
#include <arpa/inet.h>
void carrega_notificacions(Usuari_t *usuari);
char *consultar_notificacions(char *nom);
void afegir_notificacio(const char *nom, const char *emissor, const char *missatge);
void elimina_notificacions(char *nom);
int tens_notificacions(char *nom);
void gestiona_notificacions_servidor(char *paquet, int s, struct sockaddr_in contacte_client, int contacte_client_mida, Usuari_t *usuaris);

#endif