
#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

#include <arpa/inet.h>

void mostra_menu();
void notificacions_menu();
int envia_paquet(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, const char *paquet);
int rep_paquet(int s, char *paquet, struct sockaddr_in *contacte_servidor, socklen_t *contacte_servidor_mida);
int inicia_sessio(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom);
int registra(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio);
void registra_activitat(const char *tipus, const char *missatge);

void processar_opcio(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom);
void registra_nou_usuari(int s, struct sockaddr_in *contacte_servidor, socklen_t contacte_servidor_mida, char *nom);
void finalitza_sessio(int s, char *nom);

int inicialitza_socket(struct sockaddr_in *contacte_servidor, char *ip, char *port);
void mostra_error_inici_sessio(char *nom);

#endif
