# Scheduling Ships
## Instrucciones de uso
### 1. Clonar repositorio
    https://github.com/gabrielwolfw/scheduling_ships.git
    cd scheduling_ships
### 2. Instalar paquetes
    sudo apt-get update
    sudo apt-get install build-essential
### 3. Configuración mediante txt en el archivo config_canal.txt
    control_flujo=MODO_LETRERO
    longitud_canal=10
    velocidad_normal=1.0
    velocidad_pesquero=2.0
    velocidad_patrulla=3.0
    cantidad_barcos=6
    tiempo_letrero=20
    parametro_w=3
    algoritmo=ROUND_ROBIN
    quantum=5

### 4. Añadir barcos manualmente
Ejemplo:
    
    crear_barco(barcos, 0, 1, NORMAL, longitud_canal); // Barco 0, dirección derecha
    enviar_commando("GENERATE NORMAL RIGHT1\n", (void *)serial_port);
    leds[3] = 0;

    crear_barco(barcos, 1, 1, PESQUERO, longitud_canal);  // Barco 1, dirección derecha
    enviar_commando("GENERATE PESQUERO RIGHT2\n", (void *)serial_port);
    leds[4] = 1;

    crear_barco(barcos, 2, 0, PATRULLA, longitud_canal);  // Barco 2, dirección izquierda
    enviar_commando("GENERATE PATRULLA LEFT1\n", (void *)serial_port);
    leds[0] = 2;

    crear_barco(barcos, 3, 0, NORMAL, longitud_canal);    // Barco 3, dirección izquierda
    enviar_commando("GENERATE NORMAL LEFT2\n", (void *)serial_port);
    leds[1] = 3;

    crear_barco(barcos, 4, 0, NORMAL, longitud_canal);    // Barco 4, dirección izquierda
    enviar_commando("GENERATE NORMAL LEFT3\n", (void *)serial_port);
    leds[2] = 4;

### 4. Compilación
    make
    make run

