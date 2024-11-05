/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.c                                              |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

//Llibreries bàsiques
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Llibreries de xarxa
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
//Fitxer de funcions del servidor
#include "funcions_servidor.h"

#define MIDA_PAQUET 100
#define USER_FILE "usuaris.txt"
#define FRIEND_FILE "amistats.txt"

int verifica_usuari(const char *nom, const char *contrasenya) {
    FILE *file = fopen(USER_FILE, "r");
    if (!file) return -1;

    usuari_t u;
    while (fscanf(file, "%s %s", u.nom, u.contrasenya) == 2) {
        if (strcmp(u.nom, nom) == 0 && strcmp(u.contrasenya, contrasenya) == 0) {
            fclose(file);
            
            // Registra activitat de verificació (login)
            registre_activitat(&u, "Inici de sessió", "Usuari autenticat correctament");
            
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio) {
    FILE *file = fopen(USER_FILE, "a");
    if (!file) return -1;

    fprintf(file, "%s %s %s %s %d %s %s\n", nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);
    fclose(file);

    // Registra l'activitat de registre d'un nou usuari
    usuari_t nou_usuari;  // Assumeix que s'ha inicialitzat amb els valors corresponents si cal
    strncpy(nou_usuari.nom, nom, sizeof(nou_usuari.nom) - 1);
    registre_activitat(&nou_usuari, "Registre d'usuari", "Nou usuari registrat");

    return 1;
}

const char* veure_perfil(const usuari_t *usuari) {
    static char resposta[300];
    snprintf(resposta, sizeof(resposta),
        "Perfil de l'usuari:\n"
        "Nom: %s\n"
        "Sexe: %s\n"
        "Estat Civil: %s\n"
        "Edat: %d\n"
        "Ciutat: %s\n"
        "Descripció: %s\n",
        usuari->nom, usuari->sexe, usuari->estat_civil, usuari->edat, usuari->ciutat, usuari->descripcio
    );
    return resposta;
}

const char* veure_amics(const char *nom) {
    static char resposta[500];
    resposta[0] = '\0';

    FILE *friend_file = fopen(FRIEND_FILE, "r");
    if (!friend_file) {
        snprintf(resposta, sizeof(resposta), "Error: No es pot obrir el fitxer de relacions d'amistat.\n");
        return resposta;
    }

    FILE *user_file;
    char friend_name[MAX_USUARI];
    char line[100];

    while (fgets(line, sizeof(line), friend_file)) {
        char user1[MAX_USUARI], user2[MAX_USUARI];
        sscanf(line, "%s %s", user1, user2);
        
        if (strcmp(nom, user1) == 0 || strcmp(nom, user2) == 0) {
            strcpy(friend_name, (strcmp(nom, user1) == 0) ? user2 : user1);

            user_file = fopen(USER_FILE, "r");
            if (!user_file) {
                snprintf(resposta, sizeof(resposta), "Error: No es pot obrir el fitxer d'usuaris.\n");
                fclose(friend_file);
                return resposta;
            }

            usuari_t friend;
            while (fscanf(user_file, "%s %s %s %s %d %s %[^\n]", friend.nom, friend.contrasenya, friend.sexe, friend.estat_civil, &friend.edat, friend.ciutat, friend.descripcio) == 7) {
                if (strcmp(friend.nom, friend_name) == 0) {
                    snprintf(resposta + strlen(resposta), sizeof(resposta) - strlen(resposta),
                        "Amic: %s\nSexe: %s\nEstat Civil: %s\nEdat: %d\nCiutat: %s\nDescripció: %s\n\n",
                        friend.nom, friend.sexe, friend.estat_civil, friend.edat, friend.ciutat, friend.descripcio);
                    break;
                }
            }
            fclose(user_file);
        }
    }

    fclose(friend_file);
    return resposta[0] ? resposta : "No tens amics registrats.\n";
}

const char* afegir_amic(const char *nom, const char *nou_amic) {
    static char resposta[100];
    FILE *file = fopen(FRIEND_FILE, "r+");  // Abrimos el archivo para lectura y escritura
    if (!file) {
        snprintf(resposta, sizeof(resposta), "Error: No se puede abrir el archivo de relaciones de amistad.\n");
        return resposta;
    }

    char line[100];
    int exists = 0;

    // Comprobar si la amistad ya existe en cualquier dirección
    while (fgets(line, sizeof(line), file)) {
        char user1[MAX_USUARI], user2[MAX_USUARI];
        sscanf(line, "%s %s", user1, user2);
        if ((strcmp(user1, nom) == 0 && strcmp(user2, nou_amic) == 0) ||
            (strcmp(user1, nou_amic) == 0 && strcmp(user2, nom) == 0)) {
            exists = 1;
            break;
        }
    }

    if (!exists) {
        // Agregar la amistad en ambas direcciones
        fprintf(file, "%s %s\n", nom, nou_amic);
        fprintf(file, "%s %s\n", nou_amic, nom);
        snprintf(resposta, sizeof(resposta), "Amistad creada entre %s y %s\n", nom, nou_amic);
    } else {
        snprintf(resposta, sizeof(resposta), "La relación de amistad ya existe entre %s y %s\n", nom, nou_amic);
    }

    fclose(file);
    return resposta;
}

usuari_t get_user_info(const char *nom) {
    FILE *file = fopen(USER_FILE, "r");
    usuari_t usuari;
    while (fscanf(file, "%s %s %s %s %d %s %[^\n]", 
                  usuari.nom, usuari.contrasenya, usuari.sexe, 
                  usuari.estat_civil, &usuari.edat, usuari.ciutat, 
                  usuari.descripcio) == 7) {
        if (strcmp(usuari.nom, nom) == 0) {
            fclose(file);
            return usuari;
        }
    }
    fclose(file);
    usuari.nom[0] = '\0'; // Indica que el usuario no fue encontrado
    return usuari;
}

const char* processa_opcio(int opcio, const char *nom, int *continuar) {
    const char *resposta;
    usuari_t user;
    switch (opcio) {
        case 1:  // Veure perfil
            user = get_user_info(nom);
            resposta = (user.nom[0] != '\0') ? veure_perfil(&user) : "Usuari no trobat\n";
            break;
        case 2:  // Veure els meus amics
            resposta = veure_amics(nom);
            break;
        case 3:  // Afegir amics nous
            resposta = afegir_amic(nom, "nou_amic");
            break;
        case 4:  // Tancar el programa
            resposta = veure_activitat();
            *continuar = 0;
            break;
        case 5:  // Consultar l'activitat de l'usuari
            resposta = "Sortint del programa...";
            break;
        default:
            resposta = "Opció invàlida.";
    }
    return resposta;
}


void gestiona_client(int s, struct sockaddr_in contacte_client) {
    socklen_t contacte_client_mida = sizeof(contacte_client);
    char paquet[MIDA_PAQUET];

    // Rep credencials del client
    recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
    char nom[MAX_USUARI], contrasenya[MAX_CONTRASENYA];
    sscanf(paquet, "%s %s", nom, contrasenya);

    // Verifica les credencials d'usuari
    int resultat = verifica_usuari(nom, contrasenya);
    if (resultat == 1) {
        // Envia resposta de verificació correcta
        strcpy(paquet, "Usuari verificat\n");
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        int continuar = 1;
        while (continuar) {
            // Rep l'opció seleccionada pel client
            recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
            int opcio;
            sscanf(paquet, "%d %s", &opcio, nom);
            const char *resposta = processa_opcio(opcio, nom, &continuar);
            sendto(s, resposta, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
        }
    } else {
        // Respon que l'usuari no ha estat trobat o verificat
        strcpy(paquet, "Usuari no trobat o error de verificació\n");
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
    }
}

int configura_socket(int port) {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in socket_servidor;
    socket_servidor.sin_family = AF_INET;
    socket_servidor.sin_addr.s_addr = INADDR_ANY;
    socket_servidor.sin_port = htons(port);

    // Enllaça el socket al port especificat
    if (bind(s, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor)) < 0) {
        printf("No s'ha pogut enllaçar el socket\n");
        return -1;
    }
    printf("Servidor operatiu al port %d!\n", port);
    return s;
}

// ============================= HISTORIAL D'ACTIVITATS =============================

// Estructura per emmagatzemar l'activitat de l'usuari

typedef struct {
    char tipus_activitat[30];
    char detalls[100];
    time_t timestamp;
} Activitat;

// Función para registrar la actividad de un usuario
void registre_activitat(usuari_t *usuari, const char *tipus, const char *detalls) {
    Activitat nova_activitat;
    strncpy(nova_activitat.tipus_activitat, tipus, sizeof(nova_activitat.tipus_activitat) - 1);
    strncpy(nova_activitat.detalls, detalls, sizeof(nova_activitat.detalls) - 1);
    nova_activitat.timestamp = time(NULL);

    FILE *file = fopen("activitats.log", "a");  // Modo append para agregar actividades
    if (file == NULL) {
        perror("Error al obrir el fitxer d'activitats");
        return;
    }

    // Formato de registro: "tipo actividad - detalles - timestamp"
    fprintf(file, "Usuari: %s | Activitat: %s | Detalls: %s | Temps: %s", 
            usuari->nom, nova_activitat.tipus_activitat, nova_activitat.detalls, ctime(&nova_activitat.timestamp));

    fclose(file);
    printf("Activitat registrada: %s - %s\n", nova_activitat.tipus_activitat, nova_activitat.detalls);
}

// Función para ver la actividad registrada
char* veure_activitat() {
    FILE *file = fopen("activitats.log", "r");
    if (file == NULL) {
        perror("Error al obrir el fitxer d'activitats");
        return strdup("Error: no es pot obrir el fitxer d'activitats.");
    }

    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        fclose(file);
        return strdup("Error: no hi ha prou memòria.");
    }
    buffer[0] = '\0'; // Inicializamos el buffer como una cadena vacía

    char linea[256];
    while (fgets(linea, sizeof(linea), file) != NULL) {
        if (strlen(buffer) + strlen(linea) + 1 > buffer_size) {
            buffer_size *= 2; // Duplicamos el tamaño del buffer
            char *temp = realloc(buffer, buffer_size);
            if (temp == NULL) {
                free(buffer);
                fclose(file);
                return strdup("Error: no hi ha prou memòria per carregar l'activitat.");
            }
            buffer = temp;
        }
        strcat(buffer, linea); // Concatenamos la línea actual al buffer
    }

    fclose(file);
    return buffer; // Devolvemos el contenido del archivo como una cadena
}