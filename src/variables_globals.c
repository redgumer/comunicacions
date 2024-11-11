/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : src/variables_globals.c                                          |
 | Autors     : Xavi,                                                            |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

#include "tipus.h"
#include "variables_globals.h"

Usuari_t usuaris[MAX_USUARI]; // Inicialització de la variable global
int num_usuaris = 0;          // Inicialització de variables globals
int id_max = 0;
