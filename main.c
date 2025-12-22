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


//RUTAS PARA LOS SPRITES DE KIRBY
#define RUTA_KIRBY "Kirby/Kirby.bmp"
#define RUTA_KIRBYMASK "Kirby/Kirbymask.bmp"
#define RUTA_KIRBY4 "Kirby/Kirby4.bmp"
#define RUTA_KIRBY4MASK "Kirby/Kirby4mask.bmp"
#define RUTA_KIRBY5 "Kirby/Kirby5.bmp"
#define RUTA_KIRBY5MASK "Kirby/Kirby5mask.bmp"
#define RUTA_KIRBY6 "Kirby/Kirby6.bmp"
#define RUTA_KIRBY6MASK "Kirby/Kirby6mask.bmp"

#define ESCALA 35
#define DELAY 50

int main()
{
    Imagen *kirby = ventana.creaImagenConMascara(RUTA_KIRBY,RUTA_KIRBYMASK);
    Imagen *kirby4 = ventana.creaImagenConMascara(RUTA_KIRBY4,RUTA_KIRBY4MASK);
    Imagen *kirby5 = ventana.creaImagenConMascara(RUTA_KIRBY5,RUTA_KIRBY5MASK); 
    Imagen *kirby6 = ventana.creaImagenConMascara(RUTA_KIRBY6,RUTA_KIRBY6MASK);
    
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800,600);

    int coordXkirby0 = 20;
    int coordYkirby0 = ventana.altoVentana()-50;

    int tecla = ventana.teclaPresionada();

    ventana.colorFondo(COLORES.NEGRO);

    ventana.limpiaVentana();

    int i = coordXkirby0;

    while (tecla != TECLAS.ESPACIO)
    {
        tecla = ventana.teclaPresionada();
        int limitex = ventana.anchoVentana() - 50;

        while(i < limitex)
        {
            //Comienza de forma estatica
            if(i == coordXkirby0)
            {
            ventana.muestraImagenEscalada(i,coordYkirby0,ESCALA,ESCALA,kirby);
            ventana.actualizaVentana();
            ventana.espera(50);
            ventana.limpiaVentana();
            i+=3;
            continue;; //Regresa al inicio del bucle
            }
            //Movimiento con brazo derecho visible
            ventana.muestraImagenEscalada(i,coordYkirby0,ESCALA,ESCALA,kirby4);
            ventana.actualizaVentana();
            ventana.espera(50);
            ventana.limpiaVentana();
            i+=3;
            //Movimiento con brazo izquierdo sobresaliente
            ventana.muestraImagenEscalada(i,coordYkirby0,ESCALA,ESCALA,kirby5);
            ventana.actualizaVentana();
            ventana.espera(50);
            ventana.limpiaVentana();
            i+=3;
            //Finaliza de forma estatica
            if (i == limitex - 1)
            {
            ventana.muestraImagenEscalada(i,coordYkirby0,ESCALA,ESCALA,kirby);
            ventana.actualizaVentana();
            ventana.espera(50);
            ventana.limpiaVentana();
            i+=3;
            } 
        }
    }
    
    ventana.cierraVentana();
    
    return 0;
}


void onemov(int *x,int *y,int tamanio,Imagen *imagen,int incremento,int delay)
{
    ventana.muestraImagenEscalada(*x,*y,tamanio,tamanio,imagen);
    ventana.actualizaVentana();
    ventana.espera(delay);
    ventana.limpiaVentana();
    (*x) += incremento;
}
