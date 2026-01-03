#include "kirby_assets.h"
#include "control_esp.h"

void cargar_animaciones_kirby(Kirby *k)
{
    static const char *kirby_walk_r[] = {
        "Kirby/Kirby4.bmp",
        "Kirby/Kirby24.bmp",
        "Kirby/Kirby5.bmp",
        "Kirby/Kirby6.bmp"
    };

    static const char *kirby_walk_r_mask[] = {
        "Kirby/Kirby4mask.bmp",
        "Kirby/Kirby24mask.bmp",
        "Kirby/Kirby5mask.bmp",
        "Kirby/Kirby6mask.bmp"
    };

    static const char *kirby_eat_r[] = {
        "Kirby/Kirby23.bmp",
        "Kirby/Kirby22.bmp",
        "Kirby/Kirby19.bmp"
    };

    static const char *kirby_eat_r_mask[] = {
        "Kirby/Kirby23mask.bmp",
        "Kirby/Kirby22mask.bmp",
        "Kirby/Kirby19mask.bmp" 
    };

    static const char *kirby_inhale_r[] = {
        "Kirby/Kirby7.bmp",
        "Kirby/Kirby8.bmp"
    };

    static const char *kirby_inhale_r_mask[] = {
        "Kirby/Kirby7mask.bmp",
        "Kirby/Kirby8mask.bmp"
    };

    static const char* kirbyfat_walk_r[] = {
        "Kirby/Kirby17.bmp",
        "Kirby/Kirby18.bmp"
    };

    static const char* kirbyfat_walk_r_mask[] = {
        "Kirby/Kirby17mask.bmp",
        "Kirby/Kirby18mask.bmp"
    };

    static const char* kirbyfatair_walk_r[] = {
        "Kirby/Kirby14.bmp",
        "Kirby/Kirby15.bmp"
    };

    static const char* kirbyfatair_walk_r_mask[] = {
        "Kirby/Kirby14mask.bmp",
        "Kirby/Kirby15mask.bmp"
    };

    static const char* kirby_spit_r[] = {
        "Kirby/Kirby13.bmp",
        "Kirby/Kirby8.bmp",
        "Kirby/Kirby7.bmp"
    };

    static const char* kirby_spit_r_mask[] = {
        "Kirby/Kirby13mask.bmp",
        "Kirby/Kirby8mask.bmp",
        "Kirby/Kirby7mask.bmp"
    };

    static const char* kirby_idle_r[] = {
        "Kirby/kirby.bmp",
        "Kirby/kirby.bmp"
    };

    static const char* kirby_idle_r_mask[] = {
        "Kirby/Kirbymask.bmp",
        "Kirby/Kirbymask.bmp"
    };

    static const char* kirby_jump_r[] = {
        "Kirby/Kirby2.bmp",
        "Kirby/Kirby2.bmp"
    };

    static const char* kirby_jump_r_mask[] = {
        "Kirby/Kirby2mask.bmp",
        "Kirby/Kirby2mask.bmp"
    };

    static const char* kirbyfat_jump_r[] = {
        "Kirby/Kirby19.bmp",
        "Kirby/Kirby19.bmp"
    };

    static const char* kirbyfat_jump_r_mask[] = {
        "Kirby/Kirby19mask.bmp",
        "Kirby/Kirby19mask.bmp"
    };

    static const char* kirbyfat_idle_r[] = {
        "Kirby/Kirby16.bmp",
        "Kirby/Kirby16.bmp"
    };

    static const char* kirbyfat_idle_r_mask[] = {
        "Kirby/Kirby16mask.bmp",
        "Kirby/Kirby16mask.bmp"
    };

    static const char* humo_r[] = {
        "Kirby/Nube.bmp",
        "Kirby/Nube.bmp"
    };

    static const char* humo_r_mask[] = {
        "Kirby/Nubemask.bmp",
        "Kirby/Nubemask.bmp"
    };

    static const char* estrella[] = {
        "Kirby/Estrellita.bmp",
        "Kirby/Estrella2.bmp",
        "Kirby/Estrella.bmp"
    };

    static const char* estrella_mask[] = {
        "Kirby/Estrellitamask.bmp",
        "Kirby/Estrella2mask.bmp",
        "Kirby/Estrellamask.bmp"
    };

    cargar_animacion(kirbyfat_jump_r,kirbyfat_jump_r_mask,2,2,&k->arregloAnim[AN_FAT_FALL]);
    k->arregloAnim[AN_FAT_FALL].bucle = true; //True se repite la animacion, false no

    cargar_animacion(kirby_idle_r,kirby_idle_r_mask,2,2,&k->arregloAnim[AN_IDLE]);
    k->arregloAnim[AN_IDLE].bucle = true;

    cargar_animacion(kirby_jump_r,kirby_jump_r_mask,2,2,&k->arregloAnim[AN_JUMP]);
    k->arregloAnim[AN_JUMP].bucle = true;

    cargar_animacion(kirby_walk_r,kirby_walk_r_mask,4,3,&k->arregloAnim[AN_WALK]);
    k->arregloAnim[AN_WALK].bucle = true;

    cargar_animacion(kirby_eat_r,kirby_eat_r_mask,3,5,&k->arregloAnim[AN_EAT]);
    k->arregloAnim[AN_EAT].bucle = false;

    cargar_animacion(kirbyfat_idle_r,kirby_idle_r_mask,2,2,&k->arregloAnim[AN_FAT_IDLE]);
    k->arregloAnim[AN_FAT_IDLE].bucle = true;

    cargar_animacion(kirbyfat_walk_r,kirbyfat_walk_r_mask,2,2,&k->arregloAnim[AN_FAT_WALK]);
    k->arregloAnim[AN_FAT_WALK].bucle = true;

    cargar_animacion(kirbyfatair_walk_r,kirbyfatair_walk_r_mask,2,7,&k->arregloAnim[AN_FLY]);
    k->arregloAnim[AN_FLY].bucle = false;

    cargar_animacion(kirby_spit_r,kirby_spit_r_mask,3,8,&k->arregloAnim[AN_SPIT]);
    k->arregloAnim[AN_SPIT].bucle = false;

    cargar_animacion(kirby_inhale_r,kirby_inhale_r_mask,2,2,&k->arregloAnim[AN_INHALE]);
    k->arregloAnim[AN_INHALE].bucle = true;

    cargar_animacion(humo_r,humo_r_mask,2,2,&k->proyectil.arregloProyectil[0]);
    k->proyectil.arregloProyectil[0].bucle = true;

    cargar_animacion(estrella,estrella_mask,3,3,&k->proyectil.arregloProyectil[1]);
    k->proyectil.arregloProyectil[1].bucle = true;

}

void cargar_animacionesmirror_kirby(Kirby *k)
{
    static const char *kirby_walk_r_mirror[] = {
        "Kirbymirror/Kirby4.bmp",
        "Kirbymirror/Kirby24.bmp",
        "Kirbymirror/Kirby5.bmp",
        "Kirbymirror/Kirby6.bmp"
    };

    static const char *kirby_walk_r_mask_mirror[] = {
        "Kirbymirror/Kirby4mask.bmp",
        "Kirbymirror/Kirby24mask.bmp",
        "Kirbymirror/Kirby5mask.bmp",
        "Kirbymirror/Kirby6mask.bmp"
    };

    static const char *kirby_eat_r_mirror[] = {
        "Kirbymirror/Kirby23.bmp",
        "Kirbymirror/Kirby22.bmp",
        "Kirbymirror/Kirby19.bmp"
    };

    static const char *kirby_eat_r_mask_mirror[] = {
        "Kirbymirror/Kirby23mask.bmp",
        "Kirbymirror/Kirby22mask.bmp",
        "Kirbymirror/Kirby19mask.bmp" 
    };

    static const char *kirby_inhale_r_mirror[] = {
        "Kirbymirror/Kirby7.bmp",
        "Kirbymirror/Kirby8.bmp",
    };

    static const char *kirby_inhale_r_mask_mirror[] = {
        "Kirbymirror/Kirby7mask.bmp",
        "Kirbymirror/Kirby8mask.bmp",
    };

    static const char* kirbyfat_walk_r_mirror[] = {
        "Kirbymirror/Kirby17.bmp",
        "Kirbymirror/Kirby18.bmp"
    };

    static const char* kirbyfat_walk_r_mask_mirror[] = {
        "Kirbymirror/Kirby17mask.bmp",
        "Kirbymirror/Kirby18mask.bmp"
    };

    static const char* kirbyfatair_walk_r_mirror[] = {
        "Kirbymirror/Kirby14.bmp",
        "Kirbymirror/Kirby15.bmp"
    };

    static const char* kirbyfatair_walk_r_mask_mirror[] = {
        "Kirbymirror/Kirby14mask.bmp",
        "Kirbymirror/Kirby15mask.bmp"
    };

    static const char* kirby_spit_r_mirror[] = {
        "Kirbymirror/Kirby13.bmp",
        "Kirbymirror/Kirby8.bmp",
        "Kirbymirror/Kirby7.bmp"
    };

    static const char* kirby_spit_r_mask_mirror[] = {
        "Kirbymirror/Kirby13mask.bmp",
        "Kirbymirror/Kirby8mask.bmp",
        "Kirbymirror/Kirby7mask.bmp"
    };

    static const char* kirby_idle_r_mirror[] = {
        "Kirbymirror/Kirby.bmp",
        "Kirbymirror/Kirby.bmp"
    };

    static const char* kirby_idle_r_mask_mirror[] = {
        "Kirbymirror/Kirbymask.bmp",
        "Kirbymirror/Kirbymask.bmp"
    };

    static const char* kirby_jump_r_mirror[] = {
        "Kirbymirror/Kirby2.bmp",
        "Kirbymirror/Kirby2.bmp"
    };

    static const char* kirby_jump_r_mask_mirror[] = {
        "Kirbymirror/Kirby2mask.bmp",
        "Kirbymirror/Kirby2mask.bmp"
    };

    static const char* kirbyfat_jump_r_mirror[] = {
        "Kirbymirror/Kirby19.bmp",
        "Kirbymirror/Kirby19.bmp"
    };

    static const char* kirbyfat_jump_r_mask_mirror[] = {
        "Kirbymirror/Kirby19mask.bmp",
        "Kirbymirror/Kirby19mask.bmp"
    };

    static const char* kirbyfat_idle_r_mirror[] = {
        "Kirbymirror/Kirby16.bmp",
        "Kirbymirror/Kirby16.bmp"
    };

    static const char* kirbyfat_idle_r_mask_mirror[] = {
        "Kirbymirror/Kirby16mask.bmp",
        "Kirbymirror/Kirby16mask.bmp"
    };

    static const char* humo_r_mirror[] = {
        "Kirbymirror/Nube.bmp",
        "Kirbymirror/Nube.bmp"
    };

    static const char* humo_r_mask_mirror[] = {
        "Kirbymirror/Nubemask.bmp",
        "Kirbymirror/Nubemask.bmp"
    };

    static const char* estrella_mirror[] = {
        "Kirbymirror/Estrellita.bmp",
        "Kirbymirror/Estrella2.bmp",
        "Kirbymirror/Estrella.bmp"
    };

    static const char* estrella_mask_mirror[] = {
        "Kirbymirror/Estrellitamask.bmp",
        "Kirbymirror/Estrella2mask.bmp",
        "Kirbymirror/Estrellamask.bmp"
    };

    cargar_animacion(kirbyfat_jump_r_mirror,kirbyfat_jump_r_mask_mirror,2,2,&k->arregloAnimMirror[AN_FAT_FALL]);
    k->arregloAnimMirror[AN_FAT_FALL].bucle = true;

    cargar_animacion(kirby_idle_r_mirror,kirby_idle_r_mask_mirror,2,2,&k->arregloAnimMirror[AN_IDLE]);
    k->arregloAnimMirror[AN_IDLE].bucle = true;

    cargar_animacion(kirby_jump_r_mirror,kirby_jump_r_mask_mirror,2,2,&k->arregloAnimMirror[AN_JUMP]);
    k->arregloAnimMirror[AN_JUMP].bucle = true;

    cargar_animacion(kirby_walk_r_mirror,kirby_walk_r_mask_mirror,4,3,&k->arregloAnimMirror[AN_WALK]);
    k->arregloAnimMirror[AN_WALK].bucle = true;

    cargar_animacion(kirby_eat_r_mirror,kirby_eat_r_mask_mirror,3,5,&k->arregloAnimMirror[AN_EAT]);
    k->arregloAnimMirror[AN_EAT].bucle = false;

    cargar_animacion(kirbyfat_idle_r_mirror,kirby_idle_r_mask_mirror,2,2,&k->arregloAnimMirror[AN_FAT_IDLE]);
    k->arregloAnimMirror[AN_FAT_IDLE].bucle = true;

    cargar_animacion(kirbyfat_walk_r_mirror,kirbyfat_walk_r_mask_mirror,2,2,&k->arregloAnimMirror[AN_FAT_WALK]);
    k->arregloAnimMirror[AN_FAT_WALK].bucle = true;

    cargar_animacion(kirbyfatair_walk_r_mirror,kirbyfatair_walk_r_mask_mirror,2,7,&k->arregloAnimMirror[AN_FLY]);
    k->arregloAnimMirror[AN_FLY].bucle = false;

    cargar_animacion(kirby_spit_r_mirror,kirby_spit_r_mask_mirror,3,8,&k->arregloAnimMirror[AN_SPIT]);
    k->arregloAnimMirror[AN_SPIT].bucle = false;

    cargar_animacion(kirby_inhale_r_mirror,kirby_inhale_r_mask_mirror,2,2,&k->arregloAnimMirror[AN_INHALE]);
    k->arregloAnimMirror[AN_INHALE].bucle = true;
    
    cargar_animacion(humo_r_mirror,humo_r_mask_mirror,2,2,&k->proyectil.arregloProyectilMirror[0]);
    k->proyectil.arregloProyectilMirror[0].bucle = true;

    cargar_animacion(estrella_mirror,estrella_mask_mirror,3,3,&k->proyectil.arregloProyectilMirror[1]);
    k->proyectil.arregloProyectilMirror[1].bucle = true;
}

void cargar_animaciones_enemies(Enemigo *enemies)
{
    static const char *bronto_walk[] = {
        "Enemigos/Bronto.bmp",
        "Enemigos/Bronto2.bmp"
    }; 

    static const char *bronto_walk_mask[] = {
        "Enemigos/Brontomask.bmp",
        "Enemigos/Bronto2mask.bmp"        
    };

    static const char *cappy_walk[] = {
        "Enemigos/Cappy.bmp",
        "Enemigos/Cappy2.bmp",
        "Enemigos/Cappy3.bmp"
    };

    static const char *cappy_walk_mask[] = {
        "Enemigos/Cappymask.bmp",
        "Enemigos/Cappy2mask.bmp",
        "Enemigos/Cappy3mask.bmp"
    };

    static const char *grizzo[] = {
        "Enemigos/Grizzo.bmp",
        "Enemigos/Grizzo2.bmp"
    };

    static const char *grizzo_mask[] = {
        "Enemigos/Grizzomask.bmp",
        "Enemigos/Grizzo2mask.bmp"
    };

    static const char* manzana[] = {
        "Enemigos/Manzana.bmp",
        "Enemigos/Manzana2.bmp",
        "Enemigos/Manzana3.bmp",
        "Enemigos/Manzana4.bmp"
    };

    static const char* manzana_mask[] = {
        "Enemigos/Manzanamask.bmp",
        "Enemigos/Manzana2mask.bmp",
        "Enemigos/Manzana3mask.bmp",
        "Enemigos/Manzana4mask.bmp"
    };

    static const char* poppybros[] = {
        "Enemigos/Poppybros.bmp",
        "Enemigos/Poppybros2.bmp"
    };

    static const char* poppybros_mask[] = {
        "Enemigos/Poppybrosmask.bmp",
        "Enemigos/Poppybros2mask.bmp"
    };

    static const char* twizzy[] = {
        "Enemigos/Twizzy.bmp",
        "Enemigos/Twizzy2.bmp"
    };

    static const char* twizzy_mask[] = {
        "Enemigos/Twizzymask.bmp",
        "Enemigos/Twizzy2mask.bmp"
    };

    static const char* wadledee[] = {
        "Enemigos/WadleDee.bmp",
        "Enemigos/WadleDee2.bmp"
    };

    static const char* wadledee_mask[] = {
        "Enemigos/WadleDeemask.bmp",
        "Enemigos/WadleDee2mask.bmp"
    };

    cargar_animacion(bronto_walk,bronto_walk_mask,2,3,&enemies->arregloEnemies[EN_BRONTO]);
    enemies->arregloEnemies[EN_BRONTO].bucle = true;

    cargar_animacion(cappy_walk,cappy_walk_mask,3,4,&enemies->arregloEnemies[EN_CAPPY]);
    enemies->arregloEnemies[EN_CAPPY].bucle = true;

    cargar_animacion(grizzo,grizzo_mask,2,3,&enemies->arregloEnemies[EN_GRIZZO]);
    enemies->arregloEnemies[EN_GRIZZO].bucle = true;

    cargar_animacion(manzana,manzana_mask,4,6,&enemies->arregloEnemies[EN_MANZANA]);
    enemies->arregloEnemies[EN_MANZANA].bucle = true;

    cargar_animacion(poppybros,poppybros_mask,2,3,&enemies->arregloEnemies[EN_POPPY]);
    enemies->arregloEnemies[EN_POPPY].bucle = true;

    cargar_animacion(twizzy,twizzy_mask,2,3,&enemies->arregloEnemies[EN_TWIZZY]);
    enemies->arregloEnemies[EN_TWIZZY].bucle = true;

    cargar_animacion(wadledee,wadledee_mask,2,3,&enemies->arregloEnemies[EN_WADLE]);
    enemies->arregloEnemies[EN_WADLE].bucle = true;

}