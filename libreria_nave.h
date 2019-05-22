#ifndef __LIBRERIA_NAVE_H__
#define __LIBRERIA_NAVE_H__

#include "config.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

bool vector_esta_arriba ( float ** coordenadas , size_t n , float x , float y ) ;
void vector_rotar (  float ** coordenadas , size_t n , double rad ) ;
double computar_velocidad(double vy, double a, double dt);
double computar_posicion (double pi, double vi, double dt);
void reiniciar_valores(double *posicion_x, double *posicion_y, double * vx, double *vy, double* potencia, double* angulo);
double velocidad_y(double ang,double vi);
double velocidad_x(double ang,double vi);
float calcular_pendiente(float **coordenadas,size_t n,float px,double angulo);

#endif