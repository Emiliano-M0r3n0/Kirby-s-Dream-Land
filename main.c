/**
 * @file main.c
 * @author Moreno Silva Victor Emiliano (vmorenos2200@alumno.ipn.mx)
 * @brief Kirbys Dreamland
 * @version 0.1
 * @date 2025-12-22A
 * 
 * @copyright Copyright (c) 2025
 * 
 * Numero de boleta 2026640066
 * Grupo: 1MM3
 */
#include "graficos.h"
#include "serial.h"
#include "simplecontroller.h"
#include "control_esp.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


//RUTAS PARA LOS SPRITES DE KIRBY
#define RUTA_KIRBY "Kirby/Kirby.bmp"
#define RUTA_KIRBYMASK "Kirby/Kirbymask.bmp"

#define RUTA_KIRBY2 "Kirby/Kirby2.bmp"
#define RUTA_KIRBY2MASK "Kirby/Kirby2mask.bmp"

#define RUTA_KIRBY4 "Kirby/Kirby4.bmp"
#define RUTA_KIRBY4MASK "Kirby/Kirby4mask.bmp"

#define RUTA_KIRBY5 "Kirby/Kirby5.bmp"
#define RUTA_KIRBY5MASK "Kirby/Kirby5mask.bmp"

#define RUTA_KIRBY6 "Kirby/Kirby6.bmp"
#define RUTA_KIRBY6MASK "Kirby/Kirby6mask.bmp"

//Pines para el ESP32
#define JX 35
#define JY 34
#define MTR 19
#define BUTTON_JUMP 26
#define BUTTON_ACTION 25
#define BUTTON_DOWN 27

typedef struct {
    Imagen **frames; //Puntero a puntero porque es es un arreglo de punteros
    int total_frames;
    int frame_actual; //Nos indica en que frame nos encontramos
    int delay_frames; //Delay entre cada frame
    int contador; 
} Animacion;

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

int main()
{
    //Elementos para imagenes
    const int ESCALA = 35;
    const int DELAY = 50;

    //Configuraciones de la ventana
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800,600);

    Imagen *kirby = ventana.creaImagenConMascara(RUTA_KIRBY,RUTA_KIRBYMASK);
    Imagen *kirby2 = ventana.creaImagenConMascara(RUTA_KIRBY2,RUTA_KIRBY2MASK);
    Imagen *kirby4 = ventana.creaImagenConMascara(RUTA_KIRBY4,RUTA_KIRBY4MASK);
    Imagen *kirby5 = ventana.creaImagenConMascara(RUTA_KIRBY5,RUTA_KIRBY5MASK); 
    Imagen *kirby6 = ventana.creaImagenConMascara(RUTA_KIRBY6,RUTA_KIRBY6MASK);

    Imagen *caminar_derecha[] = { kirby4, kirby6, kirby5 };

    Animacion anim_derecha = {caminar_derecha,3,0,5,};

    //Configuraciones y calibraciones del Esp32
    Board *esp32 = connectDevice("COM6",B115200);
    esp32->pinMode(esp32,JX,INPUT);
    esp32->pinMode(esp32,JY,INPUT);
    esp32->pinMode(esp32,MTR,OUTPUT);
    esp32->pinMode(esp32,BUTTON_JUMP,INPUT_PULLUP);
    esp32->pinMode(esp32,BUTTON_ACTION,INPUT_PULLUP);
    esp32->pinMode(esp32,BUTTON_DOWN,INPUT_PULLUP);

    EjeJoystick EjeX;
    EjeJoystick EjeY;

    ini_joystick(&EjeX,esp32,JX);
    ini_joystick(&EjeY,esp32,JY);

    int tecla = ventana.teclaPresionada();
    ventana.colorFondo(COLORES.NEGRO);
    ventana.limpiaVentana();
    ventana.actualizaVentana();
    ventana.espera(100);

    ini_calibracion(&EjeX,500);
    ini_calibracion(&EjeY,500);

    float v = 0.0f;
    bool btn_action = false;
    bool btn_jump = false;
    bool btn_down = false;

while (1)
{
    btn_action = esp32->digitalRead(esp32,BUTTON_ACTION);
    btn_jump = esp32->digitalRead(esp32,BUTTON_JUMP);
    btn_down = esp32->digitalRead(esp32,BUTTON_DOWN);

    v = leer_joystick(&EjeX);

    ventana.imprimeEnConsola("LensX: %.2f | Btn-action: %i | Btn-jump : %i | Btn-down: %i\n",v,btn_action,btn_jump,btn_down);
 
    if (!btn_action)
    {
        esp32->digitalWrite(esp32,MTR,true);
    }
    else
    {
        esp32->digitalWrite(esp32,MTR,false);
    }
    
    ventana.espera(50);
}
return 0;
}