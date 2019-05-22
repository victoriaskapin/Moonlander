#include "libreria_nave.h"

//funcion que recibe un doble puntero a floats, su tamaño, una posicion en x e y, y devuelve un bool si esa coordenada se encuentra arriba del vector
bool vector_esta_arriba(float **coordenadas,size_t n,float x, float y){ 
	int i=0;//,ant=0,sig=1;
	float a,b,y_temp;

	for(i=0;i<n-1;i++){
	 	if(coordenadas[i+1][0]>=x){

	 		a=(coordenadas[i][1]-coordenadas[i+1][1])/(coordenadas[i][0]-coordenadas[i+1][0]); //calculo la pendiente de la recta 
			
			b=coordenadas[i+1][1]-a*coordenadas[i+1][0]; //calculo la ordenada al origen
			
			y_temp=a*x+b; 
			
			return (y>y_temp);
	 	}
	}
	return false;
}

//funcion que recibe un doble puntero a floats, su tamaño y un double de cantidad de radianes a rotar y rota el vector
void vector_rotar(float ** coordenadas, size_t n, double rad){
	int i;
	float coordenadas_rotadax,coordenadas_rotaday,coseno_angulo,seno_angulo;
	coseno_angulo=cos(rad);
	seno_angulo=sin(rad);
	for(i=0;i<n;i++){
		
		coordenadas_rotadax = coordenadas[i][0]*coseno_angulo-coordenadas[i][1]*seno_angulo;
		coordenadas_rotaday = coordenadas[i][0]*seno_angulo+coordenadas[i][1]*coseno_angulo;
		coordenadas[i][0] = coordenadas_rotadax;
		coordenadas[i][1] = coordenadas_rotaday;
	}
}

double computar_velocidad(double vy, double a, double dt){
	return vy+(a*dt);
}

double computar_posicion (double pi, double vi, double dt){
	return pi+(vi*dt);
}

double velocidad_y(double ang,double vi){
	return sin(ang)*vi;
}

double velocidad_x(double ang,double vi){
	return cos(ang)*vi;
}

//funcion que reestablece a los valores originales 
void reiniciar_valores(double *posicion_x, double *posicion_y, double * vx, double *vy, double* potencia, double* angulo){

	*posicion_x = NAVE_X_INICIAL;
	*posicion_y = NAVE_Y_INICIAL;
	*potencia = NAVE_POTENCIA_INICIAL;
	*angulo = NAVE_ANGULO_INICIAL;
	*vx=velocidad_x(NAVE_ANGULO_INICIAL,NAVE_VX_INICIAL);
	*vy=velocidad_y(NAVE_ANGULO_INICIAL,NAVE_VY_INICIAL); 

}

//funcion que me devuelve un float que calcula la diferencia entre la pendiente del terreno y el angulo de la nave
float calcular_pendiente(float **coordenadas,size_t n,float px,double angulo){ 
	int i=0;
	float a;

	for(i=0;i<n-1;i++){
	 	if(coordenadas[i+1][0]>=px){

	 		a=(coordenadas[i][1]-coordenadas[i+1][1])/(coordenadas[i][0]-coordenadas[i+1][0]); //calculo la pendiente de la recta 
			
			return a-tan(angulo);
		}
	}	
	return 1;
}


