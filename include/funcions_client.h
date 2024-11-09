
#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

#include <arpa/inet.h>

void mostra_menu();
int envia_paquet(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, const char *paquet);
int rep_paquet(int s, char *paquet, struct sockaddr_in *contacte_servidor, socklen_t *contacte_servidor_mida);
int inicia_sessio(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom);
int registra_usuari(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom, const char *contrasenya, char *sexe, char *estat_civil, int edat, char *ciutat, char *descripcio);

#endif
