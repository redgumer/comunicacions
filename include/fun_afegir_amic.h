/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : include/fun_afegir_amic.h                                        |
 | Autors     : Xavi                                                             |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Pràctica Xarxes Segon de GEI, Comunicacions Servidor&Client      |
 =================================================================================
*/

#ifndef FUN_AFEGIR_AMIC_H
#define FUN_AFEGIR_AMIC_H

void llegirAmistats(const char *nom_fitxer);
int jaSonAmics(int idUsuari, int idNouAmic);
void afegirAmic(int idUsuari, int idNouAmic);
void guardarAmistats(const char *nom_fitxer);
int comparar(const void *a, const void *b);
void ordenarAmistats();

#endif