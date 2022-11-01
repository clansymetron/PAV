#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vad.h"
#include "pav_analysis.h"

const float FRAME_TIME = 10.0F; /* in ms. */
int Nint = 0; /*Constant that counts the number of Tramas Iniciales */
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
  
  /*we compute the different usefull features useing functions created in last Practica("pav_analysis.c") */
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
  float time_passed = FRAME_TIME * 1e-3 * vad_data->maybe_count; /*Time en s. that we are in the state MaybeSilence(MYBS) or MaybeVoice(MYBV) */

  switch (vad_data->state) {
  case ST_INIT: 
    /*When the number of frames is 15 we enter the if and compute the Tresholds. We have done this to stablish the first treshold to the level of noise 
     of the system in which the recording was recorded. Doing the logarythmic mean of the 15 first frames*/

    if(Nint==15){
      vad_data->umbral1 = 10*log10((vad_data->umbral1)/Nint) + vad_data->alpha1;
      //fprintf(stdout, "%f", vad_data->umbral1);
      vad_data->umbral2 = vad_data->umbral1 + vad_data->alpha2;
      vad_data->state = ST_SILENCE;
      Nint =0;
    }
    else{
        
        vad_data->umbral1 += pow(10, (f.p/10) );
        //fprintf(stdout, "%f", vad_data->umbral1);
        Nint++;
      }

    break;

  case ST_SILENCE:
    if (f.p > vad_data->umbral1)
      vad_data->state = ST_MYB_VOICE; /*If Power of frame is bigger than Treshold1 we enter MYBV state*/
    break;

  case ST_VOICE:
    if (f.p < vad_data->umbral2)
      vad_data->state = ST_MYB_SILENCE; /*If power is lower than Tresholdl2 we enter MYBS state*/
    break;

    case ST_MYB_SILENCE: 
        if(f.p < vad_data->umbral2){  
          if(time_passed>0.266 || (time_passed>0.204 && f.p<vad_data->umbral1)){  /*We use 2 conditions to change to Silence state beacause 
                                                                                      we want to be very sure we are in Silence state and not lose info.*/
             vad_data->state = ST_SILENCE;
             vad_data->maybe_count= 0; /*restart the Maybe counter*/
          }
          else {
            vad_data->maybe_count ++;  /*If conditions are not passed we add 1 to the count of frames in a Maybe state */
          }
        }
        else{
          vad_data->state = ST_VOICE; /*If conditions above are not passed we can say we are in Voise state*/
          vad_data->maybe_count= 0;  /*restart the Maybe counter*/

        }


    break;

    case ST_MYB_VOICE:
        if(f.p > vad_data->umbral1){
          if((time_passed>0.05 && f.p>vad_data->alpha2) || time_passed >0.08){ /*We are less restrictive to enter Voice state to not lose information*/
             vad_data->state = ST_VOICE;
             vad_data->maybe_count= 0;  /*restart the Maybe counter*/
          }
          else {
            vad_data->maybe_count ++; /*If conditions are not passed we add 1 to the count of frames in a Maybe state */
          }
        }
        else{
          vad_data->state = ST_SILENCE;
          vad_data->maybe_count= 0;  /*restart the Maybe counter*/

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
