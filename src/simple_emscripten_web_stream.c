#include <stdlib.h>
#include "simple_seven_eight_decode.h"
#include "simple_decrypt.h"
#include "simple_emscripten_web_stream.h"
#include "simple_processor.h"

#define protocol_log(stderr,...) 
#define only_protocol(retval) return(retval)
//#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}

int emscripten_already_initted=0;
int we_are_streaming_web=0;
unsigned long bytes_streamed=0;


#include<stdio.h>
#include<emscripten.h>
#include<assert.h>
#include<string.h>
#include <time.h>
 
struct webstream {
  char original_url[4000];
  char doctored_url[4000];
  void *data;
  long original_byte_position;
  int offset_for_processing;
  int current_length;
  };
  
#define MAX_NUMBER_OF_WEB_STREAMS 4
struct webstream active_streams[MAX_NUMBER_OF_WEB_STREAMS];

sr_simple_seven_eight_decode *sr78dec=NULL;


int get_packet_from_web_chunk(int *,unsigned char *);
extern int read_files;
extern int process_packets;
extern int bytes_read;


/*
int emscripten_async_wget2(const char* url, 
const char* file,  const char* requesttype, 
const char* param, void *arg, em_async_wget2_onload_func 
onload, em_async_wget2_onstatus_func onerror, em_async_wget2_onstatus_func onprogress);
above as example
*/

int hib_async_wget2(const char *url,const char *ss,const char *tt,const char *uu);


long expected_new_start=0l;

static char new_byte_position_string[400];

int read_stream_data_into_process(unsigned char *data,long new_start,int len, char *server_content_type);




char * web_stream_process(
             char *file_start_byte_position,
	     char *original_byte_position,
             int offset,
	     int len,
	     int offset_in_8_bit,
	     unsigned char *data,
	     int reqnum,
	     char *server_content_type) {
data +=8;
//fprintf(stderr,"wow\n");
//fprintf(stderr,"wow  file_start_byte_position %s   original_byte_position %s   offset %d  len %d  data (%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x) reqnum %d\n",
//         file_start_byte_position,original_byte_position,offset,len,data[0],data[1],data[2],
//	 data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15],reqnum);
unsigned char *b=data;
data = b;
	   
long file_start = atol(file_start_byte_position);
long original = atol(original_byte_position);
long new_start = file_start + offset_in_8_bit;

if (sr78dec==NULL) {
  sr78dec = new_sr78dec();
  }

//int real_length = read_stream_data_into_process(data,new_start,len,server_content_type);
int real_length = read_stream_data_into_process(data,offset,len,server_content_type);

sprintf(new_byte_position_string,"%16ld",new_start+ ((long)real_length));
return new_byte_position_string;
}





void web_stream_onerror(void *varg,void *vbuf,unsigned int len) 
{
/* should probably transition to an internal sound */
}


void web_stream_fini(void *varg,void *vbuf,unsigned int len) 
{
}




/* note - this might be re-called by the receive loop based on various errors
because we sometimes get a recvfrrom failed error on android, when switching
networks */
void init_web_stream(char *url) {
//int x = EM_ASM_INT({ return hib_async_wget2($0,"GET","","");},url);
if (sr78dec==NULL) {
  sr78dec = new_sr78dec();
  }
int x=hib_async_wget2(url,"GET","","");
if (!x) {
  fprintf(stderr,"calling the site did not work\n");
  return;
  }
expected_new_start=-1;

we_are_streaming_web=1; 
bytes_streamed=0;


}

  
  
int the_web_stream_offset_in_8_bit=0;
  
int web_stream_offset_in_8_bit() {
return the_web_stream_offset_in_8_bit;
}




static unsigned char bufferbig[4000];
static int save_buffer=0;
static int save_len=0;



static int recvStringLen=0;
static unsigned char packetbuffer[4000];

unsigned char *wdata = NULL;
int wlen=0;

int read_stream_data_into_process(unsigned char *data,long new_start,int len, char *server_content_type) {
int translated_len=len;
//fprintf(stderr,"read_stream_data_into_process  packet %ld 	len %d 	char1 %x%x\n",new_start,len,data[0],data[1]);
if (expected_new_start != -1) {
  if (new_start != expected_new_start) {
    fprintf(stderr,"bad. new start was ssupposed to be %ld. got %ld\n",new_start,expected_new_start);
    save_buffer=0;
    save_len=0;
    sr78dec->seven_to_eight_res_count = '\0';
    }
  }
expected_new_start = new_start+len;  
unsigned char *d2 = NULL;
if (sr78dec_we_are_7_bit(sr78dec,data,len,server_content_type)) {   /* should get the content type from app.js ??? */
  d2 = data-8; /*Wha???!*/

  translated_len= sr78dec_seven_to_8(sr78dec,d2,data,len);
  the_web_stream_offset_in_8_bit = translated_len;
  
  data = d2;
  wdata = (unsigned char *)d2;
  }
else {
  fprintf(stderr,"what? eight? what?\n");
  wdata = (unsigned char *)data;
  }  
wlen = translated_len;
while (wlen) {
 if (get_packet_from_web_chunk(&recvStringLen,packetbuffer)) {
    process_packet_ignore_framestart(recvStringLen,packetbuffer);
    }
  }

return translated_len;
}




/* so we saved the utf16 codepoints in javascript - and we are trying to go back through the 
javascript crap that assumes utf8 or something and makes utf16
I don't know how it really works, so I am going to send some tests
*/
static int wread(unsigned char *w,int size,int count) {
int sizecount = size*count;
//fprintf(stderr, "wread size %d count %d wlen %d\n",size,count,wlen);
int move = 0;
//fprintf(stderr, "move %d\n",move);
int i;
i=0;
while ((wlen>0)&&(move<sizecount)) {
  w[move++] = *wdata++;
  wlen--;
  } /* the loop */
//fprintf(stderr, "moved %d\n",i);
return move;
}




static int weof() {
return 0;
}



int stream_slide_until_sr_indicator(char *startbuf,int number_in_buf)
{
int i=0;
int state = 0;
char ch;
while (1) {
  if (i<number_in_buf) {
    ch=startbuf[i];
    }
  else {
    int f=wread((void *)startbuf,1,1);
    if (!f) return 0;
    bytes_streamed++;
    ch = startbuf[0];
    }
  if (state==0) {
    if (ch=='s') state = 1; else state=0;
    }
  else if (state==1) {
    if (ch=='r') state = 2; else state=0;
    }
  else if (state==2) {
    if (ch=='0') state = 3; else state=0;
    }
  else if (state==3) {
    if ((ch=='0')||(ch=='1')) return(1); else state=0;
    }
  i++;
  }
}



int hold_off_because_too_busy() {
    int queue_size = soundringtail-soundringhead;
    if (queue_size<0) queue_size += SOUNDRING_COUNT;
    if (queue_size >(SOUNDRING_COUNT-250)) return(1);
return(0);
}


int sliding=0;
static int lc=0;
static unsigned char bufferbig[4000];
int captured_so_far=0;
char *the_packet_web_buffer;
int internal_recvStringLen=0;


int get_packet_from_web_chunk(int *precvStringLen,unsigned char *packetbuffer) {
protocol_log(stderr,"lc %d sliding %d captured_so_far %d\n",lc,sliding,captured_so_far);
/* here we know that we will get one packet - because the polls told us */


/* Receive a single datagram from the server */
{



  }
  
if (captured_so_far==0) {

  if (lc!=2) {
    int f;
reread:
//    if (!url_file) return 0;
    f=wread((void *)bufferbig+lc,1,2-lc);
    bytes_streamed += 1;
//  protocol_log(stderr,"f is %d\n",f);
    if (!f) {
      if (weof()) {
//        url_fclose(url_file);
//        url_file=NULL;
        we_are_streaming_web = -1;
        }
      return(0);
      }
    if (lc <2) {
      lc +=f;
      return(0); 
      }   
    } /* if lcd is not set */


  if (!sliding) {
    captured_so_far=0;
    internal_recvStringLen = bufferbig[0] + (bufferbig[1]<<8);
      protocol_log(stderr,"recvStringLen %d\n",internal_recvStringLen);
    if ((internal_recvStringLen<1)||(internal_recvStringLen>2000)) sliding=1;
    }
  if (sliding) {
    int f=stream_slide_until_sr_indicator((char *)bufferbig,2);
    if (!f) {
      if (weof()) {
//        url_fclose(url_file);
//        url_file=NULL;
        we_are_streaming_web = -1;
        }
      return 0;  
      }
//  protocol_log(stderr,"error %d   total: %ld\n",errno,bytes_streamed); 
    lc=0;
    sliding=0;
    goto reread;
    }
    
    
    
    
    
  the_packet_web_buffer= (char *)packetbuffer;  
  } /* if not partially captured */



int f = wread((void *)the_packet_web_buffer+captured_so_far,1,internal_recvStringLen-captured_so_far);
protocol_log(stderr,"f is %d\n",f);
if (f==0) {
  if (weof()) {
    fprintf(stderr,"error2 %d %ld total\n",errno,bytes_streamed);
//    url_fclose(url_file);
//    url_file=NULL;
    we_are_streaming_web = -1;
    }
  return(0);
  }
captured_so_far +=f;
bytes_streamed += f;

if (captured_so_far>internal_recvStringLen) {
  fprintf( stderr, "oops - too much\n");
//  dump8("encoded frame",the_packet_web_buffer,internal_recvStringLen,2,captured_so_far);  
  }
else if (captured_so_far<internal_recvStringLen) {
  return(0);
  }
/* here we have a match */
captured_so_far=0;

if (internal_recvStringLen>4) {
  decrypt1((unsigned char *)the_packet_web_buffer,internal_recvStringLen-4,internal_recvStringLen);
  *precvStringLen=internal_recvStringLen;
//  dump8("decoded frame",the_packet_web_buffer,internal_recvStringLen,2,internal_recvStringLen);
  }
lc=0;
return(1);
}




int get_packet_from_web_stream() {return 0;}






