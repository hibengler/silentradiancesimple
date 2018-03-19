pkglib_LTLIBRARIES = libsilentradiancesimple.la


libsilentradiancesimple_la_SOURCES = simple_code_data1.h simple_curl_web_stream.c simple_curl_web_stream.h simple_decrypt.c simple_decrypt.h simple_processor.c simple_processor.h simple_seven_eight_decode.c simple_seven_eight_decode.h simple_sound_alsa.c simple_sound_alsa.h

libsilentradiancesimple_la_LDFLAGS = -version-info 1:0:1
include_HEADERS = silentradiancesimple.h

# later: simple_sound_sdl2.c simple_sound_sdl2.h

#lib_LTLIBRARIES = libm.la
#if WANT_LIBCURL
#lib_LTLIBRARIES += libcurl.la
#endif
#if WANT_LIBASOUND
#lib_LTLIBRARIES += libasound.la
#endif
#if WANT_LIBSDL2
#lib_LTLIBRARIES += libsdl2.la
#endif

#EXTRA_LTLIBRARIES = libcurl.la libm.la libasound.la
#https://www.gnu.org/software/automake/manual/html_node/Conditional-Libtool-Libraries.html#Conditional-Libtool-Libraries
#EXTRA_libhello_la_SOURCES = hello-linux.c hello-generic.c
#libhello_la_LIBADD = $(HELLO_SYSTEM)
#libhello_la_DEPENDENCIES = $(HELLO_SYSTEM)

AM_LDFLAGS = -Wl,-rpath -Wl,LIBDIR

bin_PROGRAMS = simple_sr_alsa_curl simple_sr_nosound_curl 


AM_CXXFLAGS = 

simple_sr_nosound_curl_SOURCES = simple_sr_nosound_curl.c
simple_sr_nosound_curl_LDADD =   -lsilentradiancesimple -lcurl -lasound -lopus  -lm  
simple_sr_nosound_curl_DEPENDENCIES = libsilentradiancesimple.la

simple_sr_alsa_curl_SOURCES = simple_sr_alsa_curl.c
simple_sr_alsa_curl_LDADD =  -lsilentradiancesimple -lcurl -lasound -lopus -lm 
simple_sr_alsa_curl_DEPENDENCIES = libsilentradiancesimple.la


