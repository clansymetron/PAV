#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[]) {
    float durTrm = 0.010;
    float fm;
    int   N;
    int   trm;
    float *x;
    short *buffer;
    int mod;
    short channel;
    int muestras;
    FILE  *fpWave;
    FILE  *fpResult;
    FILE  *stdout_P1 = stdout;

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }

    if ((fpWave = abre_wave(argv[1], &fm, &mod, &channel, &muestras)) == NULL) {
        fprintf(stderr, "Error al abrir el fichero WAVE de entrada %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }

    //lectura 2 parametro. 
    if(argc ==3){
        fpResult = fopen(argv[2], "w");
        stdout_P1 = fpResult; //Hacemos que la salida estándar sea el descriptor de fichero de escritura
    } 

    N = durTrm * fm;
    if ((buffer = malloc(N * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno));
        return -1;
    }

    trm = 0;
    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) {
        for (int n = 0; n < N; n++) x[n] = buffer[n] / (float) (1 << 15);

        //Al no ser binario se puede hacer print sobre el descriptor de fichero directamente sin necesidad del fwrite para binarios
        fprintf(stdout_P1, "%d\t%f\t%f\t%f\n", trm, compute_power(x, N),
                                        compute_am(x, N),
                                        compute_zcr(x, N, fm));
        trm += 1;
    }
    fprintf(stdout_P1, "FM = %.2f Hz, ",fm);
    fprintf(stdout_P1, "mod= %d PCM, ",mod);
    fprintf(stdout_P1, "channel= %d (mono), ",channel);
    fprintf(stdout_P1, "bytes = %d ",muestras);
    cierra_wave(fpWave);
    free(buffer);
    free(x);

    return 0;
}
