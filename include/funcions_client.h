#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

#include <netinet/in.h> // Necessari per a la definició de sockaddr_in

// ================================ CONSTANTS ================================ //
#define MIDA_PAQUET 100
#define MAX_USUARI 30
#define MAX_CONTRASENYA 30

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
 * @return int Retorna 1 si l'usuari és verificat, 0 si no ho és
 */
int verifica_usuari(int s, struct sockaddr_in contacte_servidor);

/**
 * @brief Gestiona el menú d'opcions del client
 *
 * @param s Descriptor de socket per a la connexió
 * @param contacte_servidor Estructura sockaddr_in amb la informació del servidor
 */
void gestiona_menu(int s, struct sockaddr_in contacte_servidor);

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

#endif // FUNCIONS_CLIENT_H
