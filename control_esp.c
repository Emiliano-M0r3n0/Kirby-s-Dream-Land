#include "control_esp.h"
#include "graficos.h"
#include <math.h>

void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin)
{
    Eje->board = board;
    Eje->pin = pin;
    Eje->offset = 0.0f;
}

void ini_calibracion(EjeJoystick *Eje, int muestras)
{
    float suma = 0.0f;

    for (int i = 0; i < muestras; i++) {
        suma += Eje->board->analogRead(Eje->board, Eje->pin);
        ventana.texto(ventana.anchoVentana()/2,ventana.altoVentana()/2,"Calibrando Joystick...");
        ventana.espera(5);
    }

    float promedio = suma / (float)muestras;
    Eje->offset = promedio - 0.5f;
}

float leer_joystick(EjeJoystick *eje)
{
    float v = 0.0f;
    v = eje->board->analogRead(eje->board, eje->pin) - 0.5f - eje->offset;

    return v;
}