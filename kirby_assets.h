#ifndef __KIRBY_ASSETS__
#define __KIRBY_ASSETS__

#include "graficos.h"
#include "control_esp.h"

/// @brief Carga las animaciones de kirby, saltar, comer, inhalar, etc.
/// @param k Direccion de la variable kirby donde se guardaran las direcciones de memoria
void cargar_animaciones_kirby(Kirby *k);

/// @brief Cargar las animaciones de kirby inversas
/// @param k Direccion de la variable kirby donde se guardaran las direcciones de memoria
void cargar_animacionesmirror_kirby(Kirby *k);

void cargar_animaciones_enemies(Enemigo *enemies);

void cargar_all_fondos(Fondos *fondos);

#endif