#include "control_esp.h"
#include "graficos.h"
#include <math.h>

#define ALPHA 0.1f
#define DEADZONE 0.1f

void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin)
{
    Eje->board = board;
    Eje->pin = pin;
    Eje->centro = 0.5f;
    Eje->filtrado = 0.5f;

    Eje->suma = 0.0f;
    Eje->muestras_actuales = 0;
    Eje->muestras_objetivo = 0;
    Eje->calibrado = false;

}

void ini_calibracion(EjeJoystick *Eje, int muestras)
{
    Eje->suma = 0.0f;
    Eje->muestras_actuales = 0;
    Eje->muestras_objetivo = muestras;
    Eje->calibrado = false;
}

void actualizar_calibracion(EjeJoystick *Eje)
{
    if (Eje->calibrado)return;

    float v = Eje->board->analogRead(Eje->board,Eje->pin);
    Eje->suma += v;
    Eje->muestras_actuales++;
    
    if (Eje->muestras_actuales >= Eje->muestras_objetivo)
    {
        Eje->centro = Eje->suma / Eje->muestras_objetivo;
        Eje->filtrado = Eje->centro;
        Eje->calibrado = true;
    }
    
}

float leer_joystick(EjeJoystick *Eje)
{
    float raw = Eje->board->analogRead(Eje->board,Eje->pin);
    ventana.imprimeEnConsola("RAW: %.3f\n", raw);

    Eje->filtrado = ALPHA * raw + (1.0f - ALPHA) * Eje->filtrado;

    float value = Eje->filtrado - Eje->centro;

    if (fabs(value) < DEADZONE)
    {
        return 0.0f;
    }
    
    return value;
}