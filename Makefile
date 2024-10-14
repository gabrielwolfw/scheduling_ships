# Nombre del ejecutable
TARGET = simulacion

# Archivos fuente
SRC = programa.c CEThreads.c calendarizacion.c barco.c canal.c configuracion.c

# Opciones de compilación
CFLAGS = -Wall -Wextra

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	@gcc -o $(TARGET) $(SRC) $(CFLAGS) -lpthread

# Regla para ejecutar el programa
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET) hardware/scriptComandoIndividual *.o

# Comandos de ejecucion
hardware:
	@gcc -o hardware/scriptComandoIndividual hardware/scriptComandoIndividual.c canal.c barco.c CEThreads.c calendarizacion.c

runHardware:
	@./hardware/scriptComandoIndividual