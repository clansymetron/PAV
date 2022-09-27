#include <stdio.h>
#include "fic_wave.h"

FILE    *abre_wave(const char *ficWave, float *fm) {
    FILE    *fpWave;

    if ((fpWave = fopen(ficWave, "r")) == NULL) return NULL;
    fseek(fpWave, 24, SEEK_SET);
    fread(fm, sizeof(char), 4 , fpWave);
    
    if (fseek(fpWave, 44, SEEK_SET) < 0) return NULL;


    return fpWave;
}

size_t   lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave) {
    return fread(x, size, nmemb, fpWave);
}

void    cierra_wave(FILE *fpWave) {
    fclose(fpWave);
}
