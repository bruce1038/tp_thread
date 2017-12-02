#include "synchro.h"
#include "ensitheora.h"

int vides = 30 ;
bool fini;
cond fp ; //con d'attente pour les producteurs
cond fc ; //con d'attente pour les consommateurs

/* les variables pour la synchro, ici */

pthread_mutex_t m;
pthread_mutex_init(&m, NULL);
pthread_cond_t window, texture;
pthread_cond_init(&window,NULL);
pthread_cond_init(&texture,NULL);
int Taille, TailleFenetre;
Taille = 0;
pthread_cond_t fConsom, fDepot;
pthread_cond_init(&fConsom,NULL);
pthread_cond_init(&fDepot,NULL);


/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
  pthread_mutex_lock(&m);
  windowsx := buffer[0].width;
  windowsy := buffer[0].length;
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
  while (windowsx == 0 && windowsy == 0)
    pthread_cond_wait(&texture, &m);
  pthread_mutex_unlock(&m);
}

void debutConsommerTexture() {
  pthread_mutex_lock(&m);
  while(vides==NBTEX){
    pthread_cond_wait(&fConsom, &m);
  }
  vides --;
  //consommation
  //pthread_cond_broadcast(&plein);
  pthread_mutex_unlock(&m);
}

void finConsommerTexture() {
  pthread_mutex_lock(&m);
    pthread_cond_signal(&fDepot);
  pthread_mutex_unlock(&m);
}


void debutDeposerTexture() {
  pthread_mutex_lock(&m);
  if(vides == 0){
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
