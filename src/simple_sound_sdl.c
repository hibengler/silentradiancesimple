/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/* for testing performance */
//#define DO_NO_SOUND 1

/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

#include <stdio.h> /* for printf() and fprintf() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#include <errno.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

/*#define THREADING_ON 1 
^^^ for threadding example - doesnt help much just burns a cpu */

#include "simple_processor.h"

#include "simple_sound_sdl.h"

extern int audio_done;

short zero_buffer[480];

short * getNextQueue240() {
char status;

   status='z';
      short *nextBuffer=zero_buffer;

  
    if (soundringhead == soundringtail) { /* Empty zero it! */
        status='a';
        soundringcurrent=-1;
    } else {
        if (soundringcurrent==-1) {  /* was emptied. Check to see if we have enough */
            /* Here we should wait until we got at least 4 or so */
            int queue_size;
            queue_size = soundringtail-soundringhead;
            if (queue_size<0) queue_size += SOUNDRING_COUNT;
            if (queue_size>200) { /* ok, we have 48 consecutive ones. Go at it! */
                soundringfirst=soundringhead;
                soundringcurrent=soundringhead;
		status='2';
                }
            }
        /* OK - here we may or may not be doing real sound.  so do one of the other */
        if (soundringcurrent==-1) { /* still not good enough */
            /* do nothing here - we are already set to zero_buffer */
        }
        else {
            soundringcurrent = soundringhead;
            nextBuffer = (short *)soundring[soundringcurrent];
	    status=' ';
            soundringhead = (soundringhead + 1) % SOUNDRING_COUNT;
        }
    }
//    if (nextBuffer==zero_buffer) {fprintf(stderr,"%c",status);}
    return  nextBuffer;

}


  int init_send_sound=1;
  long loops;
  int rc;
  int size;
  unsigned int val;
  int dir;
  char *buffer;
  struct pollfd *ourpolls;
  int soundpolls_count;
  int socket_poll;
  
  int sound_state = 0;
  
  static int frame=0;
int no_sound = 0;


    SDL_AudioSpec obtained;




int init_receiver_sound(int block) {
/* from https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideaudioexamples.html */
fprintf(stderr,"setting up audio\n");


SDL_Init(SDL_INIT_AUDIO);


    SDL_AudioSpec wanted;
    extern void fill_audio(void *udata, Uint8 *stream, int len);

    /* Set the audio format */
    wanted.freq = 48000;
    wanted.format = AUDIO_S16;
    wanted.channels = 2;    /* 1 = mono, 2 = stereo */
    wanted.samples = 512;  /* Good low-latency value for callback */
    wanted.callback = fill_audio;
    wanted.userdata = NULL;

    /* Open the audio device, forcing the desired format */
    if ( SDL_OpenAudio(&wanted, &obtained) < 0 ) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return(-1);
    }
    if (obtained.freq != wanted.freq) {
      fprintf(stderr,"freq is %d",obtained.freq);
      }
    if (obtained.format != wanted.format) {
      fprintf(stderr,"format is %d",obtained.format);
      }
    if (obtained.samples!= wanted.samples) {
      fprintf(stderr,"samples is %d",obtained.samples);
      }
    


  size = 4000;
  buffer = (char *) malloc(size);
  
fprintf(stderr,"setting up zero buffer\n");
sound_state=0;
{
  int i;
  for (i=0;i<480;i++) {
//   zero_buffer[i]=(i*80)%32768;   /* for debugging */
    zero_buffer[i]=0;
    }
  }
SDL_PauseAudio(0);

fprintf(stderr,"Sound is OK\n");
return 0;
}


int finish_play_sound() {
  free(buffer);
return(0);
}


void DieWithError(char *errorMessage); /* External error handling function */

void DieWithError(char *errorMessage)
{
perror(errorMessage);
exit(1);
}












		    
			
			        



  

extern int audio_len;
 
  
 static Uint8 *audio_chunk;
 static Uint32 soundBufferSize=0;;
 static Uint8 *audio_pos;
static char *soundBuffer;

    /* The audio function callback takes the following parameters:
       stream:  A pointer to the audio buffer to be filled
       len:     The length (in bytes) of the audio buffer
    */
void fill_audio(void *udata, Uint8 *stream, int len)
{
audio_done++;
//fprintf(stderr,"%d,%d %d,%d,%d\n",len,soundBufferSize,soundringcurrent,soundringhead,soundringtail);
while (len) {
  if (soundBufferSize) {
    int len2 = (len>soundBufferSize)?soundBufferSize:len;
    audio_len += len2;
  
    memcpy(stream,audio_pos,len2);
//  SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    stream += len2;
    audio_pos += len2;
    soundBufferSize -= len2;
    len = len - len2;
    }
  if((len)&&(soundBufferSize<=0)) {
    /* get data now if we have none to begin with */
    soundBuffer = (char *)getNextQueue240();
    soundBufferSize = 960;
    audio_pos = (Uint8 *)soundBuffer;
    }
  } /* while we are outputting */
}


#ifdef crudsdsda
//SDL_PauseAudio(0);
//       fprintf(stderr,"%d\n",len);

        /* Mix as much data as possible */
//        len = ( len > audio_len ? audio_len : len );
//len=960;
//    }

    /* Load the audio data ... */

    ;;;;;

    audio_pos = audio_chunk;

    /* Let the callback function play the audio chunk */
    SDL_PauseAudio(0);

    /* Do some processing */

    ;;;;;

    /* Wait for sound to complete */
    while ( audio_len > 0 ) {
        SDL_Delay(100);         /* Sleep 1/10 second */
    }
    SDL_CloseAudio(); 
#endif



int playOneSoundBuffer() {
if (!soundBuffer) {
  soundBuffer = (char *)getNextQueue240();
  soundBufferSize=960;
  }
return(0);
}
