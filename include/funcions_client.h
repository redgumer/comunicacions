/*
 ================================ PROJECTE XARXES ================================
<<<<<<< HEAD
 | Fitxer     : funcions_servidor.h                                              |
=======
 | Fitxer     : funcions_client.h                                                |
>>>>>>> 7a45e98ae33727134f6a194d7d1aea31be3f7b85
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

<<<<<<< HEAD
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
=======
/**
 * @brief Demana les credencials de l'usuari (nom i contrasenya)
 *
 * @param nom Punter a char per emmagatzemar el nom de l'usuari
 * @param contrasenya Punter a char per emmagatzemar la contrasenya de l'usuari
 
 */
void demana_credencials(char *nom, char *contrasenya);


/**
 * @brief Mostra el menú d'opcions i retorna l'opció seleccionada per l'usuari
 *
 * @return int Retorna l'opció seleccionada per l'usuari
 */
int mostra_menu();

int inicializar_cliente(const char *ip, int port, struct sockaddr_in *contacte_servidor);
int enviar_paquete(int s, struct sockaddr_in *contacte_servidor, char *paquet);
void demana_credencials(char *nom, char *contrasenya);
void procesa_respuesta_servidor(int s, struct sockaddr_in *contacte_servidor);

#endif
>>>>>>> 7a45e98ae33727134f6a194d7d1aea31be3f7b85
