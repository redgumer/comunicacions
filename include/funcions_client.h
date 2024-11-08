#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

#include <netinet/in.h> // Necessari per a la definició de sockaddr_in

// ================================ CONSTANTS ================================ //
#define MIDA_PAQUET 1500
#define MAX_USUARI 30
#define MAX_CONTRASENYA 30
#define MAX_SEXE 10
#define MAX_ESTAT_CIVIL 15
#define MAX_CIUTAT 20
#define MAX_DESC 100

// ================================ FUNCIONS ================================ //

/**
 * @brief Inicialitza la connexió amb el servidor
 *
 * @param contacte_servidor Punter a la struct sockaddr_in per a la configuració del servidor
 * @param ip Adreça IP del servidor
 * @param port Port del servidor
 * @return int Retorna el descriptor de socket creat
 */
int inicialitza_connexio(struct sockaddr_in *contacte_servidor, const char *ip, const char *port);

/**
 * @brief Verifica les credencials de l'usuari amb el servidor
 *
 * @param s Descriptor de socket per a la connexió
 * @param contacte_servidor Estructura sockaddr_in amb la informació del servidor
 * @param nom Nom d'usuari a verificar
 * @return int Retorna 1 si l'usuari és verificat, 0 si no ho és
 */
int verifica_usuari(int s, struct sockaddr_in contacte_servidor, char *nom);

/**
 * @brief Gestiona el menú d'opcions del client
 *
 * @param s Descriptor de socket per a la connexió
 * @param contacte_servidor Estructura sockaddr_in amb la informació del servidor
 */
void gestiona_menu(int s, struct sockaddr_in contacte_servidor, char nom[MAX_USUARI]);

/**
 * @brief Demana les credencials a l'usuari
 *
 * @param nom Cadena on s'emmagatzema el nom d'usuari introduït
 * @param contrasenya Cadena on s'emmagatzema la contrasenya introduïda
 */
void demana_credencials(char *nom, char *contrasenya);

/**
 * @brief Mostra el menú d'opcions i retorna l'opció seleccionada
 *
 * @return int Retorna el número de l'opció seleccionada
 */
int mostra_menu();
/**
 * @brief Registra un usuari al servidor
 * @param nom Nom de l'usuari
 * @param contrasenya Contrasenya de l'usuari
 * @param sexe Sexe de l'usuari
 * @param estat_civil Estat civil de l'usuari
 * @param edat Edat de l'usuari
 * @param ciutat Ciutat de l'usuari
 * @param descripcio Descripció de l'usuari
 */
void registre_usuari(char *nom, char *contrasenya, char *sexe, char *estat_civil, int *edat, char *ciutat, char *descripcio);

#endif // FUNCIONS_CLIENT_H
