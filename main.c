/**
 * @file main.c
 * @author Moreno Silva Victor Emiliano (vmorenos2200@alumno.ipn.mx)
 * @brief Kirbys Dreamland
 * @version 0.1
 * @date 2025-12-22
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
#define MTR 12
#define BUTTON_JUMP 26
#define BUTTON_ACTION 25

/// @brief Realiza 1 movimiento en direccion horizontal, usa paso por referencia
/// @param x Posicion inicial en X
/// @param y Posicion inicial en Y
/// @param tamanio Tamanio al que se desea ser escalada la imagen
/// @param imagen Imagen que vamos a mover
/// @param incrementoX Cantidad de pixeles que se mueve en X
/// @param incrementoY Cantidad de pixeles que se mueve en Y
/// @param delay Tiempo de espera
void onemov(int *x,int *y,int tamanio,Imagen *imagen,int incrementoX,int incrementoY,int delay)
{
    ventana.muestraImagenEscalada(*x,*y,tamanio,tamanio,imagen);
    ventana.actualizaVentana();
    ventana.espera(delay);
    ventana.limpiaVentana();
    (*x) += incrementoX;
    (*y) += incrementoY;
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

    //Configuraciones y calibraciones del Esp32
    Board *esp32 = connectDevice("COM6",B115200);
    esp32->pinMode(esp32,JX,INPUT);
    esp32->pinMode(esp32,JY,INPUT);
    esp32->pinMode(esp32,MTR,OUTPUT);
    esp32->pinMode(esp32,BUTTON_JUMP,INPUT_PULLUP);
    esp32->pinMode(esp32,BUTTON_ACTION,INPUT_PULLUP);

    EjeJoystick EjeX;
    EjeJoystick EjeY;

    ini_joystick(&EjeX,esp32,JX);
    ini_joystick(&EjeY,esp32,JY);
    
    //Valores donde se guarda la lectura del esp32
    float lecturax;
    float lecturay;

    //Coordenadas iniciales de kirby
    int coordXkirby0 = 20;
    int coordYkirby0 = ventana.altoVentana()-50;

    int tecla = ventana.teclaPresionada();

    ventana.colorFondo(COLORES.NEGRO);
    ventana.limpiaVentana();
    ventana.actualizaVentana();

    ventana.espera(100);

    ini_calibracion(&EjeX,200);
    ini_calibracion(&EjeY,200);

    int i = coordXkirby0;
    int j = coordYkirby0;

    while (tecla != TECLAS.ESCAPE)
    {
        tecla = ventana.teclaPresionada();
        int limitex = ventana.anchoVentana() - 50;

        float test = esp32->analogRead(esp32, JX);
        ventana.imprimeEnConsola("TEST RAW JX = %.3f\n", test);

        actualizar_calibracion(&EjeX);
        actualizar_calibracion(&EjeY);

        if (!EjeX.calibrado || !EjeY.calibrado)
        {
            ventana.limpiaVentana();
           ventana.texto(ventana.anchoVentana()/2,ventana.altoVentana()/2,"Calibrando joystick...");
           ventana.actualizaVentana();
            continue;
        }
        
        lecturax = leer_joystick(&EjeX);
        lecturay = leer_joystick(&EjeY);

        ventana.imprimeEnConsola("LensX: %.2f LensY: %.2f\n",lecturax,lecturay);
        ventana.espera(10);

        while(i < limitex)
        {
            //Comienza y termina de forma estatica
            if(i == j || i >= limitex - 1)
            {
            onemov(&i,&j,ESCALA,kirby,3,0,DELAY);
            continue; //Regresa al inicio del bucle
            }

            //Movimiento con brazo derecho visible
            onemov(&i,&j,ESCALA,kirby4,3,0,DELAY);
            
            //Movimiento de transicion
            onemov(&i,&j,ESCALA,kirby6,3,0,DELAY);

            //Movimiento con brazo izquierdo sobresaliente
            onemov(&i,&j,ESCALA,kirby5,3,0,DELAY);

            //Movimiento hacia arriba (Salto)
            onemov(&i,&j,ESCALA,kirby2,10,10,DELAY);

            //Movimiento hacia abajo (Caida)
            onemov(&i,&j,ESCALA,kirby2,10,-10,DELAY);
        }
    }
    
    ventana.cierraVentana();
    
    return 0;
}
