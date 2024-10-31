#ifndef FUNCIONS_CLIENT_H
#define FUNCIONS_CLIENT_H

/**
 * @brief Demana les credencials de l'usuari (nom i contrasenya)
 *
 * @param nom Punter a char per emmagatzemar el nom de l'usuari
 * @param contrasenya Punter a char per emmagatzemar la contrasenya de l'usuari
 * @param max_length Longitud màxima per a nom i contrasenya
 */
void demana_credencials(char *nom, char *contrasenya, int max_length);

/**
 * @brief Mostra el menú d'opcions i retorna l'opció seleccionada per l'usuari
 *
 * @return int Retorna l'opció seleccionada per l'usuari
 */
int mostra_menu();

#endif