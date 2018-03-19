



AC_INIT(src/simple_sr_nosound_curl.c)

AC_CONFIG_MACRO_DIRS([m4])

LT_PREREQ([2.4.6])
LT_INIT([disable-shared])
LTDL_INIT([installable])

PACKAGE=silentradiancesimple
VERSION=0.1.0

AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

AC_PROG_CC


AC_PROG_CXX

AC_OUTPUT(Makefile src/Makefile)


AC_CHECK_LIB(curl,curl_multi_timeout)
AC_CHECK_LIB(asound,snd_pcm_prepare)
AC_CHECK_LIB(opus,opus_decoder_create)
AC_CHECK_LIB(silentradiancesimple,init_processor)
