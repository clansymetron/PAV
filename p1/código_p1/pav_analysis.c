#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {
    float res=0;
    int i;
    for(i=0;i<N;i++){
    	res=res + ((x[i] )* x[i]);
    }
    res= res/N;
    res= (float) 10*log10(res);
    return res;
}

float compute_am(const float *x, unsigned int N) {
    float res=0;
    int i;
    for(i=0;i<N;i++){
    	res=res + (float) fabs((x[i] ));
    }
    res= res/N;
    return res;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
   int i;
   float res;
   for(i=1;i<N;i++){
   	if((x[i]>0 && x[i-1]<0) || (x[i]<0 && x[i-1]>0) )
   	res=res+1;
   }
   res= res*fm/(2*(N-1));
   return res;
}
