#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {
    float res = 0;
    for(int i = 0;i < N;i++)
    	res += (x[i] * x[i]);
    
    return (float) 10*log10(res/N);
    
}

float compute_am(const float *x, unsigned int N) {
    float res=0;
    for(int i = 0 ;i < N;i++)
    	res += (float) fabs(x[i]);
        
    return res/N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
   float res=0;
   for(int i =1; i < N;i++){
   	if((x[i]>0 && x[i-1]<0) || (x[i]<0 && x[i-1]>0) ) //falta considerar caso 0
   	    res++;
   }
   return res*fm/(2*(N-1));
   
}
