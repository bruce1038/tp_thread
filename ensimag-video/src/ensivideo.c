#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "stream_common.h"
#include "oggstream.h"


int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);


    // start the two stream readers
    pthread_t ReadImg, ReadSon ;

    pthread_create(&ReadImg,NULL,theoraStreamReader,argv[1]);
    pthread_create(&ReadSon,NULL,vorbisStreamReader,argv[1]);

    // wait audio thread
    pthread_cond_t c;
    pthread_cond_init(&c,NULL);
    pthread_cond_wait(&c,&ReadSon);
    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(&ReadSon);
    pthread_cancel(&ReadImg);

    // attendre les 2 threads videos
    pthread_cond_wait(&c,&ReadSon);
    pthread_cond_wait(&c,&ReadImg);

    exit(EXIT_SUCCESS);

}
