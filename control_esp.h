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

// Definicion de fondos
#define FO_A 0
#define FO_B 1
#define FO_C 2
#define FO_D 3
#define FO_Aa 4
#define FO_COUNT 5

// Definicion de enemigos
#define EN_BRONTO 0
#define EN_CAPPY 1
#define EN_GRIZZO 2
#define EN_MANZANA 3
#define EN_POPPY 4
#define EN_TWIZZY 5
#define EN_WADLE 6
#define EN_COUNT 7
// Definicion de estados de Kirby
#define ST_IDLE 0
#define ST_WALKING 1
#define ST_JUMPING 2
#define ST_EATING 3
#define ST_FAT_IDLE 4
#define ST_FAT_FLYING 5
#define ST_SPITTING 6
#define ST_FAT_WALKING 7
#define ST_INHALE 8

// Definicion de animaciones de Kirby
#define AN_IDLE 0
#define AN_WALK 1
#define AN_JUMP 2
#define AN_EAT 3
#define AN_FAT_IDLE 4
#define AN_FAT_WALK 5
#define AN_FLY 6
#define AN_FAT_FALL 7
#define AN_SPIT 8
#define AN_INHALE 9
#define AN_COUNT 10 // Numero total de animaciones

// Defincion de constantes
#define SPEED_X 400.0f
#define SALTO_FUERZA 450.0f
#define GRAVEDAD 1200.0f
#define DT (1.0f / 60.0f)

/// @brief Estructura que define la cajita conocida como hitbox
typedef struct Hitbox
{
    float x, y;
    float height, width;
} Hitbox;

typedef struct Fondos {
    Imagen *img[FO_COUNT];
    int alto_original[FO_COUNT];
    int ancho_original[FO_COUNT];
    float escalas[FO_COUNT];
    float ancho_escalado[FO_COUNT];
    float alto_escalado[FO_COUNT];
    bool cargado[FO_COUNT];
} Fondos;

/// @brief Estructura utilizada para encapsular las lecturas del joystick
typedef struct EjeJoystick
{

    Board *board;
    uint8_t pin;
    float offset; // diferencia respecto a 0.5

} EjeJoystick;

/// @brief Almacena los datos de un boton y sus pulsasiones
typedef struct Boton
{
    Board *board;
    uint8_t pin;
    bool actual;
    bool previo;
} Boton;

/// @brief Estructura utilizada para encapsular las lecturas de los botones y del joystick
typedef struct Lectura
{

    Board *board;
    float lensX; // Se guarda el resultado de la funcion leer_joystick

    Boton button_action;
    Boton button_jump;
    Boton button_down;

    uint8_t pin_down;
    uint8_t pin_mtr;

    bool mtr_on;

} Lectura;

/// @brief Estructura que alberga un arreglo de frames (Imagenes) y permite manipularlos como animacion
typedef struct Animacion
{

    Imagen **frames;  // Puntero a puntero porque es es un arreglo de punteros
    int total_frames; // Numero total de frames
    int frame_actual; // Nos indica en que frame nos encontramos
    int delay_frames; // Delay entre cada frame
    int contador;

    bool bucle;

} Animacion;

/// @brief Estructura que describe las fisicas y estados del proyectil de kirby
typedef struct Proyectil
{

    float x, y;      // Posición
    float velX;      // Velocidad horizontal
    bool activo;     // Si está activo en pantalla
    bool esEstrella; // true = estrella, false = humo
    Hitbox hitbox;
    int timerdisparo;                    // Tiempo de vida
    Animacion arregloProyectil[2];       // 2 Animaciones estrella y humo
    Animacion arregloProyectilMirror[2]; // Estrella 0 y humo 1
    Animacion *animActual;
} Proyectil;

typedef struct Camara
{

    float x, y;
    float width, height;
    float fondo_width, fondo_height;

} Camara;

/// @brief Estructura que alberga todas las variables necesarias para el funcionamiento de kirby
typedef struct Kirby
{
    // Timer
    float timerAccion;
    // Posicion y velocidades
    float x, y;
    float velX, velY;
    int estado;
    bool mirandoDerecha;
    // Variables que guardan el pulso (flancos)
    bool jump_p;
    bool action_p;
    // Puntero a la animacion actual
    Animacion *animActual;
    // Arreglo en el que se encuentran todas las animaciones
    Animacion arregloAnim[AN_COUNT];
    Animacion arregloAnimMirror[AN_COUNT];
    Proyectil proyectil;

    Hitbox kirbyhitbox; // Hitbox kirby

    Hitbox succionhitbox;

    Camara camara;

    Fondos fondo;
    bool stomach_wenemie; // Estomago con enemigo
    bool Gordito;         // Gordito o no gordito
    int enemies_eaten;    // Numero de enemigos que ha comido

} Kirby;

/// @brief Estructura de un enemigo
typedef struct Enemigo
{
    float x, y;
    Hitbox hitbox;
    Animacion arregloEnemies[EN_COUNT];
    Animacion *animActual;
    int typeenemie;
    bool activo;
} Enemigo;

/// @brief Inicializa los valores de offset, los pines y la placa vinculada al joystick
/// @param Eje Direccion del Eje el cual inicializaremos
/// @param board Placa a la cual esta vinculada
/// @param pin Pin al que esta conectado el eje del joystick
void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin);

/// @brief Detecta los flancos y si un boton fue presionado o soltado
/// @param button Boton a detectar
/// @param board Placa a la cual esta conectado
/// @return true en caso de presionado, false en caso de soltado
bool fue_presionado(Boton *button, Board *board);

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
void ini_lens(Lectura *lens, Board *board, uint8_t pin_action, uint8_t pin_jump, uint8_t pin_down, uint8_t pin_mtr);

/// @brief Lee todos los valores, entradas digitales y analogicas
/// @param lens Direccion de la variable donde se almacenan los datos
/// @param EjeX Direccion del eje que queremos leer
/// @param terminal (true) en caso de querer ver una terminal con los valores que leemos
void leer_entrada(Lectura *lens, EjeJoystick *EjeX, bool terminal);

/// @brief Selecciona un frame del arreglo y lo devuelve generando la animacion
/// @param anim Animacion deseada
/// @return Frame del arreglo
Imagen *animacion_actual(Animacion *anim);

/// @brief Carga los datos necesarios a la estructura animacion, con las rutas ingresadas se crean las imagenes
/// @param rutas_img Rutas de las imagenes pertenecientes a la animacion
/// @param rutas_mask Rutas de las mascaras
/// @param total_frames Numero total de imagenes
/// @param delay_frames Numero de frames de delay
/// @param anim Estructura en la que se guardara
void cargar_animacion(const char **rutas_img, const char **rutas_mask, int total_frames, int delay_frames, Animacion *anim);

/// @brief Detecta la colision entre dos hitbox
/// @param a Hitbox a
/// @param b Hitbox b
/// @return true en caso de que colisionen, false en caso de que no
bool hitbox_colision(Hitbox a, Hitbox b);

/// @brief Actualiza la hitbox de kirby tras un cambio de posicion
/// @param k Direccion de la variable kirby
void actualizar_hitbox_kirby(Kirby *k);

/// @brief Actualiza la hitbox generada para succionar un enemigo tras un cambio de posicon
/// @param k Direccion de la variable kirby
void actualizar_hitbox_succion(Kirby *k);

/// @brief Actualiza la hitbox de un enemigo tras un cambio de posicion
/// @param enemie Direccion de la variable enemigo
void actualizar_hitbox_enemie(Enemigo *enemie);

/// @brief Crea un proyectil en funcion de la posicion 'x' y 'y', segun la orientacion de kirby y si tiene un enemigo en su estomago
/// @param p Direccion de la variable tipo proyectil
/// @param x Coordenada X inicial
/// @param y Coordenada Y inicial
/// @param mirandoDerecha Orientacion de kirby 'true' derecha 'false' izq
/// @param esEstrella 'true' tiene un enemigo escupe estrella 'false' no tiene enemigo por lo tanto escupe humo
void crear_proyectil(Proyectil *p, float x, float y, bool mirandoDerecha, bool esEstrella);

/// @brief Actualiza las fisicas del proyectil en base a si esta activo o no
/// @param p Direccion de la variable tipo proyectil
/// @param dt Variable DT
void actualizar_proyectil(Proyectil *p, float dt);

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
void seleccionar_animacion_kirby(Kirby *k);

/// @brief En base al tipo de enemigo, decide que animacion de enemigo selecciona
/// @param enemies Direccion de la variable tipo enemigo
void seleccionar_enemies(Enemigo *enemies);

/// @brief En base a el contenido del estomago de kirby decide que proyectil dispara
/// @param kirby Direccion de la variable tipo kirby
void seleccionar_proyectil(Kirby *kirby);

void crear_fondos(Fondos *fondos, const char **rutas_fondos);

/// @brief Crea y actualiza las escalas y medidas de los fondos
/// @param fondos Direccion de la variable fondos
void crear_escalas_fondos(Fondos *fondos);

/// @brief Dibujar el fondo con la escala pertinente
/// @param fondo Direccion de la variable fondo
/// @param indice Fondo que quieres imprimir
void dibujar_fondo(Fondos *fondo,int indice);

#endif