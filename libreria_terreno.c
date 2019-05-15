#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "libreria_terreno.h"
#include "config.h"


//funcion que recibe un puntero a vector, su capacidad, la capacidad nueva a adquirir y un margen para los puntos y devuelve un doble puntero a float con el vector desificado

float **densificar_vector(float **v, size_t nv, size_t nn, float margen){
	
	float **v2,**vrandom;
	int i,j=0;
	int cantidad_puntos = (nn-nv)/(nv-1);//cantidad de puntos a agregar entre puntos preestablecidos 
	int resto = (nn-nv)%(nv-1);//en el caso de que la cuenta anterior no sea exacta
	int tamano_segmento = cantidad_puntos+1;

	v2=(float **)malloc(sizeof(float*)*nn);//pido memoria para el nuevo vector a punteros, con el tamaño que recibe la funcion

	if(v2==NULL)
		return NULL;//validacion

 	//itero sobre el vector 

	for(i = 0;i < nv-2;i++){
		
		if(pedir_memoria(v2,M,tamano_segmento*i))
			return NULL;

		v2[tamano_segmento*i][0] = v[i][0]; 
		v2[tamano_segmento*i][1] = v[i][1];
		
		//genero un vector de cantidad_puntos x M, donde almaceno los numero random generados entre dos puntos del vector original en los que estoy posicionado y lo guardo en mi vector random auxiliar.
		vrandom=generar_puntos(v,cantidad_puntos,M,i,margen);
		if(vrandom==NULL)
			return NULL;

		for(j = 1;j <= cantidad_puntos;j++){
			
			if(pedir_memoria(v2,M,tamano_segmento*i+j))
				return NULL;
			
			v2[tamano_segmento*i+j][0]=vrandom[j-1][0];//copio los datos del vector random aux
			v2[tamano_segmento*i+j][1]=vrandom[j-1][1];//en mi nuevo vector densificado
		}

		destruir_vector(vrandom,cantidad_puntos,M);//libero la memoria de mi vector random aux
													//para poder reutilizarlo nuevo y no perder memoria. 		
	}

	//hago la ultima iteracion aparte para poder agregarle el resto de puntos que faltan en el ultimo tramo. 

	for(i = nv-2;i < nv;i++){

		if(tamano_segmento*i + resto < nn-1){

			if(pedir_memoria(v2,M,tamano_segmento*i))
				return NULL;
		
			v2[tamano_segmento*i][0] = v[i][0];
			v2[tamano_segmento*i][1] = v[i][1];

			vrandom=generar_puntos(v,(cantidad_puntos+resto),M,i,margen);
			if(vrandom==NULL)
				return NULL;

			for(j = 1;j <= (cantidad_puntos+resto) ;j++){
				
				if(pedir_memoria(v2,M,tamano_segmento*i+j))
					return NULL;
				
				v2[tamano_segmento*i+j][0]=vrandom[j-1][0];
				v2[tamano_segmento*i+j][1]=vrandom[j-1][1];

			}
			destruir_vector(vrandom,(cantidad_puntos+resto),M);
		}

		else if (tamano_segmento*i+resto==nn-1){
		//cuando se encuentra en la ultima posicion del
		//nuevo vector, copia el ultimo punto del vector original. 

			if(pedir_memoria(v2,M,tamano_segmento*i+resto))
				return NULL;

			v2[tamano_segmento*i+resto][0] = v[i][0];
			v2[tamano_segmento*i+resto][1] = v[i][1];

		}
	}

	return v2;
}

//funcion que recibe la cantidad de puntos random a generar, un puntero con los puntos base y la posicion en la que se esta leyendo y la dimension de la matriz.
// e interpola puntos en x y asigna puntos randoms en y para luego guardar en un vector temporal.

float** generar_puntos(float**v,int cantidad_puntos,int m,int posicion,float margen){
	int i;
	float dx;
	float** vector_rand;

	vector_rand = (float**) malloc(sizeof(float*)*cantidad_puntos);
	if(vector_rand==NULL)
		return NULL;

	dx = (v[posicion+1][0]-v[posicion][0])/(float)(cantidad_puntos+1);//calculo la distacia que tendran los puntos generados en x
	

	for (i=0;i<cantidad_puntos;i++){

		if(pedir_memoria(vector_rand,m,i))
				return NULL;

		vector_rand[i][0] = dx*(i+1)+v[posicion][0];
		vector_rand[i][1] = generar_random(v,posicion,margen,vector_rand[i][0]);//lleno mi vector temporal con las x e y generadas.
	}	
	return vector_rand;
}

//funcion que calcula una coordenada y random entre un margen  
float generar_random(float** v,int posicion,float margen, float x_rand){

	
	float a,b,y_temp;

	a=(v[posicion][1]-v[posicion+1][1])/(v[posicion][0]-v[posicion+1][0]); //calculo la pendiente de la recta 
			
	b=v[posicion+1][1]-a*v[posicion+1][0]; //calculo la ordenada al origen
			
	y_temp=a*x_rand+b;//calcula la posicion y en la que se encuentra para luego calcular su margen 

	//funcion que genera un y random entre los extremos de coordenadas ya predeterminadas.
	return 2*margen*((float)rand()/(float)RAND_MAX)+(y_temp-margen);
}  
//funcion que recibe un doble puntero dinamico y libera la memoria pedida
void destruir_vector(float **v,size_t n,size_t m){

	for(int i = 0;i < n;i++){

		free (v[i]);
	}//for

	free (v);
}//destuir_vector

//funcion que recibe una matriz estatica y devuelve un puntero dinamico con la misma informacion
float **matriz_a_vector(const float m[][M], size_t n){
	float **v;
	int i,j;

	v = (float **) malloc(sizeof(float*)*n);

	if(v==NULL){
		return NULL;
	}

	for(i = 0;i < n;i++){

		if(pedir_memoria(v,M,i)){
			return NULL;
		}

		for(j = 0;j < M;j++){
			v[i][j]=m[i][j];
		}//segundo for
		
	}// primer for
	return v;
}//matriz_avector

//funcion que pide y valida memoria devolviendo un bool y en el caso que no se asigne, libera todas las pedidas anteriormente 
bool pedir_memoria(float **v,int m,int posicion){
	
	v[posicion]=malloc(sizeof(float)*m);

	if (v[posicion]==NULL){
		destruir_vector(v,posicion-1,m);
		return true;
	}

	else
		return false;
}


// Devuelve un vector que representa el terreno y su tamano n.
float ** crear_terreno ( size_t * n ) {
	
	* n = 0;
	
	const float terreno_estatico [][2]={
		{0 , 100} ,// izquierda
		{ -1 , VENTANA_ALTO * 2.0 / 3} ,// " medio "
		{ VENTANA_ANCHO , 100}// derecha
	};

	size_t nt = 3;
	float ** terreno = matriz_a_vector ( terreno_estatico , nt ) ;

	if ( terreno == NULL ) 
		return NULL ;

	// Asignamos la coordenada del medio aleatoriamente :
	terreno [1][0] = rand () % VENTANA_ANCHO ;

	// Iterativamente densificamos 30 veces achicando el margen cada vez :
	for ( size_t i = 1; i < 30; i ++) {

		float ** aux = densificar_vector( terreno , nt , 2 * ( i + 5) , 100 / i ) ;
		destruir_vector ( terreno , nt , 2) ;
		if ( aux == NULL ) 
			return NULL ;
		terreno = aux ;
		nt = 2 * ( i + 5) ;
	}
	* n = nt ;
	return terreno ; 
}