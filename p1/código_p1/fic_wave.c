#include <stdio.h>
#include "fic_wave.h"

FILE    *abre_wave(const char *ficWave, float *fm) {
    FILE    *fpWave;
    int fmi; //para leer los 4 bytes (int) de frecuencia

    if (fpWave == fopen(ficWave, "r") == NULL) return NULL;
    
    //lectura frecuencia
    if (fseek(fpWave, 24, SEEK_SET) < 0) return NULL;
    fread(&fmi, sizeof(int), 1 , fpWave);
    
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
