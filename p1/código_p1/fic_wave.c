#include <stdio.h>
#include "fic_wave.h"

FILE    *abre_wave(const char *ficWave, float *fm, int *mod, short *channel, int *muestras) {
    FILE    *fpWave;
    int fmi; //para leer los 4 bytes (int) de frecuencia
    short PCM; //leer si es PCM son 2 bytes (short en c) igual que el canal
    if ((fpWave = fopen(ficWave, "r")) == NULL) return NULL;
    
    //modulation
    if(fseek(fpWave, 16 , SEEK_SET)<0) return NULL;
    fread(mod, 4, 1 , fpWave);
    fread(&PCM, 2, 1, fpWave);
    if(*mod!=16 || PCM!=1) return NULL; //SI ES PCM ES 1

    //lectura canal
    if(fseek(fpWave, 22, SEEK_SET)<0) return NULL;
    fread(channel, 2, 1 , fpWave);
    if(*channel != 1) return NULL;

    //lectura frecuencia
    if (fseek(fpWave, 24, SEEK_SET) < 0) return NULL;
    fread(&fmi, 4, 1 , fpWave);

    if (fseek(fpWave, 40, SEEK_SET) < 0) return NULL;
    fread(muestras, 4, 1 , fpWave);

    //posicionamiento en 44 byte para empezar a leer datos
    if (fseek(fpWave, 44, SEEK_SET) < 0) return NULL;
    *fm= (float)fmi;
    

    return fpWave;
}

size_t   lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave) {
    return fread(x, size, nmemb, fpWave);
}

void    cierra_wave(FILE *fpWave) {
    fclose(fpWave);
}
