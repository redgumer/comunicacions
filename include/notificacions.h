/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.h                                              |
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

void afegir_notificacio(Usuari_t *usuari, const char *emissor, const char *missatge);
void emmagatzema_notificacions(Usuari_t *usuari);
void carrega_notificacions(Usuari_t *usuari);
void consultar_notificacions(Usuari_t *usuari);
int tens_notificacions(char *nom);
void elimina_notificacions(Usuari_t *usuari);
void gestiona_notificacions_servidor(char *paquet, int s, struct sockaddr_in contacte_client, int contacte_client_mida, Usuari_t *usuaris);

#endif