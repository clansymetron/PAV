#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sndfile.h>

#include "vad.h"
#include "vad_docopt.h"

#define DEBUG_VAD 0x1

int main(int argc, char *argv[]) {
  int verbose = 0; /* To show internal state of vad: verbose = DEBUG_VAD; */

  SNDFILE *sndfile_in, *sndfile_out = 0;
  SF_INFO sf_info;
  FILE *vadfile;
  int n_read = 0, i;

  VAD_DATA *vad_data;
  VAD_STATE state, last_state, defined_state;

  float *buffer, *buffer_zeros;
  int frame_size;         /* in samples */
  float frame_duration;   /* in seconds */
  unsigned int t, last_t, defined_t; /* in frames */

  char	*input_wav, *output_vad, *output_wav;
  float alfa1; /*Cambiar el valor de alpha1*/
  float alfa2;
  DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "2.0");

  verbose    = args.verbose ? DEBUG_VAD : 0;
  input_wav  = args.input_wav;
  output_vad = args.output_vad;
  output_wav = args.output_wav;
  alfa1 = atof(args.alfa1); //reading from argument first treshold
  alfa2 = atof(args.alfa2); 
  

  if (input_wav == 0 || output_vad == 0) {
    fprintf(stderr, "%s\n", args.usage_pattern);
    return -1;
  }

  /* Open input sound file */
  if ((sndfile_in = sf_open(input_wav, SFM_READ, &sf_info)) == 0) {
    fprintf(stderr, "Error opening input file %s (%s)\n", input_wav, strerror(errno));
    return -1;
  }

  if (sf_info.channels != 1) {
    fprintf(stderr, "Error: the input file has to be mono: %s\n", input_wav);
    return -2;
  }

  /* Open vad file */
  if ((vadfile = fopen(output_vad, "wt")) == 0) {
    fprintf(stderr, "Error opening output vad file %s (%s)\n", output_vad, strerror(errno));
    return -1;
  }

  /* Open output sound file, with same format, channels, etc. than input */
  if (output_wav) {
    if ((sndfile_out = sf_open(output_wav, SFM_WRITE, &sf_info)) == 0) {
      fprintf(stderr, "Error opening output wav file %s (%s)\n", output_wav, strerror(errno));
      return -1;
    }
  }

  vad_data = vad_open(sf_info.samplerate,alfa1, alfa2);
  /* Allocate memory for buffers */
  frame_size   = vad_frame_size(vad_data);
  buffer       = (float *) malloc(frame_size * sizeof(float));
  buffer_zeros = (float *) malloc(frame_size * sizeof(float));
  for (i=0; i< frame_size; ++i) buffer_zeros[i] = 0.0F;

  frame_duration = (float) frame_size/ (float) sf_info.samplerate;
  last_state = ST_UNDEF; //last state is the last different state recorded.
  defined_state = ST_SILENCE; //defined state is considered to be the last SILENCE or VOICE state. In order to compute transitions 

  for (t = last_t = defined_t = 0; ; t++) { /* For each frame ... */
    /* End loop when file has finished (or there is an error) */
    if  ((n_read = sf_read_float(sndfile_in, buffer, frame_size)) != frame_size) break;

    if (sndfile_out != 0) {
      sf_write_float(sndfile_out, buffer, frame_size); //We write into the output file the samples of the buffer
    }

    state = vad(vad_data, buffer);
    if (verbose & DEBUG_VAD) vad_show_state(vad_data, stdout);
    
    /* TODO: print only SILENCE and VOICE labels */
 
    if (state != last_state) {
      if (t != last_t){ 
        //We see if there is a change from Silence to Voice or Voice to Silence. If it is the case, we write into the vad file.
        if((defined_state==ST_SILENCE && state==ST_VOICE && last_state==ST_MYB_VOICE) || (defined_state ==ST_VOICE && state== ST_SILENCE && last_state==ST_MYB_SILENCE)){
            
            //defined_t points to the time where Silence or Voice starts. It refers to the previous one in analysis. last_t -1 points to the time where the state was different for last time.
            fprintf(vadfile, "%.5f\t%.5f\t%s\n", defined_t * frame_duration, (last_t-1) * frame_duration, state2str(defined_state));
            
            if (sndfile_out != 0 && defined_state == ST_SILENCE) { //if there is output file and the last defined state is Silence (which means we have just switch to voice) we put to 0 all the samples of silence.
              sf_seek(sndfile_out, defined_t* frame_size, SEEK_SET); //Point to the position of the file where the Silence starts (defined_t * frame_size)
              int frames = last_t-1-defined_t; //Difference of frames in which the state has taken place
              
              sf_write_float(sndfile_out,buffer_zeros, frames* frame_size); //put to 0 all the samples
              
              sf_seek(sndfile_out, 0, SEEK_END); // point to the end of the file for the next iteration to write the buffer
    }
            defined_t = last_t-1; //the defined_t will become last_t-1 as it is now the last different state.
            defined_state = state; //the defined_state will now become the current state. As it is or Silence or Voice
        }
      }
      if (last_state == ST_UNDEF){
           last_state=state; //for the first iteration
        }
        
      last_state = state; //updating the state of the loop and the last_state
      last_t = t;
      
    }  

    
  }

  state = vad_close(vad_data);
  /* TODO: what do you want to print, for last frames? */
  if (t != last_t){
    if(state==ST_VOICE || state== ST_SILENCE){ //If the current state is Defined, We do the same as in the previous loop but with the last frame

      fprintf(vadfile, "%.5f\t%.5f\t%s\n", defined_t * frame_duration, t * frame_duration + n_read / (float) sf_info.samplerate, state2str(state));
      
      if (sndfile_out != 0 && state == ST_SILENCE) {
              sf_seek(sndfile_out, defined_t* frame_size, SEEK_SET);
              int frames = t-defined_t;
              
              sf_write_float(sndfile_out,buffer_zeros, frames* frame_size + n_read); //We need to consider the n_read samples of last frame 
              //Here there is no need to update the pointer as it is the last frame
      }
    }

    else{ //if it is in an undefined_state (MYBE) we have to consider the output
      if(defined_state == ST_SILENCE){
        state = ST_SILENCE; //if the last defined state was silence, it will be silence
         if (sndfile_out != 0) {
              sf_seek(sndfile_out, defined_t* frame_size, SEEK_SET);
              int frames = t-defined_t;
              fprintf(stdout, "%d  ", frames);
      }  }
        else state = ST_VOICE; // if the defined_state was voice, it will be voice. But normally the end of files appear to be silence
     fprintf(vadfile, "%.5f\t%.5f\t%s\n", defined_t * frame_duration, t * frame_duration + n_read / (float) sf_info.samplerate, state2str(state));
      }
    
   
  }
    

  /* clean up: free memory, close open files */
  free(buffer);
  free(buffer_zeros);
  sf_close(sndfile_in);
  fclose(vadfile);
  if (sndfile_out) sf_close(sndfile_out);
  return 0;
}
