/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2018 Hibbard M. Engler

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






//
// Created by hib on 6/17/16.
//

#ifndef DASOUND24_PROCESSOR_H
#define DASOUND24_PROCESSOR_H
#include <stdio.h>

#include <opus/opus.h>



#define SOUNDRING_COUNT 400
#define COLLECTION_COUNT 400

#ifndef PER_FRAME_OVERHEAD
#define PER_FRAME_OVERHEAD 16
#endif

typedef struct processor_stat {
  int len; /* - if not yet decoded via opus , + of decoded - always 960, 0 if this stat is empty */
  int frame; /* frame number */
  unsigned int version;
  char stats_area[PER_FRAME_OVERHEAD]; // only set for SR00, not SR01 */
  } processor_stat;




extern OpusDecoder *opusdecoder;
extern FILE *write_file_spec;

extern volatile struct processor_stat *soundstat[SOUNDRING_COUNT];
extern volatile struct processor_stat soundstat_area[SOUNDRING_COUNT];

extern volatile int soundringfirst; /* first one on record - we can go back that far */
extern volatile int soundringcurrent; /* we are playing this RIGHT NOW: -1 if we are not playing.  This i*/

extern volatile int soundringhead;
extern volatile int soundringtail;
extern volatile short *soundring[SOUNDRING_COUNT];
extern volatile short *soundring[SOUNDRING_COUNT];  /* points to soundring_area */

extern volatile char *commandring[SOUNDRING_COUNT];
extern volatile char commandring_area[SOUNDRING_COUNT][2000]; /* commands for the sound ring */
extern volatile int commandlen[SOUNDRING_COUNT];



#define NUMBER_OF_PACKETS_BEFORE_ACTIVATING_SOUND 80

extern int collectionhead;
extern int collection_size; /* collection size */


extern int base_frame;
extern short *collections[COLLECTION_COUNT];
extern volatile struct processor_stat *collectionstat[COLLECTION_COUNT];

extern void process_packet(int recvStringLen, unsigned char *packetbuffer);
extern void process_packet_ignore_framestart(int recvStringLen, unsigned char *packetbuffer); /* framestart is uses to keep frames in order for opus decoding.
               But in a streaming application where we don't have to retry (TCP/IP or other stream) the order is known */
extern void init_processor(void);
extern void init_processor_buffers(void); /*used by analyzer to init the processor command sound, and collection buffers, but without opus though */
extern void shutdown_processor(void);

typedef struct simple_silent_radiance {
  OpusDecoder *opusdecoder;
  volatile struct processor_stat soundstat_area[SOUNDRING_COUNT];
  volatile int soundringfirst; /* first one on record - we can go back that far */
  volatile int soundringcurrent; /* we are playing this RIGHT NOW: -1 if we are not playing.  This i*/

  volatile int soundringhead;
  volatile int soundringtail;
  volatile short soundring_area[SOUNDRING_COUNT][960];

  #define NUMBER_OF_PACKETS_BEFORE_ACTIVATING_SOUND 80
  float minpcm;
  float maxpcm;

  } simple_silent_radiance;



extern void process_packet(int recvStringLen, unsigned char *packetbuffer);
extern void process_packet_ignore_framestart(int recvStringLen, unsigned char *packetbuffer);
extern void shutdown_processor();


#endif //DASOUND24_PROCESSOR_H
