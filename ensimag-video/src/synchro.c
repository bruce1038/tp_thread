#include "synchro.h"
#include "ensitheora.h"

int vides = 30 ;
bool fini;
cond fp ; //con d'attente pour les producteurs
cond fc ; //con d'attente pour les consommateurs

/* les variables pour la synchro, ici */

pthread_mutex_t m;
pthread_mutex_init(&m, NULL);
pthread_cond_t vide, plein;
//pthread_cond_init(&vide,NULL);
//pthread_cond_init(&plein,NULL);
int Taille, TailleFenetre;
Taille = 0;
pthread_cond_t fConsom, fDepot;
pthread_cond_init(&fConsom,NULL);
pthread_cond_init(&fDepot,NULL);


/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
  pthread_mutex_lock(&threadAffichage);
  windowsx := buffer[0].width;
  windowsy := buffer[0].length;
  pthread_mutex_unlock(&threadAffichage);
}

void attendreTailleFenetre() {
  pthread_mutex_lock(&threadAffichage);
  while (windowsx == 0 && windowsy == 0)
  {
    pthread_cond_wait(&threadAffichage);
  }
  pthread_mutex_unlock(&threadAffichage);
}

void signalerFenetreEtTexturePrete() {
  pthread_cond_signal(&threadAffichage);
  pthread_cond_signal(&threadVideo);

}

void attendreFenetreTexture() {
  pthread_mutex_lock(&threadAffichage);
  while (windowsx == 0 && windowsy == 0)
  {
    pthread_cond_wait(&threadAffichage);
  }
  pthread_mutex_unlock(&threadAffichage);
}

void debutConsommerTexture() {
  pthread_mutex_lock(&m);
  while(vides==NBTEX){
    pthread_cond_wait(&fConsom,&m);
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
