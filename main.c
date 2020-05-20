//Victor Hugo Oyervides Covarrubias - A01382836.
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//VARIABLES CONSTANTES
#define GOTAS_DE_AGUA 100000000
#define NUMERO_THREADS 4
//Declaracion de variables globales
pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
long valorICirculo = 0;
long cantidadGotasThread;

//Funcion para calcular la aprox de monte carlo por thread
void *calculoPiThread(){
  //Declaracion de variables
  double x,y;
  unsigned int valorRandom = rand();
  long valorLocalThread = 0;
  long i;
  for (i = 0; i < cantidadGotasThread; i++){
    //Generar los puntos random
    x = rand_r(&valorRandom) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
    y = rand_r(&valorRandom) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
    //Formula de circulo para ver si esta en el circulo, sumamos 1 a nuestro contador local
    if ((x*x) + (y*y) < 1){
      valorLocalThread++;
    }
  }
  //Exclusion mutua para entrar en seccion critica y modificar el valor global
  pthread_mutex_lock(&threadMutex);
  valorICirculo += valorLocalThread;
  pthread_mutex_unlock(&threadMutex);
  pthread_exit(0);
}

//Declaracion de la funcion main
int main(int argc, const char *argv[]){
    //Declaracion de variables
    cantidadGotasThread = GOTAS_DE_AGUA / NUMERO_THREADS; //Calculamos la cantidad de gotas de agua por thread
    time_t contadorTiempo = time(NULL);
    srand((unsigned)time(NULL)); //Seed
    pthread_t *hilos = malloc(NUMERO_THREADS * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int i;
    //Ciclo for para crear los threads
    for (i = 0; i < NUMERO_THREADS; i++){
      pthread_create(&hilos[i], &attr, calculoPiThread, (void *) NULL);
    }
    //Ciclo for para esperar por todos los threads
    for (i = 0; i < NUMERO_THREADS; i++){
      pthread_join(hilos[i], NULL);
    }
    //Liberar memoria!
    pthread_mutex_destroy(&threadMutex);
    free(hilos);
    //Calculo final de PI
    printf("PI: %.20f\n", (4. * (double)valorICirculo) / ((double)cantidadGotasThread * NUMERO_THREADS));
    printf("Timpo de CPU: %d sec\n", (unsigned int)(time(NULL) - contadorTiempo));
    return 0;
}