bin_PROGRAMS = testpin
testpin_SOURCES = test.c
testpin_CFLAGS = -I${top_srcdir}/include -I${top_srcdir}/src \
	@LIBCONFIG_CFLAGS@
testpin_LDFLAGS = -L${top_builddir}/src/.libs -lgpio \
	@LIBCONFIG_LIBS@
