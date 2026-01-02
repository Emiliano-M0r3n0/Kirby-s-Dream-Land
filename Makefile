EJECUTABLE = main.exe
MAIN = main.c

#Incluye las librerias que necesites, se incluye el .c, todas tus librerias se colocan en una misma variable, separalas por espacios
#---------------------------------
LIBS = graficos.c serial.c simplecontroller.c control_esp.c kirby_assets.c
#Reglas para permitir el enlazado correcto
OBJ = $(LIBS:.c=.o)
DEPS = $(OBJ:.o=.d)
#---------------------------------

#Escoge el compilador que necesites
#---------------------------------
CC = gcc
#---------------------------------

#Añade las banderas que necesites, segun sean de compilación, enlazado, etc.
#---------------------------------
LDFLAGS += -mwindows -lwinmm
#---------------------------------

#Regla principal
run: $(EJECUTABLE)
	./$(EJECUTABLE)

$(EJECUTABLE): $(MAIN) $(OBJ)
	$(CC) $(MAIN) $(OBJ) -o $@ $(LDFLAGS)
#MD crea un archivo .d con el que revisamos todas las dependencias .h
%.o: %.c #El %.o: %.c crea una regla general que para todo archivo .c se pueda generar un .o
	$(CC) -MD -c $< -o $@ 
-include $(DEPS) #Incluye los archivos .d si es que existen, el - evita errores
#Elimina los archivos temporales
clean:
	rm -f *.o *.d $(EJECUTABLE) 