#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vad.h"
#include "pav_analysis.h"

const float FRAME_TIME = 10.0F; /* in ms. */
int Nint = 0;
/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT", "MYBS", "MYBV"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
 
  Features feat;
  feat.p=compute_power(x,N);
  feat.zcr = compute_zcr(x, N,16000);
  feat.am = compute_am(x, N);
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate,float alfa1, float alfa2) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  vad_data->alpha1=alfa1;
  vad_data->alpha2 = alfa2;
  vad_data->maybe_count = 0;
  vad_data->umbral1 = 0;
  vad_data->umbral2 =0;
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */
  float time_passed = FRAME_TIME * 1e-3 * vad_data->maybe_count;

  switch (vad_data->state) {
  case ST_INIT: 

    if(Nint==15){
    vad_data->umbral1 = 10*log10((vad_data->umbral1)/Nint) + vad_data->alpha1;
    fprintf(stdout, "%f", vad_data->umbral1);
    vad_data->umbral2 = vad_data->umbral1 + vad_data->alpha2;
    vad_data->state = ST_SILENCE;
    Nint =0;
    }
    else{
        
        vad_data->umbral1 += pow(10, (f.p/10) );
        fprintf(stdout, "%f", vad_data->umbral1);
        Nint++;
      }

    break;

  case ST_SILENCE:
    if (f.p > vad_data->umbral1)
      vad_data->state = ST_MYB_VOICE;
    break;

  case ST_VOICE:
    if (f.p < vad_data->umbral2)
      vad_data->state = ST_MYB_SILENCE;
    break;

    case ST_MYB_SILENCE: //cruces por cero
        if(f.p < vad_data->umbral2){
          if(time_passed>0.266 || (time_passed>0.204 && f.p<vad_data->umbral1)){
             vad_data->state = ST_SILENCE;
             vad_data->maybe_count= 0;
          }
          else {
            vad_data->maybe_count ++;
          }
        }
        else{
          vad_data->state = ST_VOICE;
          vad_data->maybe_count= 0;

        }


    break;

    case ST_MYB_VOICE:
        if(f.p > vad_data->umbral1){
          if((time_passed>0.05 && f.p>vad_data->alpha2) || time_passed >0.08){
             vad_data->state = ST_VOICE;
             vad_data->maybe_count= 0;
          }
          else {
            vad_data->maybe_count ++;
          }
        }
        else{
          vad_data->state = ST_SILENCE;
          vad_data->maybe_count= 0;

        }


    break;

  case ST_UNDEF:
    break;
  }

  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_VOICE || vad_data->state == ST_MYB_SILENCE ||
      vad_data->state == ST_MYB_VOICE) 
    return vad_data->state;
  else
    return ST_UNDEF;
}

void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
