#ifndef __LIBRERIA_TERRENO_H__
#define __LIBRERIA_TERRENO_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "config.h"

#define M 2


float **densificar_vector(float **v, size_t nv, size_t nn, float margen); 
void destruir_vector(float **v,size_t n,size_t m);
float **matriz_a_vector(const float m[][M], size_t n);
bool pedir_memoria(float **v,int m,int posicion);
float generar_random(float** v,int posicion,float margen, float x_rand);
float** generar_puntos(float**v,int cantidad_puntos,int m,int posicion,float margen);
float ** crear_terreno ( size_t * n );

#endif