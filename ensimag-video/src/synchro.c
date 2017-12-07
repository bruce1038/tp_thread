#include "synchro.h"
#include "ensitheora.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int vides = 30 ;
bool fini;

/* les variables pour la synchro, ici */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t window = PTHREAD_COND_INITIALIZER;
pthread_cond_t texture = PTHREAD_COND_INITIALIZER;
int TailleFenetre;
int Taille = 0;
pthread_cond_t fConsom = PTHREAD_COND_INITIALIZER;
pthread_cond_t fDepot = PTHREAD_COND_INITIALIZER;


/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
  pthread_mutex_lock(&m);
  windowsx = buffer[0].width;
  windowsy = buffer[0].height;
  pthread_cond_signal(&window);
  pthread_mutex_unlock(&m);
}

void attendreTailleFenetre() {
  pthread_mutex_lock(&m);
  while (windowsx == 0 && windowsy == 0)
    pthread_cond_wait(&window, &m);
  pthread_mutex_unlock(&m);
}

void signalerFenetreEtTexturePrete() {
  pthread_mutex_lock(&m);
  pthread_cond_signal(&texture);
  pthread_mutex_unlock(&m);

}

void attendreFenetreTexture() {
  pthread_mutex_lock(&m);
  while (boolTexture == false)
  	pthread_cond_wait(&texture, &m);
  pthread_mutex_unlock(&m);
}

void debutConsommerTexture() {
  pthread_mutex_lock(&m);
  while(vides==NBTEX){
    pthread_cond_wait(&fConsom, &m);
  }
  vides --;
  pthread_mutex_unlock(&m);
}

void finConsommerTexture() {
  pthread_mutex_lock(&m);
    pthread_cond_signal(&fDepot);
  pthread_mutex_unlock(&m);
}


void debutDeposerTexture() {
  pthread_mutex_lock(&m);
  while (vides == 0){
    pthread_cond_wait(&fDepot,&m);
  }
  vides ++;
  pthread_mutex_unlock(&m);
}

void finDeposerTexture() {
  pthread_mutex_lock(&m);
    pthread_cond_signal(&fConsom);
  pthread_mutex_unlock(&m);
}
