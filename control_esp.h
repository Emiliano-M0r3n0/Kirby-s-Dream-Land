/**
 * @file joystick.h
 * @author Moreno Silva Victor Emiliano (vmorenos2200@alumno.ipn.mx)
 * @brief 
 * @version 0.1
 * @date 2025-12-22
 * 
 * @copyright Copyright (c) 2025
 * 
 * Numero de boleta 2026640066
 * Grupo: 1MM3
 */
#ifndef __CONTROLESP__
#define __CONTROLESP__

#include "simplecontroller.h"
#include "graficos.h"
#include <stdint.h>
#include <stdbool.h>

// Definicion de estados de Kirby
#define ST_IDLE          0
#define ST_WALKING       1
#define ST_JUMPING       2
#define ST_EATING        3
#define ST_FAT_IDLE      4
#define ST_FAT_FLYING    5
#define ST_SPITTING      6
#define ST_FAT_WALKING   7

// Definicion de animaciones de Kirby
#define AN_IDLE       0
#define AN_WALK       1
#define AN_JUMP       2
#define AN_EAT        3
#define AN_FAT_IDLE   4
#define AN_FAT_WALK   5
#define AN_FLY        6
#define AN_FAT_FALL   7
#define AN_SPIT       8
#define AN_COUNT      9 //Numero total de animaciones

// Defincion de constantes
#define SPEED_X 400.0f
#define SALTO_FUERZA 450.0f
#define GRAVEDAD 1200.0f
#define DT (1.0f / 60.0f)

/// @brief Estructura utilizada para encapsular las lecturas del joystick
typedef struct EjeJoystick{

    Board *board;
    uint8_t pin;
    float offset;   // diferencia respecto a 0.5

}EjeJoystick;

typedef struct Boton {
    Board *board;
    uint8_t pin;
    bool actual;
    bool previo;
}Boton;

/// @brief Estructura utilizada para encapsular las lecturas de los botones y del joystick
typedef struct Lectura{

    Board *board;
    float lensX; //Se guarda el resultado de la funcion leer_joystick

    Boton button_action;
    Boton button_jump;
    Boton button_down;

    uint8_t pin_down;
    uint8_t pin_mtr;

    bool mtr_on;

}Lectura;

/// @brief Estructura que alberga un arreglo de frames (Imagenes) y permite manipularlos como animacion
typedef struct Animacion{

    Imagen **frames; //Puntero a puntero porque es es un arreglo de punteros
    int total_frames; //Numero total de frames
    int frame_actual; //Nos indica en que frame nos encontramos
    int delay_frames; //Delay entre cada frame
    int contador;
    
    bool bucle;

} Animacion;

typedef struct Kirby {
    // Timers
    float timerAccion;
    // Posición y Física
    float x, y;
    float velX, velY;
    int estado;
    bool mirandoDerecha;
    //Variables que guardan el pulso (flancos)
    bool jump_p;
    bool action_p;
    // Puntero a la animacion actual 
    Animacion *animActual;
    //Arreglo en el que se encuentran todas las animaciones
    Animacion arregloAnim[AN_COUNT];
    
} Kirby;

/// @brief Inicializa los valores de offset, los pines y la placa vinculada al joystick
/// @param Eje Direccion del Eje el cual inicializaremos
/// @param board Placa a la cual esta vinculada
/// @param pin Pin al que esta conectado el eje del joystick
void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin);

bool fue_presionado(Boton *button,Board *board);

/// @brief Inicia la calibracion, hace un promedio de lecturas para obtener el offset
/// @param Eje Direccion del Eje que calibraremos
/// @param muestras Numero de muestras que realizaremos
void ini_calibracion(EjeJoystick *Eje, int muestras);

/// @brief Lee el valor que nos devuelve el joystick
/// @param eje Direccion del Joystick que queremos leer
/// @return Valor ajustado con el offset y el centro
float leer_joystick(EjeJoystick *eje);

/// @brief Inicializa los valores de lectura
/// @param lens Direccion de la variable donde se almacenaran los datos
/// @param board Placa a la que esta conectada
/// @param pin_action Pin al que esta conectado el boton accion
/// @param pin_jump Pin al que esta conectado el boton de salto
/// @param pin_down Pin al que esta conectado el boton de agacharse
/// @param pin_mtr Pin al que esta conectado el motor de vibracion
void ini_lens(Lectura *lens,Board *board,uint8_t pin_action, uint8_t pin_jump, uint8_t pin_down,uint8_t pin_mtr);

/// @brief Lee todos los valores, entradas digitales y analogicas
/// @param lens Direccion de la variable donde se almacenan los datos
/// @param EjeX Direccion del eje que queremos leer
/// @param terminal (true) en caso de querer ver una terminal con los valores que leemos
void leer_entrada(Lectura *lens,EjeJoystick *EjeX,bool terminal);

void reiniciar_animacion(Animacion *anim);

/// @brief Selecciona un frame del arreglo y lo devuelve generando la animacion
/// @param anim Animacion deseada
/// @return Frame del arreglo
Imagen* animacion_actual(Animacion *anim);

/// @brief Carga los datos necesarios a la estructura animacion, con las rutas ingresadas se crean las imagenes
/// @param rutas_img Rutas de las imagenes pertenecientes a la animacion
/// @param rutas_mask Rutas de las mascaras
/// @param total_frames Numero total de imagenes
/// @param delay_frames Numero de frames de delay
/// @param anim Estructura en la que se guardara
void cargar_animacion(const char **rutas_img, const char **rutas_mask,int total_frames,int delay_frames,Animacion *anim); 

/// @brief Inicializa una variable de tipo kirby, inicia su animacion en estado quieto y en las posiciones indicadas
/// @param k Dirección de la variable tipo Kirby
/// @param x Posición inicial donde quieres que aparezca
/// @param y Posicion inicial donde quieres que aparezca
/// @param arregloAnim Direccion del arreglo que alberga todas las animaciones
void ini_kirby(Kirby *k, float x, float y);

/// @brief Actualiza las fisicas y el estado de kirby en base a las lecturas del esp
/// @param k Direccion de la variable tipo Kirby
/// @param input Direccion de la variable tipo lectura que almacena las lecturas del esp
/// @param dt Variable para ajustar los FPS
/// @param anchoVentana Ancho de la ventana (necesaria para los limites)
/// @param altoVentana Alto de la ventana (necesaria para los limites)
void actualizar_kirby(Kirby *k, Lectura *input, float dt, int anchoVentana, int altoVentana);

/// @brief En base al estado de kirby selecciona la animación que tiene que ser plasmada
/// @param k Direccion de la variable tipo Kirby
/// @param idle Animacion en estado quieto
/// @param walk Animacion caminando
/// @param jump Animacion saltando
/// @param eat Animacion comiendo
/// @param fat_idle Animacion gordito en estaado quieto
/// @param fly Animacion volando
/// @param spit Animacion escupiendo
void seleccionar_animacion_kirby(Kirby *k); 
#endif