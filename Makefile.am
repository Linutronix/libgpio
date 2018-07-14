SUBDIRS = src include cfg examples
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgpio.pc

EXTRA_DIST = libgpio.pc.in libgpio.doxy.in tools/ChangeLog-from-git

dist-hook:
	$(top_srcdir)/tools/ChangeLog-from-git $(top_srcdir)

html-local:
	doxygen libgpio.doxy

mostlyclean-local:
	-rm -rf doxydoc/

DISTCLEANFILES = *~

MAINTAINERCLEANFILES = ChangeLog INSTALL Makefile.in aclocal.m4 \
	cfg/Makefile.in compile config.guess config.h.in config.sub \
	configure depcomp install-sh ltmain.sh m4/* missing
