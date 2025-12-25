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
        ventana.actualizaVentana();
        ventana.limpiaVentana();
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

void ini_lens(Lectura *lens,Board *board,uint8_t pin_action, uint8_t pin_jump, uint8_t pin_down,uint8_t pin_mtr)
{
    lens->board = board;
    lens->pin_action = pin_action;
    lens->pin_down = pin_down;
    lens->pin_jump = pin_jump;
    lens->pin_mtr = pin_mtr;

    lens->lens_action = true;
    lens->lens_jump = true;
    lens->lens_down = true;
}

void leer_entrada(Lectura *lens,EjeJoystick *EjeX,bool terminal)
{
    lens->lens_jump = lens->board->digitalRead(lens->board,lens->pin_jump);
    lens->lens_action = lens->board->digitalRead(lens->board,lens->pin_action);
    lens->lens_down = lens->board->digitalRead(lens->board,lens->pin_down);

    lens->lensX = leer_joystick(EjeX);

    if (!(lens->lens_action)) lens->board->digitalWrite(lens->board,lens->pin_mtr,true);
    else lens->board->digitalWrite(lens->board,lens->pin_mtr,false);
    
    if(terminal)
{    ventana.imprimeEnConsola("LensX: %.2f | Btn-action: %i | Btn-jump : %i | Btn-down: %i\n",
        lens->lensX,
        lens->lens_action,
        lens->lens_jump,
        lens->lens_down);
    }
    ventana.espera(30);
}

Imagen* animacion_actual(Animacion *anim)
{
    anim->contador++;

    if (anim->contador >= anim->delay_frames) {
        anim->contador = 0;
        anim->frame_actual++;
        if (anim->frame_actual >= anim->total_frames) {
            anim->frame_actual = 0;
        }
    }

    return anim->frames[anim->frame_actual];
}
