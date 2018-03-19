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

 
//define SR_NOSOUND
#include <stdio.h>
#include "silentradiancesimple.h"

#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif



#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif

 

int audio_len=0;   
int audio_done=0;   
int read_files=0;
int process_packets=0;

 
void nothing() {}   

int playing=0;

int stop() {
if (playing) {
 shutdown_processor();
// finish_stream_sound();
// shutdown_web_stream(); 
 playing=0;
 } 
return(0);
}



int play() {
if (!playing) {

  init_processor();
  //timer_on_polling_flag=1; // ???
  fprintf(stderr,"opening url\n");
  init_web_stream("download7.cgi");

  //soundPlayCool=1;
  init_receiver_sound(1);
  playing=1;
  }
return(0);
}






int main(int argc,char *argv[]) {

play();     
emscripten_set_main_loop(nothing,5,1);  
  
return(0);
}

/* end of file */
