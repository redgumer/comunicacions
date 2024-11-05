/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_client.h                                                |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

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

#endif