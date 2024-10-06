# Nombre del ejecutable
TARGET = test_program

# Archivos fuente
SRC = main.c CEThreads.c Utils/Scheduler.c

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
	@rm -f $(TARGET)
