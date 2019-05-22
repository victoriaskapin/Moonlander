#include <SDL2/SDL.h>
#include "libreria_terreno.h"
#include "config.h"
#include "naves.h"
#include "caracteres.h"
#include "libreria_nave.h"
#include <time.h>


//funciones que grafican el texto en la pantalla 
void dibujar_letra (const int (*vect_dir)[2], int n_dic, SDL_Renderer *renderer, float desplazamiento, float posicion_y);
void dibujar_string(char *string, caracteres * diccionario, size_t n_string, SDL_Renderer *renderer, float desplazamiento, float posicion_y);
void datos_en_pantalla(SDL_Renderer *renderer, double py, double vx, double vy, float combustible,double tiempo,float score);
void mensajes_finde_partida(SDL_Renderer *renderer,double vx, double vy,double angulo,float *combustible,float * score);

int main() {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Lunar Lander");

	int dormir = 0;

	// BEGIN código del alumno

	srand(time(NULL));

	double px=NAVE_X_INICIAL, py=NAVE_Y_INICIAL;
	double vx=NAVE_VX_INICIAL, vy= NAVE_VY_INICIAL;
	double angulo=NAVE_ANGULO_INICIAL;
	double potencia=NAVE_POTENCIA_INICIAL; 
	float combustible =JUEGO_COMBUSTIBLE_INICIAL;
	float ** terreno;
	double tiempo=0000.0;
	float score=0000.0;
	size_t n_terreno;
	float angulo_aterrizaje;
	
	// Terreno inicial:
	terreno= crear_terreno(&n_terreno); 

	// Mi nave:

	size_t nave_tam = sizeof(nave_grande) / sizeof(nave_grande[0]);
	
	//escribo la nave como vector dinamico para poder modificarla luego.
	float **nave = matriz_a_vector(nave_grande,nave_tam);

	// El chorro de la nave:
	 float const chorro[3][2] = {
		{-NAVE_GRANDE_TOBERA_X, NAVE_GRANDE_TOBERA_Y},
		{0, NAVE_GRANDE_TOBERA_Y},
		{NAVE_GRANDE_TOBERA_X, NAVE_GRANDE_TOBERA_Y}
	};

	float **chorro_rot=matriz_a_vector(chorro,3);

	
	// Queremos que todo se dibuje escalado por f:
	float f = 10;

	// END código del alumno

	unsigned int ticks = SDL_GetTicks();

	while(combustible>0) {
		
		//me fijo si la nave esta arriba del terreno
		if (!vector_esta_arriba(terreno,n_terreno, px,770-py)){//el 770 -py es la diferencia entre el terreno y la nave en el punto de su tobera
			//si la nave aterrizo activo el valor dormir, para luego imprimir los mensajes de fin de partida.
			dormir=3000;
			
		}

		if(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				break;
	    	if (event.type == SDL_KEYDOWN) {
				// BEGIN código del alumno
				switch(event.key.keysym.sym) {

					case SDLK_UP:						
						if(potencia<NAVE_MAX_POTENCIA){

							vy=velocidad_y(angulo,NAVE_VY_INICIAL-potencia);
							vx=velocidad_x(angulo,NAVE_VX_INICIAL+potencia);
							potencia += 1;

							// Agrandamos el dibujo del chorro segun el angulo en el que se encuentra:						
							chorro_rot[1][0] -= cos(angulo);
							chorro_rot[1][1] -= sin(angulo);

						}
						
						break;

					case SDLK_DOWN:
						if(potencia>0){

							vy=velocidad_y(angulo,NAVE_VY_INICIAL-potencia);
							vx=velocidad_x(angulo,NAVE_VX_INICIAL+potencia);
							potencia -= 1;
							// Achicamos el dibujo del chorro:			
							chorro_rot[1][0] += cos(angulo);
							chorro_rot[1][1] += sin (angulo);
						
						}
			
						break;

					case SDLK_RIGHT:
						if(angulo>=0){

							angulo-=NAVE_ROTACION_PASO;
							combustible-=JUEGO_COMBUSTIBLE_RADIANES*NAVE_ROTACION_PASO; 
							vy=velocidad_y(angulo,NAVE_VY_INICIAL-potencia);
							vx=velocidad_x(angulo,NAVE_VX_INICIAL+potencia);

							// Rota el dibujo de la nave:
							vector_rotar ( nave, nave_tam, -NAVE_ROTACION_PASO);
							vector_rotar (chorro_rot, 3, -NAVE_ROTACION_PASO);	
						}	
						
						break;
					
					case SDLK_LEFT:
						if(angulo<=PI){
							angulo+=NAVE_ROTACION_PASO;
							combustible-=JUEGO_COMBUSTIBLE_RADIANES*NAVE_ROTACION_PASO;
							vy=velocidad_y(angulo,NAVE_VY_INICIAL-potencia);
							vx=velocidad_x(angulo,NAVE_VX_INICIAL+potencia);
							// Rota el dibujo de la nave:
							vector_rotar ( nave, nave_tam,NAVE_ROTACION_PASO);
							vector_rotar (chorro_rot,3,NAVE_ROTACION_PASO);	
						}
				
						break;
				}//END SWITCH

				// END código del alumno

			}// END IF 2
			continue;
		}//END IF 1

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    	SDL_RenderClear(renderer);
    	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

		// BEGIN código del alumno 

		//en el caso de aterrizaje o choque se activa la variable dormir y se ingresa al if 
		if (dormir){
			
			angulo_aterrizaje = calcular_pendiente(terreno,n_terreno,px,angulo);//calculo la diferencia entre la pendiente y el angulo de mi nave 

			mensajes_finde_partida(renderer,vx,vy,angulo_aterrizaje,&combustible,&score);//imprimo las leyendas de fin de partida y sumo los puntos acordes
			
			destruir_vector(terreno,n_terreno,M);
			terreno=crear_terreno(&n_terreno);//creo una nueva pantalla
			
			//destruyo los vectores de la nave y el chorro y los vuelvo a inicializar en su posicion original 
			destruir_vector(nave,nave_tam,M);
			nave = matriz_a_vector(nave_grande,nave_tam);
			
			destruir_vector(chorro_rot,3,M);
			chorro_rot = matriz_a_vector(chorro,3);

			reiniciar_valores(& px,& py,& vx,& vy, & potencia,& angulo);

		}

		tiempo+=1.0/JUEGO_FPS;

		combustible-=JUEGO_COMBUSTIBLE_POT_X_SEG*(potencia/tiempo);

		vy=computar_velocidad(vy, G,1.0/JUEGO_FPS);//la gravedad es positiva ya que el eje esta al reves 

		px=computar_posicion(px,vx,1.0/JUEGO_FPS);
		py=computar_posicion (py, vy,1.0/JUEGO_FPS);	

		//la pantalla es cilindrica. 
		if(px>VENTANA_ANCHO)
			px=0;

		if(px<0)
			px=VENTANA_ANCHO;


		// Gráficos: 

		datos_en_pantalla(renderer,py,vx,vy,combustible,tiempo,score);//dibujo los stats arriba en la pantalla 
		
		// Dibujamos la nave escalada por f y la traslado en las posiciones en x e y calculadas previamente
		f= 2;
		for(int i = 0; i < nave_tam - 1; i++){

			SDL_RenderDrawLine(
				renderer,
				nave[i][0]* f  + px,
				-nave[i][1]* f + py,
				nave[i+1][0]* f  + px,
				-nave[i+1][1]* f  + py
			);
		}

		// Dibujamos el chorro escalado por f 
		for(int i = 0; i < 3 - 1; i++){
			SDL_RenderDrawLine(
				renderer,
				chorro_rot[i][0]* f  + px,
				-chorro_rot[i][1]* f  + py,
				chorro_rot[i+1][0]* f  + px,
				-chorro_rot[i+1][1]* f  + py
			);
		}

		//dibujo el terreno: 
		for(int i=0;i<n_terreno-1;i++){
			SDL_RenderDrawLine(
				renderer,
				terreno[i][0],
				VENTANA_ALTO-terreno[i][1],
				terreno[i+1][0],
				VENTANA_ALTO-terreno[i+1][1] );
		}
		
		// END código del alumno

        SDL_RenderPresent(renderer);
		ticks = SDL_GetTicks() - ticks;
		if(dormir) {
			SDL_Delay(dormir);
			dormir = 0;
		}
		else if(ticks < 1000 / JUEGO_FPS)
			SDL_Delay(1000 / JUEGO_FPS - ticks);
		ticks = SDL_GetTicks();
	}//END WHILE

	// BEGIN código del alumno

	//print puntaje final 
	printf("FINAL SCORE: %i\n",(int)score);

	//libero la memoria que utilice para la nave y el chorro 
	destruir_vector(nave,nave_tam,M);
	destruir_vector(chorro_rot,3,M);
	
	// END código del alumno

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

//funcion que gratica el vector a 2 ints que recibe, en una posicion que recibe como "desplazamiento" y "posicion_y"
void dibujar_letra (const int (*vect_dir)[2],int n_dic,SDL_Renderer *renderer,float desplazamiento,float posicion_y){
	int i;

	for( i = 0; i < n_dic-1 ; i++){

		SDL_RenderDrawLine(
			renderer,
			vect_dir[i][0]*CARACTER_ANCHO + desplazamiento,
			-vect_dir[i][1]*CARACTER_ALTO + posicion_y,
			vect_dir[i+1][0]*CARACTER_ANCHO + desplazamiento,
			-vect_dir[i+1][1]*CARACTER_ALTO + posicion_y);
	}
}

//funcion que dibuja la string que se le cargue en una posicion x= desplazamiento y una posicion en y especificadas 
void dibujar_string(char *string,caracteres * diccionario,size_t n_string,SDL_Renderer *renderer,float desplazamiento,float posicion_y){
	int j;
	int i;

	for( i=0;i<n_string;i++){

		for( j=0;string[i]!=diccionario[j].caracter;j++){

		}

		dibujar_letra(diccionario[j].vect_dir,diccionario[j].longitud,renderer,desplazamiento,posicion_y);

		desplazamiento+=CARACTER_ANCHO*5.5;

	}

}

//funcion que imprime los datos que recibe por pantalla junto con sus respectivas etiquetas
void datos_en_pantalla(SDL_Renderer *renderer,double py,double vx,double vy,float combustible,double tiempo,float score){

	caracteres * diccionario;
	size_t n_string;
	char *string ;
	float desplazamiento;
	float posicion_y;

	double altitude=VENTANA_ALTO-py;

	string= (char *)malloc(sizeof(char)*MAX_STRING);
	
	diccionario=crear_diccionario();

	posicion_y=30;//renglon 1

	strcpy(string,"score");
	desplazamiento=150;
	n_string = strlen(string);	
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	//score
	sprintf(string,"%04i",(int)score);
	n_string = strlen(string);
	desplazamiento=230;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
	

	strcpy(string,"altitude");
	n_string = strlen(string);
	desplazamiento=500;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	//altitud
	sprintf(string,"%04i",(int)altitude);
	n_string = strlen(string);
	desplazamiento=600;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		
	
	posicion_y+=30;//renglon 2

	strcpy(string,"tiempo");
	n_string = strlen(string);
	desplazamiento=150;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	//tiempo
	sprintf(string,"%04i",(int)tiempo);
	n_string = strlen(string);
	desplazamiento=220;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);	
	
	strcpy(string,"horizontal speed");
	n_string = strlen(string);
	desplazamiento=500;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	
	if(vx<0){
		strcpy(string,"<");
		n_string = strlen(string);
		desplazamiento=790;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		
		//horizontal speed
		sprintf(string,"%04i",(int)-vx);
		n_string = strlen(string);
		desplazamiento=700;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		}

	else{
		strcpy(string,">");
		n_string = strlen(string);
		desplazamiento=790;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		
		sprintf(string,"%04i",(int)vx);
		n_string = strlen(string);
		desplazamiento=700;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	}

	posicion_y+=30;//renglon 3

	strcpy(string,"fuel");
	n_string = strlen(string);
	desplazamiento=150;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	//combustible
	sprintf(string,"%04i",(int)combustible);
	n_string = strlen(string);
	desplazamiento=200;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	strcpy(string,"vertical speed");
	n_string = strlen(string);
	desplazamiento=500;
	dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);


	if(vy<0){
		strcpy(string,"^");
		n_string = strlen(string);
		desplazamiento=790;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

		sprintf(string,"%04i",(int)-vy);
		n_string = strlen(string);
		desplazamiento=700;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
	}

	else{
		strcpy(string,"V");
		n_string = strlen(string);
		desplazamiento=790;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		
		sprintf(string,"%04i",(int)vy);
		n_string = strlen(string);
		desplazamiento=700;
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

	}
	free(diccionario); 
	free(string);

}

//funcion que recibe las posiciones en x e y, el angulo, la direccion de memoria del combustible y el score.
// acorde al angulo y velocidades imprime por pantalla una leyenda y/o modifica los valores de combustible y score
void mensajes_finde_partida(SDL_Renderer *renderer,double vx, double vy,double angulo,float *combustible,float * score){

	caracteres * diccionario;
	size_t n_string;
	char *string ;
	float desplazamiento;
	float posicion_y=600;

	string= (char *)malloc(sizeof(char)*MAX_STRING);
	
	diccionario=crear_diccionario();

	posicion_y=250;
	desplazamiento =500;

	if(vx<=1.0 && vy<=10.0 && angulo<=0.01){

		strcpy(string,"you landed well");
		n_string = strlen(string);
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		*score+=50;
	}

	else if (vx<=2.0 && vy<=20.0 && angulo<=0.01){

		strcpy(string,"you landed hard");
		n_string = strlen(string);
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);

		*score+=15;
	}

	else {

		strcpy(string,"destroyed");
		n_string = strlen(string);
		dibujar_string(string,diccionario,n_string,renderer,desplazamiento,posicion_y);
		
		*combustible-=250;
		*score+=5;
	}

	free(diccionario); 
	free(string);
}