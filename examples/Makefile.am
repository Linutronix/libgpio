# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2013 Linutronix GmbH
# Author: Manuel Traut <manut@linutronix.de>

bin_PROGRAMS = testpin
testpin_SOURCES = testpin.c
testpin_CFLAGS = -Wall -Wextra -Werror \
	-I${top_srcdir}/include @LIBCONFIG_CFLAGS@
testpin_LDFLAGS = ${top_builddir}/src/libgpio.la @LIBCONFIG_LIBS@

dist_man_MANS = testpin.1

DISTCLEANFILES = *~

MAINTAINERCLEANFILES = Makefile.in
