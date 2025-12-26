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

typedef struct EjeJoystick{

    Board *board;
    uint8_t pin;
    float offset;   // diferencia respecto a 0.5

}EjeJoystick;

typedef struct Lectura{

    Board *board;
    float lensX;

    bool lens_action;
    bool lens_jump;
    bool lens_down;

    uint8_t pin_action;
    uint8_t pin_jump;
    uint8_t pin_down;
    uint8_t pin_mtr;

}Lectura;

typedef struct Animacion{

    Imagen **frames; //Puntero a puntero porque es es un arreglo de punteros
    int total_frames; //Numero total de frames
    int frame_actual; //Nos indica en que frame nos encontramos
    int delay_frames; //Delay entre cada frame
    int contador; 

} Animacion;

/// @brief Inicializa los valores de offset, los pines y la placa vinculada al joystick
/// @param Eje Direccion del Eje el cual inicializaremos
/// @param board Placa a la cual esta vinculada
/// @param pin Pin al que esta conectado el eje del joystick
void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin);

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

Imagen* animacion_actual(Animacion *anim);

#endif