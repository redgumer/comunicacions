/*
 ================================ PROJECTE XARXES ================================
 | Fitxer     : funcions_servidor.c                                              |
 | Autors     : Programador 1, Programador 2, Programador 3                      |
 | Assignatura: Xarxes (Segon curs, Enginyeria Informàtica)                      |
 | Universitat: Universitat Rovira i Virgili                                     |
 | Descripció : Breu descripció del fitxer o mòdul                               |
 =================================================================================
*/

// Llibreries bàsiques
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Llibreries de xarxa
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
// Fitxer de funcions del servidor
#include "../include/funcions_servidor.h"

#define USER_FILE "data/usuaris.txt"
#define FRIEND_FILE "data/amistats.txt"

//variables globals
char nom[MAX_USUARI]; // Asegúrate de definir MAX_USUARI con el tamaño adecuado

int verifica_usuari(const char *nom, const char *contrasenya)
{
    FILE *file = fopen(USER_FILE, "r");
    if (!file)
        return -1;

    usuari_t u;
    while (fscanf(file, "%s %s", u.nom, u.contrasenya) == 2)
    {
        if (strcmp(u.nom, nom) == 0 && strcmp(u.contrasenya, contrasenya) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int *edat, const char *ciutat, const char *descripcio)
{
    FILE *file = fopen(USER_FILE, "a"); // Obrir el fitxer en mode "a" per afegir dades al final
    if (!file)
    {
        printf("Error: No es pot obrir el fitxer per registrar l'usuari.\n");
        return -1; // Retorna -1 si no es pot obrir el fitxer
    }

    // Afegir la informació de l'usuari al fitxer
    fprintf(file, "\n%s %s %s %s %d %s %s", nom, contrasenya, sexe, estat_civil, *edat, ciutat, descripcio);
    fclose(file);
    printf("Usuari registrat correctament: %s\n", nom); // Confirmació de registre
    return 1;                                           // Retorna 1 per indicar que el registre ha estat exitós
}

char *imprimir_amics(const char *nom_usuari) {
    FILE *file = fopen(FRIEND_FILE, "r");
    if (!file) {
        perror("Error al obrir el fitxer d'amistats");
        return NULL;
    }

    // Variables per emmagatzemar els IDs d'usuaris
    int id1, id2;
    char linea[256];
    char *resultat = malloc(1024); // Memòria per a la cadena retornada
    if (!resultat) {
        perror("Error al reservar memòria");
        fclose(file);
        return NULL;
    }

    // Inicialitzem la cadena de resultat
    resultat[0] = '\0';

    // Obtenim la informació de l'usuari proporcionat
    usuari_t usuari = obtenir_informació_de_usuari(nom_usuari);
    if (usuari.id == -1) {
        snprintf(resultat, 1024, "Usuari '%s' no trobat.\n", nom_usuari);
        fclose(file);
        return resultat;
    }

    // Llistem les amistats
    strcat(resultat, "Amics de ");
    strcat(resultat, nom_usuari);
    strcat(resultat, ":\n");

    while (fgets(linea, sizeof(linea), file)) {
        if (sscanf(linea, "%d; %d", &id1, &id2) == 2) {
            if (id1 == usuari.id || id2 == usuari.id) {
                // Obtenim la informació de l'amic
                char nom_amic[MAX_USUARI];
                sscanf(linea, "%*d; %s", nom_amic);
                usuari_t amic = obtenir_informació_de_usuari(nom_amic);
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "Nom: %s, Sexe: %s, Edat: %d, Ciutat: %s, Descripció: %s\n",
                         amic.nom, amic.sexe, amic.edat, amic.ciutat, amic.descripcio);
                strcat(resultat, buffer);
            }
        }
    }

    fclose(file);
    return resultat;
}

const char *afegir_amic(const char *nom, const char *nou_amic)
{
    static char resposta[100];
    FILE *file = fopen(FRIEND_FILE, "r+"); // Abrimos el archivo para lectura y escritura
    if (!file)
    {
        snprintf(resposta, sizeof(resposta), "Error: No se puede abrir el archivo de relaciones de amistad.\n");
        return resposta;
    }

    char line[100];
    int exists = 0;

    // Comprobar si la amistad ya existe en cualquier dirección
    while (fgets(line, sizeof(line), file))
    {
        char user1[MAX_USUARI], user2[MAX_USUARI];
        sscanf(line, "%s %s", user1, user2);
        if ((strcmp(user1, nom) == 0 && strcmp(user2, nou_amic) == 0) ||
            (strcmp(user1, nou_amic) == 0 && strcmp(user2, nom) == 0))
        {
            exists = 1;
            break;
        }
    }

    if (!exists)
    {
        // Agregar la amistad en ambas direcciones
        fprintf(file, "%s %s\n", nom, nou_amic);
        fprintf(file, "%s %s\n", nou_amic, nom);
        snprintf(resposta, sizeof(resposta), "Amistad creada entre %s y %s\n", nom, nou_amic);
    }
    else
    {
        snprintf(resposta, sizeof(resposta), "La relación de amistad ya existe entre %s y %s\n", nom, nou_amic);
    }

    fclose(file);
    return resposta;
}

usuari_t obtenir_informació_de_usuari(const char *nom)
{
    FILE *file = fopen(USER_FILE, "r");
    usuari_t usuari;
    char linea[256];
    char nom_fitxer[50];

    // Inicialitzem l'estructura per evitar valors indeterminats
    memset(&usuari, 0, sizeof(usuari_t));

    // Verifiquem si s'ha pogut obrir el fitxer
    if (!file)
    {
        perror("Error al obrir el fitxer d'usuaris");
        return usuari;
    }

    // Llegim el fitxer línia per línia
    while (fgets(linea, sizeof(linea), file))
    {
        // Extraiem el nom per comparar
        sscanf(linea, "%s", nom_fitxer);

        // Comprovem si el nom coincideix
        if (strcmp(nom_fitxer, nom) == 0)
        {
            // Carreguem la informació de l'usuari a l'estructura
            sscanf(linea, "%s %s %s %s %d %s %[^\n]",
                   usuari.nom,
                   usuari.contrasenya,
                   usuari.sexe,
                   usuari.estat_civil,
                   &usuari.edat,
                   usuari.ciutat,
                   usuari.descripcio);

            fclose(file);
            return usuari;
        }
    }

    // Si no es troba l'usuari, tanquem el fitxer i retornem l'estructura buida
    fclose(file);
    fprintf(stderr, "Usuari no trobat: %s\n", nom);
    return usuari;
}

char *imprimir_usuari(const usuari_t *usuari)
{
    // Reserva de memoria para la cadena de salida
    char *cadena = (char *)malloc(512 * sizeof(char));
    if (cadena == NULL)
    {
        printf("Error al asignar memoria\n");
        return NULL;
    }

    // Formateo de la cadena
    sprintf(cadena,
            "Nom: %s\n"
            "Contrasenya: %s\n"
            "Sexe: %s\n"
            "Estat civil: %s\n"
            "Edat: %d\n"
            "Ciutat: %s\n"
            "Descripció: %s\n",
            usuari->nom,
            usuari->contrasenya,
            usuari->sexe,
            usuari->estat_civil,
            usuari->edat,
            usuari->ciutat,
            usuari->descripcio);

    return cadena;
}


const char *processa_opcio(int opcio, const char *nom, int *continuar)
{
    const char *resposta;
    usuari_t user;
    switch (opcio)
    {
    case 1: // Veure perfil
        user = obtenir_informació_de_usuari(nom);
        resposta = (user.nom[0] != '\0') ? imprimir_usuari(&user) : "Usuari no trobat\n";
        break;
    case 2: // Veure els meus amics
        resposta = imprimir_amics(nom);
        break;
    case 3: // Afegir amics nous
        resposta = afegir_amic(nom, "nou_amic");
        break;
    case 4: // Tancar el programa
        resposta = "Sortint del programa...";
        *continuar = 0;
        break;
    default:
        resposta = "Opció invàlida.";
    }
    return resposta;
}

int configura_socket(int port)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in socket_servidor;
    socket_servidor.sin_family = AF_INET;
    socket_servidor.sin_addr.s_addr = INADDR_ANY;
    socket_servidor.sin_port = htons(port);

    // Enllaça el socket al port especificat
    if (bind(s, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor)) < 0)
    {
        printf("No s'ha pogut enllaçar el socket\n");
        return -1;
    }
    printf("Servidor operatiu al port %d!\n", port);
    return s;
}

void gestiona_client(int s, struct sockaddr_in contacte_client)
{
    socklen_t contacte_client_mida = sizeof(contacte_client);
    char paquet[MIDA_PAQUET];
    char contrasenya[MAX_CONTRASENYA], sexe[MAX_SEXE], estat_civil[MAX_ESTAT_CIVIL], ciutat[MAX_CIUTAT], descripcio[MAX_DESC];
    int edat;
    int resultat; // Declaració de resultat fora del bucle do-while

    printf("Iniciant gestió del client.\n");

    // Rep credencials d'usuari del client
    printf("Esperant credencials del client...\n");
    recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
    sscanf(paquet, "%s %s", nom, contrasenya);
    printf("Credencials rebudes: Usuari : %s, Contrasenya : %s\n", nom, contrasenya);

    // Verifica les credencials d'usuari
    resultat = verifica_usuari(nom, contrasenya);
    printf("Resultat de la verificació: %d\n", resultat);
    do
    {
        if (resultat == 1)
        {
            // Respon que la verificació ha estat correcta
            printf("Usuari verificat\n");
            
            int continuar = 1, opcio = 0;
            while (continuar)
            {
                // Esperar la recepció de l'opció seleccionada pel client
                printf("[Servidor] Esperant opció seleccionada pel client...\n");
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
                printf("[Servidor] Paquet rebut: %s\n", paquet);

                // Analitzar el paquet rebut
                sscanf(paquet, "%d %s", &opcio, nom);
                printf("[Servidor] Opció rebuda: Opció = %d, Usuari = %s\n", opcio, nom);

                // Processar l'opció seleccionada pel client
                printf("[Servidor] Processant opció seleccionada...\n");
                const char *resposta = processa_opcio(opcio, nom, &continuar);
                printf("[Servidor] Resposta processada: %s\n", resposta);

                // Enviar la resposta al client
                sendto(s, resposta, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
                printf("[Servidor] Resposta enviada al client: %s\n", resposta);

                // Comprovar si s'ha de continuar
                printf("[Servidor] Continuar: %d\n", continuar);
            }
        }
        else if (resultat == 0)
        {
            // L'usuari no es troba o no ha estat verificat
            printf("Usuari no trobat o s'ha de registrar\n");
            snprintf(paquet, sizeof(paquet), "%d", resultat);
            sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
            printf("Resposta enviada del servidor no trobat: %s\n", paquet);

            // Processar dades rebudes per registre
            printf("Esperant dades de registre del client...\n");
            recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
            sscanf(paquet, "%s %s %s %s %d %s %[^\n]", nom, contrasenya, sexe, estat_civil, &edat, ciutat, descripcio);
            printf("Dades de registre rebudes: Usuari = %s, Contrasenya = %s, Sexe = %s, Estat civil = %s, Edat = %d, Ciutat = %s, Descripció = %s\n",
                   nom, contrasenya, sexe, estat_civil, edat, ciutat, descripcio);

            // Intenta registrar l'usuari
            resultat = registra_usuari(nom, contrasenya, sexe, estat_civil, &edat, ciutat, descripcio);
            printf("Resultat del registre: %d\n", resultat);
        }
        else
        {
            // Error en la verificació de l'usuari
            strcpy(paquet, "Usuari no trobat o error de verificació\n");
            sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
            printf("Resposta enviada per error de verificació: %s\n", paquet);
        }
        printf("Resultat abans del while: %d\n", resultat);
    } while (resultat != -1);

    printf("Finalitzada la gestió del client.\n");
}
