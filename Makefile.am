SUBDIRS = src include cfg examples
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgpio.pc

DEBIAN_FILES = debian/changelog debian/compat debian/control \
	debian/libgpio-dev.dirs debian/libgpio-dev.install \
	debian/libgpio1.dirs debian/libgpio1.install \
	debian/rules debian/source/format

EXTRA_DIST = libgpio.pc.in $(DEBIAN_FILES)

DISTCLEANFILES = *~

MAINTAINERCLEANFILES = INSTALL Makefile.in aclocal.m4 cfg/Makefile.in \
	compile config.guess config.h.in config.sub configure depcomp \
	install-sh ltmain.sh m4/* missing
