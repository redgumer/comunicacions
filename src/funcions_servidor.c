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
#include <time.h>
#include "cJSON.h"

// Llibreries de xarxa
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Fitxer de funcions del servidor
#include "../include/funcions_servidor.h"

#define MAX_SUGGERIMENTS 5 // Máximo de sugerencias a mostrar
#define MIDA_PAQUET 1500

// Verifica si el usuario existe y si la contraseña es correcta
int verifica_usuari(const char *nom, const char *contrasenya)
{
    FILE *file = fopen("include/users.json", "r");
    if (!file)
    {
        perror("Error al abrir el archivo JSON");
        return -1; // Error al abrir el archivo
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *json_data = (char *)malloc(file_size + 1);
    if (!json_data)
    {
        fclose(file);
        return -1; // Error de memoria
    }
    fread(json_data, 1, file_size, file);
    fclose(file);
    json_data[file_size] = '\0';

    cJSON *usuarios_json = cJSON_Parse(json_data);
    free(json_data);
    if (!usuarios_json)
    {
        printf("Error al parsear JSON.\n");
        return -1;
    }

    // Buscar el usuario en el JSON
    cJSON *usuario = cJSON_GetObjectItem(usuarios_json, nom);
    if (!usuario)
    {
        cJSON_Delete(usuarios_json);
        return 0; // Usuario no encontrado
    }

    // Verificar la contraseña
    cJSON *contrasenya_json = cJSON_GetObjectItem(usuario, "contrasenya");
    if (!contrasenya_json || !cJSON_IsString(contrasenya_json))
    {
        cJSON_Delete(usuarios_json);
        return -2; // Error en el formato de la contraseña
    }
    const char *password_json = contrasenya_json->valuestring;
    if (strcmp(password_json, contrasenya) == 0)
    {
        cJSON_Delete(usuarios_json);
        return 1; // Usuario autenticado correctamente
    }

    cJSON_Delete(usuarios_json);
    return -2; // Contraseña incorrecta
}

// Función para registrar un nuevo usuario en el archivo JSON
int gestiona_registre(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio)
{
    FILE *file = fopen("../include/users.json", "r");
    long file_size = 0;
    char *json_data = NULL;

    if (file)
    {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        json_data = (char *)malloc(file_size + 1);
        if (!json_data)
        {
            fclose(file);
            return -1; // Error de memoria
        }
        fread(json_data, 1, file_size, file);
        fclose(file);
        json_data[file_size] = '\0';
    }

    // Cargar o inicializar JSON de usuarios
    cJSON *usuarios_json = file_size > 0 ? cJSON_Parse(json_data) : cJSON_CreateObject();
    free(json_data);
    if (!usuarios_json)
    {
        printf("Error al parsear JSON.\n");
        return -1;
    }

    // Comprobar si el usuario ya existe
    if (cJSON_GetObjectItem(usuarios_json, nom) != NULL)
    {
        cJSON_Delete(usuarios_json);
        return 0; // Usuario ya existe
    }

    // Crear el objeto JSON del nuevo usuario
    cJSON *nuevo_usuario = cJSON_CreateObject();
    cJSON_AddStringToObject(nuevo_usuario, "contrasenya", contrasenya);
    cJSON_AddStringToObject(nuevo_usuario, "sexe", sexe);
    cJSON_AddStringToObject(nuevo_usuario, "estat_civil", estat_civil);
    cJSON_AddNumberToObject(nuevo_usuario, "edat", edat);
    cJSON_AddStringToObject(nuevo_usuario, "ciutat", ciutat);
    cJSON_AddStringToObject(nuevo_usuario, "descripcio", descripcio);

    // Agregar el nuevo usuario al JSON raíz
    cJSON_AddItemToObject(usuarios_json, nom, nuevo_usuario);

    // Guardar el JSON actualizado en el archivo
    file = fopen("../include/users.json", "w");
    if (!file)
    {
        printf("Error al abrir el archivo JSON para escribir.\n");
        cJSON_Delete(usuarios_json);
        return -1;
    }
    char *json_string = cJSON_Print(usuarios_json);
    fprintf(file, "%s", json_string);
    fclose(file);

    // Liberar memoria
    cJSON_Delete(usuarios_json);
    free(json_string);

    return 1; // Registro exitoso
}

int registra_usuari(const char *nom, const char *contrasenya, const char *sexe, const char *estat_civil, int edat, const char *ciutat, const char *descripcio)
{
    // Cargar el archivo JSON de usuarios
    cJSON *usuarios_json = cargar_json("include/users.json");
    if (!usuarios_json)
    {
        return -1; // Error al abrir el archivo
    }

    // Verificar si el usuario ya existe
    if (cJSON_GetObjectItem(usuarios_json, nom) != NULL)
    {
        cJSON_Delete(usuarios_json);
        return 0; // Usuario ya registrado
    }

    // Crear un nuevo objeto JSON para el usuario
    cJSON *nuevo_usuario = cJSON_CreateObject();
    cJSON_AddStringToObject(nuevo_usuario, "contrasenya", contrasenya);
    cJSON_AddStringToObject(nuevo_usuario, "sexe", sexe);
    cJSON_AddStringToObject(nuevo_usuario, "estat_civil", estat_civil);
    cJSON_AddNumberToObject(nuevo_usuario, "edat", edat);
    cJSON_AddStringToObject(nuevo_usuario, "ciutat", ciutat);
    cJSON_AddStringToObject(nuevo_usuario, "descripcio", descripcio);

    // Añadir el nuevo usuario al JSON de usuarios
    cJSON_AddItemToObject(usuarios_json, nom, nuevo_usuario);

    // Guardar los cambios en el archivo JSON
    if (guardar_json("include/users.json", usuarios_json))
    {
        // Registrar actividad de creación del usuario
        usuari_t nou_usuari;
        strncpy(nou_usuari.nom, nom, sizeof(nou_usuari.nom) - 1);
        registre_activitat(nou_usuari.nom, "Registre d'usuari", "Nou usuari registrat");

        cJSON_Delete(usuarios_json); // Liberar memoria
        return 1;                    // Usuario registrado correctamente
    }
    else
    {
        cJSON_Delete(usuarios_json); // Liberar memoria en caso de error
        return -1;                   // Error al guardar el archivo
    }
}

const char *veure_perfil(const usuari_t *usuari)
{
    static char resposta[600];
    snprintf(resposta, sizeof(resposta),
             "Perfil de l'usuari:\n"
             "Nom: %s\n"
             "Sexe: %s\n"
             "Estat Civil: %s\n"
             "Edat: %d\n"
             "Ciutat: %s\n"
             "Descripció: %s\n",
             usuari->nom, usuari->sexe, usuari->estat_civil, usuari->edat, usuari->ciutat, usuari->descripcio);

    // Registro de actividad para la visualización del perfil
    registre_activitat(usuari->nom, "Consulta de perfil d'usuari", "Visualización del perfil");

    return resposta;
}

// Función para guardar el objeto JSON en un archivo
int guardar_json(const char *filename, cJSON *json)
{
    char *json_string = cJSON_Print(json);
    if (!json_string)
    {
        fprintf(stderr, "Error al convertir JSON a cadena\n");
        return 0;
    }

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Error al abrir el archivo para escritura");
        free(json_string);
        return 0;
    }

    fprintf(file, "%s", json_string);
    fclose(file);
    free(json_string);

    return 1;
}

const char *afegir_amic(const char *nom, const char *nou_amic)
{
    static char resposta[100];

    // Obtener la información completa del usuario
    usuari_t usuario_info = get_user_info(nom);
    if (usuario_info.nom[0] == '\0')
    { // Verificar si el usuario fue encontrado
        registre_activitat(usuario_info.nom, "Error", "Usuario no encontrado en la función afegir_amic");
        return "Error: Usuario no encontrado.\n";
    }

    // Cargar el archivo JSON de amistades
    cJSON *amistades_json = cargar_json("amistats.json");
    if (!amistades_json)
    {
        snprintf(resposta, sizeof(resposta), "Error: No se puede abrir el archivo de relaciones de amistad.\n");
        return resposta;
    }

    // Verificar si el usuario y el nuevo amigo ya son amigos
    cJSON *amigos_usuario = cJSON_GetObjectItem(amistades_json, nom);
    if (!amigos_usuario)
    {
        amigos_usuario = cJSON_AddArrayToObject(amistades_json, nom); // Crear la lista si no existe
    }

    // Verificar si el nuevo amigo ya está en la lista de amigos
    cJSON *amigo;
    int existe = 0;
    cJSON_ArrayForEach(amigo, amigos_usuario)
    {
        if (strcmp(amigo->valuestring, nou_amic) == 0)
        {
            existe = 1;
            break;
        }
    }

    // Si no existe, añadir la relación de amistad
    if (!existe)
    {
        cJSON_AddItemToArray(amigos_usuario, cJSON_CreateString(nou_amic));

        // Añadir la relación inversa en el JSON
        cJSON *amigos_nou_amic = cJSON_GetObjectItem(amistades_json, nou_amic);
        if (!amigos_nou_amic)
        {
            amigos_nou_amic = cJSON_AddArrayToObject(amistades_json, nou_amic);
        }
        cJSON_AddItemToArray(amigos_nou_amic, cJSON_CreateString(nom));

        // Guardar los cambios en el archivo JSON
        if (guardar_json("amistats.json", amistades_json))
        {
            snprintf(resposta, sizeof(resposta), "Amistad creada entre %s y %s\n", nom, nou_amic);
            registre_activitat(usuario_info.nom, "Amistad", "Amistad creada exitosamente con el usuario especificado");
        }
        else
        {
            snprintf(resposta, sizeof(resposta), "Error: No se pudo guardar la relación de amistad.\n");
            registre_activitat(usuario_info.nom, "Error", "Error al guardar la nueva amistad en el archivo JSON");
        }
    }
    else
    {
        snprintf(resposta, sizeof(resposta), "La relación de amistad ya existe entre %s y %s\n", nom, nou_amic);
        registre_activitat(usuario_info.nom, "Amistad", "Intento de amistad fallido, ya existe la relación");
    }

    cJSON_Delete(amistades_json); // Liberar memoria

    return resposta;
}

usuari_t get_user_info(const char *nom)
{
    FILE *file = fopen(USER_FILE, "r");
    usuari_t usuari;

    // Inicializamos el usuario por defecto en caso de no encontrarlo
    strcpy(usuari.nom, ""); // Indicador de usuario no encontrado
    strcpy(usuari.contrasenya, "");
    strcpy(usuari.sexe, "");
    strcpy(usuari.estat_civil, "");
    usuari.edat = -1; // Indicador de edad no válida
    strcpy(usuari.ciutat, "");
    strcpy(usuari.descripcio, "");

    if (!file)
    {
        perror("Error al abrir el archivo de usuarios");
        registre_activitat(nom, "Error al abrir el archivo de usuarios", " "); // Registro de error
        return usuari;                                                         // Retorna el usuario no encontrado (con campos vacíos)
    }

    while (fscanf(file, "%s %s %s %s %d %s %[^\n]",
                  usuari.nom, usuari.contrasenya, usuari.sexe,
                  usuari.estat_civil, &usuari.edat, usuari.ciutat,
                  usuari.descripcio) == 7)
    {
        if (strcmp(usuari.nom, nom) == 0)
        {
            fclose(file);
            registre_activitat(nom, "Consulta de información del usuario exitosa", " "); // Registro de éxito
            return usuari;
        }
    }

    fclose(file);
    registre_activitat(nom, "Usuario no encontrado en el archivo de usuarios", " "); // Registro de no encontrado
    usuari.nom[0] = '\0';                                                            // Aseguramos el indicador de usuario no encontrado
    return usuari;
}

const char *processa_opcio(int opcio, const char *nom, int *continuar)
{
    static char resposta[200];
    usuari_t user;

    switch (opcio)
    {
    case 1: // Veure perfil
        user = get_user_info(nom);
        if (user.nom[0] != '\0')
        {
            resposta[0] = '\0';
            strncpy(resposta, veure_perfil(&user), sizeof(resposta) - 1);
        }
        else
        {
            snprintf(resposta, sizeof(resposta), "Usuari no trobat\n");
        }
        break;

    case 2: // Veure els meus amics
        strncpy(resposta, veure_amics(nom), sizeof(resposta) - 1);
        break;

    case 3: // Afegir amics nous
        printf("Introdueix el nom de l'amic a afegir: ");
        char nou_amic[MAX_USUARI];
        scanf("%s", nou_amic);
        strncpy(resposta, afegir_amic(nom, nou_amic), sizeof(resposta) - 1);
        break;

    case 4: // Consultar l'activitat de l'usuari
        strncpy(resposta, veure_activitat(nom), sizeof(resposta) - 1);
        break;

    case 5: // Tancar el programa
        snprintf(resposta, sizeof(resposta), "Sortint del programa...");
        registre_activitat(nom, "Sortida", "Usuari ha tancat el programa");
        *continuar = 0; // Señalar que el programa debe cerrarse
        break;

    default:
        snprintf(resposta, sizeof(resposta), "Opció invàlida.");
        break;
    }

    return resposta;
}

// ============================= GESTIONA CONECCIÓ =============================
// Función para inicializar el servidor
int inicializar_servidor(int port, struct sockaddr_in *socket_servidor) {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("Error al crear el socket");
        exit(1);
    }
    socket_servidor->sin_family = AF_INET;
    socket_servidor->sin_addr.s_addr = INADDR_ANY;
    socket_servidor->sin_port = htons(port);
    if (bind(s, (struct sockaddr *)socket_servidor, sizeof(*socket_servidor)) < 0) {
        perror("Error en el bind");
        close(s);
        exit(1);
    }
    return s;
}

// Función para recibir datos del cliente
int recibir_datos(int s, struct sockaddr_in *contacte_client, char *paquet) {
    socklen_t contacte_client_mida = sizeof(*contacte_client);
    return recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)contacte_client, &contacte_client_mida);
}

// Función para extraer nombre y contraseña del paquete recibido
void extraer_credenciales(char *paquet, char *nombre, char *contrasena) {
    char *token = strtok(paquet, ":");
    if (token != NULL) {
        strncpy(nombre, token, 49);
        nombre[49] = '\0';
        
        token = strtok(NULL, ":");
        if (token != NULL) {
            strncpy(contrasena, token, 49);
            contrasena[49] = '\0';
        }
    }
}

// Función para enviar el código de respuesta al cliente
void enviar_respuesta(int s, struct sockaddr_in *contacte_client, int respuesta) {
    sendto(s, &respuesta, sizeof(respuesta), 0, (struct sockaddr *)contacte_client, sizeof(*contacte_client));
}

// Función principal de manejo de conexión
void manejar_conexion(int s, struct sockaddr_in *contacte_client) {
    char paquet[MIDA_PAQUET], nombre[50], contrasena[50];

    if (recibir_datos(s, contacte_client, paquet) > 0) {
        printf("Paquete recibido: %s\n", paquet);

        // Extraer nombre y contraseña
        extraer_credenciales(paquet, nombre, contrasena);

        // Verificar credenciales
        int respuesta = verifica_usuari(nombre, contrasena);

        // Enviar código de respuesta al cliente
        enviar_respuesta(s, contacte_client, respuesta);
    }
}
// ============================= HISTORIAL D'ACTIVITATS =============================

// Función para registrar la actividad de un usuario
void registre_activitat(const char *usuari, const char *accio, const char *detall)
{
    Activitat nova_activitat;
    strncpy(nova_activitat.tipus_activitat, accio, sizeof(nova_activitat.tipus_activitat) - 1);
    nova_activitat.tipus_activitat[sizeof(nova_activitat.tipus_activitat) - 1] = '\0';

    strncpy(nova_activitat.detalls, detall, sizeof(nova_activitat.detalls) - 1);
    nova_activitat.detalls[sizeof(nova_activitat.detalls) - 1] = '\0';

    nova_activitat.timestamp = time(NULL);

    FILE *file = fopen("data/activitats.log", "a"); // Modo append para agregar actividades
    if (file == NULL)
    {
        perror("Error al obrir el fitxer d'activitats");
        return;
    }

    // Formato de registro: "tipo actividad - detalles - timestamp"
    fprintf(file, "Usuari: %s | Activitat: %s | Detalls: %s | Temps: %s",
            usuari, nova_activitat.tipus_activitat, nova_activitat.detalls, ctime(&nova_activitat.timestamp));

    fclose(file);
    printf("Activitat registrada: %s - %s\n", nova_activitat.tipus_activitat, nova_activitat.detalls);
}

// Función para ver el historial de actividades de un usuario

char *veure_activitat(const char *nom)
{
    // Registrar entrada en veure_activitat
    registre_activitat(nom, "Acceso a veure_activitat", "Consulta de actividades");
    FILE *file = fopen("data/activitats.log", "r");
    if (file == NULL)
    {
        perror("Error al obrir el fitxer d'activitats");
        return strdup("Error: no es pot obrir el fitxer d'activitats.");
    }

    // Array dinámico de punteros para almacenar las líneas
    size_t max_lines = 100;
    size_t line_count = 0;
    char **lines = malloc(max_lines * sizeof(char *));
    if (lines == NULL)
    {
        fclose(file);
        return strdup("Error: no hi ha prou memòria.");
    }

    // Leer líneas del archivo y almacenarlas en el array
    char linea[256];
    while (fgets(linea, sizeof(linea), file) != NULL)
    {
        if (line_count >= max_lines)
        {
            max_lines += 50; // Expandimos el array dinámico si es necesario
            char **temp = realloc(lines, max_lines * sizeof(char *));
            if (temp == NULL)
            {
                for (size_t i = 0; i < line_count; i++)
                    free(lines[i]);
                free(lines);
                fclose(file);
                return strdup("Error: no hi ha prou memòria per carregar l'activitat.");
            }
            lines = temp;
        }
        lines[line_count] = strdup(linea);
        if (lines[line_count] == NULL)
        {
            for (size_t i = 0; i < line_count; i++)
                free(lines[i]);
            free(lines);
            fclose(file);
            return strdup("Error: no hi ha prou memòria per carregar l'activitat.");
        }
        line_count++;
    }
    fclose(file);

    // Calcular el tamaño del buffer final
    size_t buffer_size = 1;
    for (size_t i = 0; i < line_count; i++)
        buffer_size += strlen(lines[i]);

    char *buffer = malloc(buffer_size);
    if (buffer == NULL)
    {
        for (size_t i = 0; i < line_count; i++)
            free(lines[i]);
        free(lines);
        return strdup("Error: no hi ha prou memòria.");
    }
    buffer[0] = '\0';

    // Concatenar las líneas en orden inverso
    for (size_t i = line_count; i > 0; i--)
    {
        strcat(buffer, lines[i - 1]);
    }

    // Liberar memoria utilizada
    for (size_t i = 0; i < line_count; i++)
        free(lines[i]);
    free(lines);

    return buffer;
}

// ============================= SUGERÈNCIES D'AMICS =============================

// Función para cargar el archivo JSON y obtener el objeto raíz
cJSON *cargar_json(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error al abrir el archivo JSON");
        return NULL;
    }

    // Leer el contenido del archivo
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);

    // Parsear el contenido JSON
    cJSON *json = cJSON_Parse(data);
    free(data);

    if (!json)
    {
        fprintf(stderr, "Error al parsear JSON\n");
        return NULL;
    }
    return json;
}

const char *suggerir_amics(const char *nom)
{
    static char resposta[500];
    resposta[0] = '\0';

    // Obtener la información completa del usuario
    usuari_t usuario_info = get_user_info(nom);
    if (usuario_info.nom[0] == '\0')
    { // Verificar si el usuario fue encontrado
        registre_activitat(usuario_info.nom, "Error", "Usuario no encontrado en suggerir_amics");
        return "Error: Usuario no encontrado.\n";
    }

    // Cargar el archivo JSON
    cJSON *amistades_json = cargar_json("amistats.json");
    if (!amistades_json)
    {
        snprintf(resposta, sizeof(resposta), "Error: No se puede abrir el archivo de relaciones de amistad.\n");
        return resposta;
    }

    // Obtener la lista de amigos directos
    cJSON *amigos_directos = cJSON_GetObjectItem(amistades_json, nom);
    if (!amigos_directos)
    {
        snprintf(resposta, sizeof(resposta), "No tens amics registrats.\n");
        cJSON_Delete(amistades_json);
        return resposta;
    }

    // Crear un array de amigos directos para verificación rápida
    char amics_directes[MAX_AMICS][MAX_USUARI];
    int num_amics_directes = 0;

    cJSON *amigo;
    cJSON_ArrayForEach(amigo, amigos_directos)
    {
        strcpy(amics_directes[num_amics_directes++], amigo->valuestring);
    }

    int suggeriments_encontrados = 0;

    // Buscar amigos de amigos para sugerencias
    cJSON *user;
    cJSON_ArrayForEach(user, amistades_json)
    {
        const char *usuario = user->string;

        // Si el usuario ya es un amigo directo, lo ignoramos
        int es_directe = 0;
        for (int i = 0; i < num_amics_directes; i++)
        {
            if (strcmp(amics_directes[i], usuario) == 0)
            {
                es_directe = 1;
                break;
            }
        }
        if (es_directe || strcmp(usuario, nom) == 0)
            continue; // Evitar amigos directos y el mismo usuario

        // Verificar si hay amigos en común
        cJSON *amigos_usuario = cJSON_GetObjectItem(amistades_json, usuario);
        cJSON_ArrayForEach(amigo, amigos_usuario)
        {
            for (int i = 0; i < num_amics_directes; i++)
            {
                if (strcmp(amigo->valuestring, amics_directes[i]) == 0)
                { // Encontramos un amigo en común
                    snprintf(resposta + strlen(resposta), sizeof(resposta) - strlen(resposta),
                             "Suggeriment: %s\n", usuario);
                    suggeriments_encontrados++;
                    break;
                }
            }
            if (suggeriments_encontrados >= MAX_SUGGERIMENTS)
                break;
        }
        if (suggeriments_encontrados >= MAX_SUGGERIMENTS)
            break;
    }

    cJSON_Delete(amistades_json); // Liberar memoria

    if (suggeriments_encontrados > 0)
    {
        registre_activitat(usuario_info.nom, "Sugerencia", "Sugerencias de amigos generadas exitosamente");
    }
    else
    {
        snprintf(resposta, sizeof(resposta), "No hi ha suggeriments de nous amics.\n");
        registre_activitat(usuario_info.nom, "Sugerencia", "No se encontraron sugerencias de amigos");
    }

    return resposta;
}

const char *veure_amics(const char *nom)
{
    static char resposta[500];
    resposta[0] = '\0';

    // Obtener la información completa del usuario
    usuari_t usuario_info = get_user_info(nom);
    if (usuario_info.nom[0] == '\0')
    { // Verificar si el usuario fue encontrado
        registre_activitat(usuario_info.nom, "Error", "Usuario no encontrado en veure_amics");
        return "Error: Usuario no encontrado.\n";
    }

    // Cargar el archivo de amistades
    cJSON *amistades_json = cargar_json("amistats.json");
    if (!amistades_json)
    {
        snprintf(resposta, sizeof(resposta), "Error: No se puede abrir el archivo de relaciones de amistad.\n");
        return resposta;
    }

    // Obtener amigos directos del usuario
    cJSON *amigos_directos = cJSON_GetObjectItem(amistades_json, nom);
    if (!amigos_directos)
    {
        snprintf(resposta, sizeof(resposta), "No tens amics registrats.\n");
        cJSON_Delete(amistades_json);
        return resposta;
    }

    // Cargar el archivo de usuarios
    cJSON *usuarios_json = cargar_json("/include/users.json");
    if (!usuarios_json)
    {
        snprintf(resposta, sizeof(resposta), "Error: No se puede abrir el archivo de usuarios.\n");
        cJSON_Delete(amistades_json);
        return resposta;
    }

    // Iterar sobre la lista de amigos y obtener su información
    cJSON *amigo;
    int amigos_encontrados = 0;
    cJSON_ArrayForEach(amigo, amigos_directos)
    {
        const char *friend_name = amigo->valuestring;
        cJSON *friend_info = cJSON_GetObjectItem(usuarios_json, friend_name);

        if (friend_info)
        {
            const char *sexe = cJSON_GetObjectItem(friend_info, "sexe")->valuestring;
            const char *estat_civil = cJSON_GetObjectItem(friend_info, "estat_civil")->valuestring;
            int edat = cJSON_GetObjectItem(friend_info, "edat")->valueint;
            const char *ciutat = cJSON_GetObjectItem(friend_info, "ciutat")->valuestring;
            const char *descripcio = cJSON_GetObjectItem(friend_info, "descripcio")->valuestring;

            snprintf(resposta + strlen(resposta), sizeof(resposta) - strlen(resposta),
                     "Amic: %s\nSexe: %s\nEstat Civil: %s\nEdat: %d\nCiutat: %s\nDescripció: %s\n\n",
                     friend_name, sexe, estat_civil, edat, ciutat, descripcio);
            amigos_encontrados++;
        }
    }

    // Registrar la actividad según el resultado
    if (amigos_encontrados > 0)
    {
        registre_activitat(usuario_info.nom, "Consulta", "Consulta de lista de amigos completada");
    }
    else
    {
        snprintf(resposta, sizeof(resposta), "No tens amics registrats.\n");
        registre_activitat(usuario_info.nom, "Consulta", "No se encontraron amigos para el usuario");
    }

    // Liberar memoria JSON
    cJSON_Delete(amistades_json);
    cJSON_Delete(usuarios_json);

    return resposta;
}
