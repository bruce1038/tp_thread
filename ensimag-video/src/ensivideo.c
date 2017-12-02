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

    // start the two stream
    pthread_t threadVideo, threadAudio, threadAffichage;
    pthread_create(&threadVideo, NULL, theoraStreamReader, (void *) argv[1]);
    pthread_create(&threadAudio, NULL, vorbisStreamReader, (void *) argv[1]);

    // wait audio thread
    pthread_join(threadAudio, NULL);

    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(threadVideo);
    pthread_cancel(threadAffichage);

    // attendre les 2 threads videos
    pthread_join(threadVideo, NULL);
    pthread_join(threadAffichage, NULL);

    exit(EXIT_SUCCESS);
}
