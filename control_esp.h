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

    Board *board;
    uint8_t pin;
    float offset;   // diferencia respecto a 0.5

}EjeJoystick;

void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin);

void ini_calibracion(EjeJoystick *Eje, int muestras);

float leer_joystick(EjeJoystick *eje);

#endif