AC_INIT([gposttl], [0.9.4], [gmhossain@gmail.com]) 
AM_INIT_AUTOMAKE([ -Wall -Werror foreign])
AC_PROG_CC
AC_PROG_INSTALL

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES(Makefile  data/Makefile  doc/Makefile src/Makefile)

dnl AC_DEFINE_UNQUOTED([DATADIR], ["$datadir"] ,[data])
CPPFLAGS="-DDATADIR=\\\"${datadir}\\\""
AC_OUTPUT


