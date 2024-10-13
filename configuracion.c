#include "configuracion.h"

// Función para leer el archivo de configuración
void leer_configuracion(const char *nombre_archivo, ConfiguracionCanal *config) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;  // Quitar salto de línea

        char *nombre_parametro = strtok(linea, "=");
        char *valor_parametro = strtok(NULL, "=");

        if (strcmp(nombre_parametro, "control_flujo") == 0) {
            strcpy(config->control_flujo, valor_parametro);
        } else if (strcmp(nombre_parametro, "longitud_canal") == 0) {
            config->longitud_canal = atoi(valor_parametro);
        } else if (strcmp(nombre_parametro, "velocidad_normal") == 0) {
            config->velocidad_normal = atof(valor_parametro);
        } else if (strcmp(nombre_parametro, "velocidad_pesquero") == 0) {
            config->velocidad_pesquero = atof(valor_parametro);
        } else if (strcmp(nombre_parametro, "velocidad_patrulla") == 0) {
            config->velocidad_patrulla = atof(valor_parametro);
        } else if (strcmp(nombre_parametro, "cantidad_barcos") == 0) {
            config->cantidad_barcos = atoi(valor_parametro);
        } else if (strcmp(nombre_parametro, "tiempo_letrero") == 0) {
            config->tiempo_letrero = atoi(valor_parametro);
        } else if (strcmp(nombre_parametro, "parametro_w") == 0) {
            config->parametro_w = atoi(valor_parametro);
        } else if (strcmp(nombre_parametro, "algoritmo") == 0) {
            strcpy(config->algoritmo, valor_parametro);
        } else if (strcmp(nombre_parametro, "quantum") == 0) {
            config->quantum = atoi(valor_parametro);  // Leer el valor del quantum
        }
    }

    fclose(archivo);
}

// Función para determinar el modo de control de flujo
ModoControlFlujo obtener_modo_control_flujo(const char* control_flujo) {
    if (strcmp(control_flujo, "MODO_LETRERO") == 0) {
        return MODO_LETRERO;
    } else if (strcmp(control_flujo, "MODO_EQUIDAD") == 0) {
        return MODO_EQUIDAD;
    } else if (strcmp(control_flujo, "MODO_TICO") == 0) {
        return MODO_TICO;
    } else {
        printf("Método de control de flujo desconocido: %s\n", control_flujo);
        exit(EXIT_FAILURE);  // Salir si el método es desconocido
    }
}

// Función para determinar el algoritmo de calendarización
AlgoritmoCalendarizacion obtener_algoritmo_calendarizacion(const char* algoritmo) {
    if (strcmp(algoritmo, "ROUND_ROBIN") == 0) {
        return ROUND_ROBIN;
    } else if (strcmp(algoritmo, "FCFS") == 0) {
        return FCFS;
    } else if (strcmp(algoritmo, "PRIORIDAD") == 0) {
        return PRIORIDAD;
    } else if (strcmp(algoritmo, "SJF") == 0) {
        return SJF;
    } else if (strcmp(algoritmo, "TIEMPO_REAL") == 0) {
        return TIEMPO_REAL;
    } else {
        printf("Algoritmo de calendarización desconocido: %s\n", algoritmo);
        exit(EXIT_FAILURE);  // Salir si el algoritmo es desconocido
    }
}