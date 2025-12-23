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
#include <stdint.h>
#include <stdbool.h>

typedef struct EjeJoystick{

    Board *board; //Dispositivo al que esta conectado
    uint8_t pin; //Pin al que esta conectado
    float centro; //Centro calibrado
    float filtrado; //Valor filtrado previo

    float suma;
    int muestras_objetivo;
    int muestras_actuales;
    bool calibrado; //true = si false = no

}EjeJoystick;

void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin);

void ini_calibracion(EjeJoystick *Eje, int muestras);

void actualizar_calibracion(EjeJoystick *Eje);

float leer_joystick(EjeJoystick *Eje);

#endif