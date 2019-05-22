#include "libreria_terreno.h"

//funcion que recibe un puntero a vector, su capacidad, la capacidad nueva a adquirir y un margen para los puntos y devuelve un doble puntero a float con el vector desificado.

float **densificar_vector(float **v, size_t nv, size_t nv2, float margen){
	int i,j=1;
	float **v2;

	v2=pedir_memoria_matriz(nv2,M);//pido memoria para el nuevo vector a punteros, con el tamaño que recibe la funcion

	//validacion
	if(v2==NULL)
		return NULL;

	for (i=0;i<nv;i++){
		v2[i][0]=v[i][0];
		v2[i][1]=v[i][1];
	}

	for (i=nv;i<nv2;i++){//lleno el vector temporal de puntos randoms en x

		v2[i][0] = generar_random_x(v,nv);
	}

	ordenar_vector (v2, nv2);

		for (i=1;i<nv2;i++){//lleno los puntos randoms en y
		
			for (j=j ; j<nv;j++){//cuando encuentra el punto x que establecio el vector original, 
									//no le genero un x nuevo y le pido que 
										//avance en el vector
				
				if(v2[i][0]==v[j][0]){
					i++;
				}

				else{
					v2[i][1] = generar_random_y(v,j-1,margen,v2[i][0]);//lleno mi vector temporal con las x e y generadas.
					break;
				}	
			}			
		}

	return v2;
}

//funcion que recibe un doble puntero a floats y ordena sus coordenadas x (por metodo de seleccion) de menor a mayor
void ordenar_vector ( float **vector , int cantidad_puntos ) {
	int i , j ;
	float *tmp ;
	int minimo ;

	for ( i =0 ; i < ( cantidad_puntos -1) ; i ++) {
		minimo = i ; /* minimo actual */
		for ( j = i +1 ; j < cantidad_puntos ; j ++) {
			/* buscar el menor elemento del vector */
			if ( vector [ minimo ][0] > vector [ j ][0])
				minimo = j ; /* nuevo minimo */
		}
	/* intercambia el vector [ i ] por el vector [ minimo ] */
		tmp = vector [ i ];
		vector [ i ] = vector [ minimo ];
		vector [ minimo ] = tmp ;
	}
}

//funcion que recibe un doble puntero a floats y la posicion donde se encuentra y devuelve un float random entre su posicion y la siguiente
float generar_random_x(float **v,size_t pts_vector_original){
	return (v[pts_vector_original-1][0]-v[0][0])*((float)rand()/(float)RAND_MAX)+(v[0][0]);
}
//funcion que calcula una coordenada 'y' random entre un margen 
float generar_random_y(float** v,int posicion,float margen, float x_rand){

	float a,b,y_temp;

	a=(v[posicion][1]-v[posicion+1][1])/(v[posicion][0]-v[posicion+1][0]); //calculo la pendiente de la recta 
			
	b=v[posicion+1][1]-a*v[posicion+1][0]; //calculo la ordenada al origen
			
	y_temp=a*x_rand+b;//calcula la posicion y en la que se encuentra para luego calcular su margen 

	return 2*margen*((float)rand()/(float)RAND_MAX)+(y_temp-margen);
} 

//funcion que recibe un doble puntero dinamico y libera la memoria pedida
void destruir_vector(float **v,size_t n,size_t m){

	for(int i = 0;i < n;i++){

		free (v[i]);
	}

	free (v);
}

//funcion que recibe una matriz estatica y devuelve un puntero dinamico con la misma informacion
float **matriz_a_vector(const float m[][M], size_t n){
	float **v;
	int i,j;

	v = pedir_memoria_matriz(n,M);

	if(v==NULL){
		return NULL;
	}

	for(i = 0;i < n;i++){

		for(j = 0;j < M;j++){
			v[i][j]=m[i][j];
		}
		
	}
	return v;
}

//funcion que pide y valida memoria y en el caso que no se asigne, libera todas las pedidas anteriormente 
float **pedir_memoria_matriz(size_t n,int m){
	int i;
	float **v;
	
	v = (float **) malloc(sizeof(float*)*n);

	if(v==NULL)
		return NULL;
	
	for(i = 0; i < n ; i++){
		
		v[i]=malloc(sizeof(float)*m);

		if (v[i]==NULL){

			destruir_vector(v,i-1,m);

			return NULL;
		}
	}

	return v;	
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

