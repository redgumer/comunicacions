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
#include <netinet/in.h>
#include <unistd.h>
#define MAX_USUARI 30
#define MAX_CONTRASENYA 30
#define MAX_SEXE 10
#define MAX_ESTAT_CIVIL 15
#define MAX_CIUTAT 20
#define MAX_DESC 100

/**
 * @brief Estructura que representa un usuari amb els seus atributs bàsics
 */
typedef struct {
    char nom[MAX_USUARI];          /**< Nom de l'usuari */
    char contrasenya[MAX_CONTRASENYA]; /**< Contrasenya de l'usuari */
    char sexe[MAX_SEXE];            /**< Sexe de l'usuari */
    char estat_civil[MAX_ESTAT_CIVIL]; /**< Estat civil de l'usuari */
    int edat;                       /**< Edat de l'usuari */
    char ciutat[MAX_CIUTAT];        /**< Ciutat de residència de l'usuari */
    char descripcio[MAX_DESC];      /**< Descripció personal de l'usuari */
} usuari_t;

/**
 * @brief Verifica si un usuari existeix amb el nom i contrasenya proporcionats
 *
 * @param nom Nom de l'usuari a verificar
 * @param contrasenya Contrasenya de l'usuari a verificar
 * @return int Retorna 1 si existeix, 0 si no i -1 si hi ha un error amb l'arxiu
 */
int verifica_usuari(const char *nom, const char *contrasenya);

/**
 * @brief Registra un nou usuari amb la informació especificada
 *
 * @param nom Nom de l'usuari
 * @param contrasenya Contrasenya de l'usuari
 * @param sexe Sexe de l'usuari
 * @param estat_civil Estat civil de l'usuari
 * @param edat Edat de l'usuari
 * @param ciutat Ciutat de residència de l'usuari
 * @param descripcio Descripció personal de l'usuari
 * @return int Retorna 1 si el registre és correcte i -1 si hi ha un error
 */
int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, 
                    const char *estat_civil, int edat, const char *ciutat, 
                    const char *descripcio);

/**
 * @brief Retorna una descripció detallada del perfil de l'usuari
 *
 * @param usuari Punter a estructura usuari_t amb la informació de l'usuari
 * @return const char* Cadena amb la informació formatada del perfil de l'usuari
 */
const char* veure_perfil(const usuari_t *usuari);

/**
 * @brief Retorna una llista dels amics de l'usuari especificat
 *
 * @param nom Nom de l'usuari
 * @return const char* Cadena amb els noms dels amics de l'usuari
 */
const char* veure_amics(const char *nom);

/**
 * @brief Afegeix un nou amic per a l'usuari especificat
 *
 * @param nom Nom de l'usuari que vol afegir un amic
 * @param nou_amic Nom del nou amic a afegir
 * @return const char* Missatge indicant si l'amistat s'ha afegit correctament
 */
const char* afegir_amic(const char *nom, const char *nou_amic);

/**
 * @brief Obté la informació d'un usuari basat en el seu nom
 *
 * @param nom Nom de l'usuari a buscar
 * @return usuari_t Retorna una estructura usuari_t amb la informació de l'usuari
 */
usuari_t get_user_info(const char *nom);

/**
 * @brief Processa l'opció seleccionada pel client i retorna una resposta.
 *
 * @param opcio Enter que indica l'opció seleccionada pel client
 * @param nom Nom de l'usuari que ha seleccionat l'opció
 * @param continuar Punter a enter per modificar l'estat de continuació del bucle
 * @return const char* Retorna la resposta a enviar al client
 */
const char* processa_opcio(int opcio, const char *nom, int *continuar);

/**
 * @brief Gestiona la sessió d'un client verificant credencials i responent a les opcions del menú.
 *
 * @param s Socket del servidor
 * @param contacte_client Informació de l'adreça del client
 */
void gestiona_client(int s, struct sockaddr_in contacte_client);

/**
 * @brief Configura el socket del servidor i el lliga al port especificat.
 *
 * @param port Enter que representa el port del servidor
 * @return int Retorna el descriptor de fitxer del socket o -1 si hi ha un error
 */
int configura_socket(int port);

#endif
