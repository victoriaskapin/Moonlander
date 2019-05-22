naves.o : naves.c
	gcc -std=c99 -Wall -pedantic -c naves.c -o naves.o

caracteres.o : caracteres.c
	gcc -std=c99 -Wall -pedantic -c caracteres.c -o caracteres.o

libreria_terreno.o : libreria_terreno.c
	gcc -std=c99 -Wall -pedantic -c libreria_terreno.c -o libreria_terreno.o -lm

libreria_nave.o : libreria_nave.c
	gcc -std=c99 -Wall -pedantic  -c libreria_nave.c -o libreria_nave.o -lm

main.o : main.c
	gcc -std=c99 -Wall -pedantic  -c main.c -o main.o -lm -lSDL2

all: main.o naves.o caracteres.o libreria_terreno.o libreria_nave.o
	gcc -std=c99 -Wall -pedantic -g main.o naves.o caracteres.o libreria_terreno.o libreria_nave.o -o moonlander -lm -lSDL2

clean: 
	rm *.o

run: all
	./moonlander

