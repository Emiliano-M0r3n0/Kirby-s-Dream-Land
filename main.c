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

#define RUTA_KIRBY3 "Kirby/Kirby3.bmp"
#define RUTA_KIRBY3MASK "Kirby/Kirby3mask.bmp"

#define RUTA_KIRBY4 "Kirby/Kirby4.bmp"
#define RUTA_KIRBY4MASK "Kirby/Kirby4mask.bmp"

#define RUTA_KIRBY5 "Kirby/Kirby5.bmp"
#define RUTA_KIRBY5MASK "Kirby/Kirby5mask.bmp"

#define RUTA_KIRBY6 "Kirby/Kirby6.bmp"
#define RUTA_KIRBY6MASK "Kirby/Kirby6mask.bmp"

#define RUTA_KIRBY7 "Kirby/Kirby7.bmp"
#define RUTA_KIRBY7MASK "Kirby/Kirby7mask.bmp"

#define RUTA_KIRBY8 "Kirby/Kirby8.bmp"
#define RUTA_KIRBY8MASK "Kirby/Kirby8mask.bmp"

#define RUTA_KIRBY9 "Kirby/Kirby9.bmp"
#define RUTA_KIRBY9MASK "Kirby/Kirby9mask.bmp"

#define RUTA_KIRBY11 "Kirby/Kirby11.bmp"
#define RUTA_KIRBY11MASK "Kirby/Kirby11mask.bmp"

#define RUTA_KIRBY12 "Kirby/Kirby12.bmp"
#define RUTA_KIRBY12MASK "Kirby/Kirby12mask.bmp"

#define RUTA_KIRBY13 "Kirby/Kirby13.bmp"
#define RUTA_KIRBY13MASK "Kirby/Kirby13mask.bmp"

#define RUTA_KIRBY14 "Kirby/Kirby14.bmp"
#define RUTA_KIRBY14MASK "Kirby/Kirby14mask.bmp"

#define RUTA_KIRBY15 "Kirby/Kirby15.bmp"
#define RUTA_KIRBY15MASK "Kirby/Kirby15mask.bmp"

#define RUTA_KIRBY16 "Kirby/Kirby16.bmp"
#define RUTA_KIRBY16MASK "Kirby/Kirby16mask.bmp"

#define RUTA_KIRBY17 "Kirby/Kirby17.bmp"
#define RUTA_KIRBY17MASK "Kirby/Kirby17mask.bmp"

#define RUTA_KIRBY18 "Kirby/Kirby18.bmp"
#define RUTA_KIRBY18MASK "Kirby/Kirby18mask.bmp"

#define RUTA_KIRBY19 "Kirby/Kirby19.bmp"
#define RUTA_KIRBY19MASK "Kirby/Kirby19mask.bmp"

#define RUTA_KIRBY20 "Kirby/Kirby20.bmp"
#define RUTA_KIRBY20MASK "Kirby/Kirby20mask.bmp"

#define RUTA_KIRBY21 "Kirby/Kirby21.bmp"
#define RUTA_KIRBY21MASK "Kirby/Kirby21mask.bmp"

#define RUTA_KIRBY22 "Kirby/Kirby22.bmp"
#define RUTA_KIRBY22MASK "Kirby/Kirby22mask.bmp"

#define RUTA_KIRBY23 "Kirby/Kirby23.bmp"
#define RUTA_KIRBY23MASK "Kirby/Kirby23mask.bmp"

#define RUTA_NUBE "Kirby/Nube.bmp"
#define RUTA_NUBEMASK "Kirby/Nubemask.bmp"

//Pines para el ESP32
#define JX 35
#define JY 34
#define MTR 19
#define BUTTON_JUMP 26
#define BUTTON_ACTION 25
#define BUTTON_DOWN 27

//Constantes
#define SPEED_X 400.0f
#define SALTO_FUERZA 450.0f
#define GRAVEDAD 1200.0f
#define DT (1.0f / 60.0f)

int main()
{
    //Elementos para imagenes
    const int ESCALA = 65;
    const int DELAY = 50;

    //Configuraciones de la ventana
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800,600);

    Imagen *kirby = ventana.creaImagenConMascara(RUTA_KIRBY,RUTA_KIRBYMASK);
    Imagen *kirby2 = ventana.creaImagenConMascara(RUTA_KIRBY2,RUTA_KIRBY2MASK);
    Imagen *kirby4 = ventana.creaImagenConMascara(RUTA_KIRBY4,RUTA_KIRBY4MASK);
    Imagen *kirby5 = ventana.creaImagenConMascara(RUTA_KIRBY5,RUTA_KIRBY5MASK); 
    Imagen *kirby6 = ventana.creaImagenConMascara(RUTA_KIRBY6,RUTA_KIRBY6MASK);
    Imagen *kirby7 = ventana.creaImagenConMascara(RUTA_KIRBY7,RUTA_KIRBY7MASK);
    Imagen *kirby8 = ventana.creaImagenConMascara(RUTA_KIRBY8,RUTA_KIRBY8MASK);
    Imagen *kirby9 = ventana.creaImagenConMascara(RUTA_KIRBY9,RUTA_KIRBY9MASK);
    Imagen *kirby11 = ventana.creaImagenConMascara(RUTA_KIRBY11,RUTA_KIRBY11MASK);
    Imagen *kirby12 = ventana.creaImagenConMascara(RUTA_KIRBY12,RUTA_KIRBY12MASK);
    Imagen *kirby13 = ventana.creaImagenConMascara(RUTA_KIRBY13,RUTA_KIRBY13MASK);
    Imagen *kirby14 = ventana.creaImagenConMascara(RUTA_KIRBY14,RUTA_KIRBY14MASK);
    Imagen *kirby15 = ventana.creaImagenConMascara(RUTA_KIRBY15,RUTA_KIRBY15MASK);
    Imagen *kirby16 = ventana.creaImagenConMascara(RUTA_KIRBY16,RUTA_KIRBY16MASK);
    

    Imagen *caminar_derecha[] = { kirby4, kirby6, kirby5 };
    Imagen *sprite_quieto = kirby;
    Imagen *sprite_aireidle = kirby2;
    Imagen *sprite_actual;

    Animacion anim_derecha = {caminar_derecha,3,0,5,0};

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

    Lectura lectura_general;

    ini_joystick(&EjeX,esp32,JX);
    ini_joystick(&EjeY,esp32,JY);

    ini_lens(&lectura_general,esp32,BUTTON_ACTION,BUTTON_JUMP,BUTTON_DOWN,MTR);

    int tecla = ventana.teclaPresionada();
    ventana.colorFondo(COLORES.NEGRO);
    ventana.limpiaVentana();
    ventana.actualizaVentana();
    ventana.espera(100);

    ini_calibracion(&EjeX,500);
    ini_calibracion(&EjeY,500);

    float posX = 20.0f;
    float posY = ventana.altoVentana() - 20;

    float velX = 0.0f;
    float velY = 0.0f;

    bool en_aire = false;
    float velocidad_salto = 0.0f;

while (1)
{
leer_entrada(&lectura_general, &EjeX, false);

//Movimiento horizontal
velX = lectura_general.lensX * SPEED_X;
posX += velX * DT;

//Limites de la pantalla
if (posX < 0) posX = 0;
if (posX > ventana.anchoVentana() - ESCALA)
    posX = ventana.anchoVentana() - ESCALA;

/* ---- SALTO ---- */
if (!lectura_general.lens_jump && !en_aire)
{
    en_aire = true;
    velocidad_salto = SALTO_FUERZA;
    sprite_actual = sprite_aireidle;
}

if (en_aire)
{
    velocidad_salto -= GRAVEDAD * DT;
    posY -= velocidad_salto * DT;

    //Suelo (Limite del eje Y)
    if (posY >= ventana.altoVentana() - 50)
    {
        posY = ventana.altoVentana() - 50;
        en_aire = false;
        velocidad_salto = 0.0f;
    }
}

/* ---- SELECCIÓN DE SPRITE ---- */

if (fabs(velX) > 5.0f)
{
    sprite_actual = animacion_actual(&anim_derecha);
}
else
{
    sprite_actual = sprite_quieto;
}

/* ---- DIBUJO ---- */
ventana.limpiaVentana();
ventana.muestraImagenEscalada(
    (int)posX,
    (int)posY,
    ESCALA,
    ESCALA,
    sprite_actual
);
ventana.actualizaVentana();
ventana.espera(10);


}
return 0;
}