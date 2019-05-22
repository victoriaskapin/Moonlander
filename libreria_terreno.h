#ifndef __LIBRERIA_TERRENO_H__
#define __LIBRERIA_TERRENO_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

#define M 2

float **densificar_vector(float **v, size_t nv, size_t nv2, float margen); 
void destruir_vector(float **v,size_t n,size_t m);
float **matriz_a_vector(const float m[][M], size_t n);
float **pedir_memoria_matriz(size_t n,int m);
float** generar_puntos(float**v,int cantidad_puntos,int m,int posicion,float margen);
void ordenar_vector ( float **vector , int cantidad_puntos );
float generar_random_x(float **v,size_t pts_vector_original);
float generar_random_y(float** v,int posicion,float margen, float x_rand);
float ** crear_terreno ( size_t * n );


#endif