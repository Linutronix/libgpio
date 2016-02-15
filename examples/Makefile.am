bin_PROGRAMS = testpin testirq
testpin_SOURCES = test.c
testpin_CFLAGS = -I${top_srcdir}/include -I${top_srcdir}/src \
	@LIBCONFIG_CFLAGS@
testpin_LDFLAGS = -L${top_builddir}/src/.libs -lgpio -lpthread \
	@LIBCONFIG_LIBS@
	
testirq_SOURCES = testirq.c
testirq_CFLAGS = -I${top_srcdir}/include -I${top_srcdir}/src \
	@LIBCONFIG_CFLAGS@
testirq_LDFLAGS = -L${top_builddir}/src/.libs -lgpio -lpthread \
	@LIBCONFIG_LIBS@
