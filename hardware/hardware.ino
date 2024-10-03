// COMANDOS SERIALES

// Comando MOTOR <dirección>
// Cambia la dirección de giro del motor, lo que a su vez cambia el sentido del flujo del canal.
// MOTOR LEFT: Establece el motor para que gire hacia la izquierda (flujo de derecha a izquierda).
// MOTOR RIGHT: Establece el motor para que gire hacia la derecha (flujo de izquierda a derecha).

// POSICIONES
// Para los botes se manejan una cola con un orden específico para cada bote generado, cada cola
// tiene capacidad para 3 botes, y van en este orden:
// Botes del lado izquierdo: [3,2,1]
// Botes del lado derecho: [1,2,3]

// Comando GENERATE <tipo_barco> <posición>
// Genera un barco de un tipo específico en una posición específica de la cola (fila de espera).
// <tipo_barco>: Puede ser uno de los siguientes:
// PATRULLA: Barco de tipo patrulla.
// NORMAL: Barco de tipo normal.
// PESQUERO: Barco de tipo pesquero.
// <posición>: Especifica la posición en la que se generará el barco. Puede ser:
// Lado izquierdo: LEFT1, LEFT2, LEFT3.
// Lado derecho: RIGHT1, RIGHT2, RIGHT3.
// Ejemplos:
// GENERATE PATRULLA LEFT1: Genera un barco de tipo patrulla en la posición LEFT1.
// GENERATE NORMAL RIGHT2: Genera un barco de tipo normal en la posición RIGHT2.
// Si la posición especificada ya está ocupada, el Arduino responderá: La posición <posición> ya está ocupada


// Comando REMOVE <posición>
// Remueve el barco que se encuentra en la posición especificada.
// <posición>: Especifica la posición de la cual se removerá el barco. Puede ser:
// Lado izquierdo: LEFT1, LEFT2, LEFT3.
// Lado derecho: RIGHT1, RIGHT2, RIGHT3.
// Ejemplos:
// REMOVE LEFT1: Remueve el barco en la posición LEFT1.

// Si se envía un comando que no coincide con ninguno de los anteriores, el Arduino responderá: Comando desconocido

// CONSIDERACIONES
// Los comandos deben terminar con un carácter de nueva línea (\n) para que el Arduino los procese.
// Los espacios son importantes para separar los parámetros.
// El sistema no distingue entre mayúsculas y minúsculas; todos los comandos se convierten a mayúsculas internamente.

//Respuestas y Estado del Sistema
// Estado del Canal: El Arduino envía periódicamente el estado del canal y las colas por serial:
// Sentido: ->
// Cola Izq: -,P,-
// Cola Der: N,-,-
// -> indica flujo de izquierda a derecha.
// <- indica flujo de derecha a izquierda.

// Abreviaturas de los tipos de barcos en las colas:
// P: Patrulla
// N: Normal
// S: Pesquero (usamos 'S' para 'Pesquero')
// -: Posición vacía


// Pines del motor
const int in1 = 8;
const int in2 = 9;
const int in3 = 10;
const int in4 = 11;

// Pin del joystick
const int joystickX = A0; // Conectado al eje X del joystick

// Pines de los LEDs de sentido del canal
const int ledIzquierdaDerecha = 22; // LED para flujo de izquierda a derecha
const int ledDerechaIzquierda = 24; // LED para flujo de derecha a izquierda

// Pines de los LEDs (3 para la izquierda, 3 para la derecha)
// Ajustamos los pines para reflejar el orden físico 3, 2, 1
const int ledIzquierda1 = 7; // LED físico LEFT3
const int ledIzquierda2 = 6; // LED físico LEFT2
const int ledIzquierda3 = 5; // LED físico LEFT1
const int ledDerecha1 = 12;  // LED físico RIGHT3
const int ledDerecha2 = 26;  // LED físico RIGHT2
const int ledDerecha3 = 13;  // LED físico RIGHT1

// Pines de los pulsadores
const int botonPatrulla = 2;
const int botonNormal = 3;
const int botonPesquero = 4;
const int botonGenerarIzquierda = 30; // Botón para seleccionar lado izquierdo (pin 30)
const int botonGenerarDerecha = 32;   // Botón para seleccionar lado derecho (pin 32)

// Secuencia de pasos del motor
const int steps[8][4] = {
  {1, 0, 0, 1},
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1}
};

// Definición de tipos de barcos
const int NO_BOAT = 0;
const int PATRULLA = 1;
const int NORMAL = 2;
const int PESQUERO = 3;

// Variables de control
int direction = 0; // 0 = derecha, 1 = izquierda
bool flujoIzquierdaADerecha = true;

// Arrays para controlar el estado y tipo de los barcos en cada posición
int leftBoats[3] = {NO_BOAT, NO_BOAT, NO_BOAT};   // Estados de los LEDs izquierdos
int rightBoats[3] = {NO_BOAT, NO_BOAT, NO_BOAT};  // Estados de los LEDs derechos

// Variables para selección de lado
int ladoActual = 0; // 0 = ningún lado, 1 = izquierda, 2 = derecha

// Variables de tiempo
unsigned long previousMillisMotor = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisSerial = 0;  // Para el envío continuo de datos por serial
const long intervalLED = 500;   // Intervalo de parpadeo para los LEDs
const long intervalSerial = 1000; // Enviar datos por serial cada segundo
const long intervalMotor = 2;
const long debounceDelay = 200;  // Tiempo de espera para evitar rebotes (200 ms)

// Variables de estado para debouncing
unsigned long lastDebounceTime = 0;  // Tiempo de la última lectura de los botones

// Variables para comunicación serial no bloqueante
String inputString = "";         // Una cadena para almacenar los datos entrantes
bool stringComplete = false;     // Indica si la cadena está completa

void setup() {
  // Configura los pines de salida del motor
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Configura los pines de los LEDs como salida
  pinMode(ledIzquierda1, OUTPUT);
  pinMode(ledIzquierda2, OUTPUT);
  pinMode(ledIzquierda3, OUTPUT);
  pinMode(ledDerecha1, OUTPUT);
  pinMode(ledDerecha2, OUTPUT);
  pinMode(ledDerecha3, OUTPUT);

  // Configura los LEDs que indican el sentido del canal
  pinMode(ledIzquierdaDerecha, OUTPUT);
  pinMode(ledDerechaIzquierda, OUTPUT);

  // Configura los botones para seleccionar el lado
  pinMode(botonGenerarIzquierda, INPUT_PULLUP);
  pinMode(botonGenerarDerecha, INPUT_PULLUP);

  // Configura los pulsadores como entrada con resistencia pull-up
  pinMode(botonPatrulla, INPUT_PULLUP);
  pinMode(botonNormal, INPUT_PULLUP);
  pinMode(botonPesquero, INPUT_PULLUP);

  // Configura el pin del joystick
  pinMode(joystickX, INPUT);

  Serial.begin(115200);  // Aumentamos el baud rate para acelerar la comunicación serial

  // Configuración inicial del motor
  setDirection();
}

void loop() {
  unsigned long currentMillis = millis();

  // Leer los comandos seriales sin bloquear el programa
  checkSerialCommands();

  // Controlar el joystick para cambiar el flujo
  actualizarFlujo();

  // Enviar datos por serial cada segundo
  if (currentMillis - previousMillisSerial >= intervalSerial) {
    previousMillisSerial = currentMillis;
    enviarDatosSerial();
  }

  // Llamamos a la función debounce para los botones
  if (debounce(botonGenerarIzquierda)) {
    ladoActual = 1;  // Lado izquierdo seleccionado
    Serial.println("Lado izquierdo seleccionado para generar barco");
  }

  if (debounce(botonGenerarDerecha)) {
    ladoActual = 2;  // Lado derecho seleccionado
    Serial.println("Lado derecho seleccionado para generar barco");
  }

  // Controlar los pulsadores sin usar delay
  if (ladoActual != 0) {  // Si se ha seleccionado un lado
    if (debounce(botonPatrulla)) {
      generarBarco(PATRULLA, ladoActual);
    }

    if (debounce(botonNormal)) {
      generarBarco(NORMAL, ladoActual);
    }

    if (debounce(botonPesquero)) {
      generarBarco(PESQUERO, ladoActual);
    }
  }

  // Controlar el motor sin delay
  if (currentMillis - previousMillisMotor >= intervalMotor) {
    previousMillisMotor = currentMillis;
    static int stepIndex = 0;
    // Invertimos la lógica de la dirección
    setStep(direction == 0 ? 7 - stepIndex : stepIndex);
    stepIndex = (stepIndex + 1) % 8;  // Mover al siguiente paso
  }

  // Controlar el parpadeo de los LEDs del canal
  controlarParpadeoLEDs(currentMillis);
}

// Función para verificar y procesar comandos seriales sin bloquear
void checkSerialCommands() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  if (stringComplete) {
    inputString.trim(); // Eliminar espacios en blanco
    parseCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}

// Función para analizar y ejecutar comandos seriales
void parseCommand(String command) {
  command.toUpperCase(); // Convertir a mayúsculas para evitar problemas de mayúsculas/minúsculas

  if (command.startsWith("MOTOR ")) {
    String directionCommand = command.substring(6);
    directionCommand.trim();
    if (directionCommand == "LEFT") {
      direction = 1;
      if (flujoIzquierdaADerecha) {  // Solo cambiar si es necesario
        flujoIzquierdaADerecha = false;
        setDirection();
      }
      Serial.println("Motor establecido a IZQUIERDA vía comando serial");
    } else if (directionCommand == "RIGHT") {
      direction = 0;
      if (!flujoIzquierdaADerecha) {
        flujoIzquierdaADerecha = true;
        setDirection();
      }
      Serial.println("Motor establecido a DERECHA vía comando serial");
    } else {
      Serial.println("Comando MOTOR inválido");
    }
  } else if (command.startsWith("GENERATE ")) {
    String params = command.substring(9);
    params.trim();
    int firstSpace = params.indexOf(' ');
    if (firstSpace == -1) {
      Serial.println("Comando GENERATE inválido");
      return;
    }
    String typeStr = params.substring(0, firstSpace);
    String positionStr = params.substring(firstSpace + 1);
    typeStr.trim();
    positionStr.trim();

    int boatType = getBoatTypeFromString(typeStr);
    if (boatType == NO_BOAT) {
      Serial.println("Tipo de barco inválido");
      return;
    }

    if (positionStr.startsWith("LEFT") || positionStr.startsWith("RIGHT")) {
      bool isLeft = positionStr.startsWith("LEFT");
      int index = positionStr.substring(isLeft ? 4 : 5).toInt() - 1;
      if (index >= 0 && index < 3) {
        if (isLeft) {
          if (leftBoats[index] == NO_BOAT) {
            leftBoats[index] = boatType;
            Serial.println("Barco " + typeStr + " generado en " + positionStr);
            controlarLEDs();
          } else {
            Serial.println("La posición " + positionStr + " ya está ocupada");
          }
        } else {
          if (rightBoats[index] == NO_BOAT) {
            rightBoats[index] = boatType;
            Serial.println("Barco " + typeStr + " generado en " + positionStr);
            controlarLEDs();
          } else {
            Serial.println("La posición " + positionStr + " ya está ocupada");
          }
        }
      } else {
        Serial.println("Índice de posición inválido");
      }
    } else {
      Serial.println("Nombre de posición inválido");
    }
  } else if (command.startsWith("REMOVE ")) {
    String positionStr = command.substring(7);
    positionStr.trim();

    if (positionStr.startsWith("LEFT") || positionStr.startsWith("RIGHT")) {
      bool isLeft = positionStr.startsWith("LEFT");
      int index = positionStr.substring(isLeft ? 4 : 5).toInt() - 1;
      if (index >= 0 && index < 3) {
        if (isLeft) {
          if (leftBoats[index] != NO_BOAT) {
            Serial.println("Barco removido de " + positionStr);
            leftBoats[index] = NO_BOAT;
            controlarLEDs();
          } else {
            Serial.println("No hay barco en " + positionStr + " para remover");
          }
        } else {
          if (rightBoats[index] != NO_BOAT) {
            Serial.println("Barco removido de " + positionStr);
            rightBoats[index] = NO_BOAT;
            controlarLEDs();
          } else {
            Serial.println("No hay barco en " + positionStr + " para remover");
          }
        }
      } else {
        Serial.println("Índice de posición inválido");
      }
    } else {
      Serial.println("Nombre de posición inválido");
    }
  } else {
    Serial.println("Comando desconocido");
  }
}

// Función para convertir el tipo de barco de cadena a entero
int getBoatTypeFromString(String typeStr) {
  if (typeStr == "PATRULLA") {
    return PATRULLA;
  } else if (typeStr == "NORMAL") {
    return NORMAL;
  } else if (typeStr == "PESQUERO") {
    return PESQUERO;
  } else {
    return NO_BOAT;
  }
}

// Función para convertir el tipo de barco de entero a cadena
String getBoatTypeAsString(int boatType) {
  switch (boatType) {
    case NO_BOAT:
      return "No hay barco";
    case PATRULLA:
      return "Patrulla";
    case NORMAL:
      return "Normal";
    case PESQUERO:
      return "Pesquero";
    default:
      return "Desconocido";
  }
}

// Función para generar barcos según el lado seleccionado mediante botones físicos
void generarBarco(int boatType, int lado) {
  if (lado == 1) {  // Lado izquierdo
    for (int i = 0; i < 3; i++) {
      if (leftBoats[i] == NO_BOAT) {
        leftBoats[i] = boatType;
        Serial.println("Barco generado en LEFT" + String(i + 1));
        controlarLEDs();  // Actualizar LEDs después de generar un barco
        break;
      }
    }
  } else if (lado == 2) {  // Lado derecho
    for (int i = 0; i < 3; i++) {
      if (rightBoats[i] == NO_BOAT) {
        rightBoats[i] = boatType;
        Serial.println("Barco generado en RIGHT" + String(i + 1));
        controlarLEDs();  // Actualizar LEDs después de generar un barco
        break;
      }
    }
  }
}

// Función para actualizar el flujo del canal basado en el joystick
void actualizarFlujo() {
  int joystickValue = analogRead(joystickX);  // Lee el valor del eje X

  // Cambiar la dirección basada en el valor del joystick
  if (joystickValue < 400) {
    direction = 0;  // Gira a la izquierda (invertido)
    if (!flujoIzquierdaADerecha) {  // Solo cambiar si es necesario
      flujoIzquierdaADerecha = true;  // Inversión del sentido
      setDirection();  // Actualizar la dirección del motor
    }
  } else if (joystickValue > 600) {
    direction = 1;  // Gira a la derecha (invertido)
    if (flujoIzquierdaADerecha) {  // Solo cambiar si es necesario
      flujoIzquierdaADerecha = false;  // Inversión del sentido
      setDirection();  // Actualizar la dirección del motor
    }
  }
}

// Función para controlar el parpadeo de los LEDs de dirección
void controlarParpadeoLEDs(unsigned long currentMillis) {
  if (currentMillis - previousMillisLED >= intervalLED) {
    previousMillisLED = currentMillis;

    // Invertimos la lógica para que los LEDs correspondan al nuevo flujo invertido
    if (!flujoIzquierdaADerecha) {  // Invertimos la condición
      digitalWrite(ledIzquierdaDerecha, !digitalRead(ledIzquierdaDerecha));  // LED de izquierda a derecha
      digitalWrite(ledDerechaIzquierda, LOW);  // Asegurarse de que el otro LED esté apagado
    } else {
      digitalWrite(ledDerechaIzquierda, !digitalRead(ledDerechaIzquierda));  // LED de derecha a izquierda
      digitalWrite(ledIzquierdaDerecha, LOW);  // Asegurarse de que el otro LED esté apagado
    }
  }
}


// Función para controlar los LEDs en función del estado de los barcos
void controlarLEDs() {
  // Controlar LEDs del lado izquierdo (ajustado para el orden físico 3,2,1)
  digitalWrite(ledIzquierda1, leftBoats[2] != NO_BOAT ? HIGH : LOW); // LED físico LEFT3
  digitalWrite(ledIzquierda2, leftBoats[1] != NO_BOAT ? HIGH : LOW); // LED físico LEFT2
  digitalWrite(ledIzquierda3, leftBoats[0] != NO_BOAT ? HIGH : LOW); // LED físico LEFT1

  // Controlar LEDs del lado derecho (ajustado para el orden físico 3,2,1)
  digitalWrite(ledDerecha1, rightBoats[2] != NO_BOAT ? HIGH : LOW); // LED físico RIGHT3
  digitalWrite(ledDerecha2, rightBoats[1] != NO_BOAT ? HIGH : LOW); // LED físico RIGHT2
  digitalWrite(ledDerecha3, rightBoats[0] != NO_BOAT ? HIGH : LOW); // LED físico RIGHT1
}

// Función para enviar datos por serial cada segundo
void enviarDatosSerial() {
  // Construir una cadena con toda la información
  String mensaje = "";

  // Sentido del canal
  mensaje += "Sentido: ";
  mensaje += (flujoIzquierdaADerecha ? "->" : "<-");  // Cambio realizado aquí
  mensaje += "\n";

  // Cola Izquierda (invertimos el orden para reflejar el orden físico)
  mensaje += "Cola Izq: ";
  for (int i = 2; i >= 0; i--) {
    mensaje += getBoatTypeAbbreviation(leftBoats[i]);
    if (i > 0) mensaje += ",";
  }
  mensaje += "\n";

  // Cola Derecha (invertimos el orden para reflejar el orden físico)
  mensaje += "Cola Der: ";
  for (int i = 2; i >= 0; i--) {
    mensaje += getBoatTypeAbbreviation(rightBoats[i]);
    if (i > 0) mensaje += ",";
  }
  mensaje += "\n";

  // Enviar el mensaje completo
  Serial.print(mensaje);
}

// Función para obtener la abreviatura del tipo de barco
char getBoatTypeAbbreviation(int boatType) {
  switch (boatType) {
    case NO_BOAT:
      return '-';
    case PATRULLA:
      return 'P';
    case NORMAL:
      return 'N';
    case PESQUERO:
      return 'S';  // Usamos 'S' para 'Pesquero'
    default:
      return '?';
  }
}

// Función para establecer la dirección del motor
void setDirection() {
  if (direction == 0) {
    Serial.println("Motor girando: ->");  // Flecha invertida
  } else {
    Serial.println("Motor girando: <-");  // Flecha invertida
  }
}



// Función para activar las bobinas del motor paso a paso
void setStep(int step) {
  digitalWrite(in1, steps[step][0]);
  digitalWrite(in2, steps[step][1]);
  digitalWrite(in3, steps[step][2]);
  digitalWrite(in4, steps[step][3]);
}

// Función para debouncing de los botones
bool debounce(int pin) {
  bool buttonState = digitalRead(pin);
  if (buttonState == LOW) {
    if (millis() - lastDebounceTime > debounceDelay) {
      lastDebounceTime = millis();  // Actualizamos el tiempo de debounce
      return true;  // Solo se detecta un pulso
    }
  }
  return false;
}

// Función para contar el número de barcos en un lado
int countBoats(int boats[]) {
  int count = 0;
  for (int i = 0; i < 3; i++) {
    if (boats[i] != NO_BOAT) count++;
  }
  return count;
}
