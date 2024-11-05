#ifndef FUNCIONS_SERVIDOR_H
#define FUNCIONS_SERVIDOR_H

#include <time.h>

// Tamaño máximo de usuario y contraseña
#define MAX_USUARI 50
#define MAX_CONTRASENYA 50

// Estructura para la información del usuario
typedef struct {
    char nom[MAX_USUARI];
    char contrasenya[MAX_CONTRASENYA];
    char sexe[10];
    char estat_civil[20];
    int edat;
    char ciutat[50];
    char descripcio[300];
} usuari_t;

// Prototipos de funciones
int verifica_usuari(const char *nom, const char *contrasenya);
int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio);
const char *veure_perfil(const usuari_t *usuari);
const char *veure_amics(const char *nom);
const char *afegir_amic(const char *nom, const char *nou_amic);
const char *processa_opcio(int opcio, const char *nom, int *continuar);
void gestiona_client(int s, struct sockaddr_in contacte_client);
int configura_socket(int port);
void registre_activitat(const char* usuari, const char* accio, const char* detall);
char *veure_activitat(const char *nom);
usuari_t get_user_info(const char *nom);

#endif // FUNCIONS_SERVIDOR_H