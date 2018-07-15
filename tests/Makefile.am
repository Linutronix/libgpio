# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2013 Linutronix GmbH
# Author: Manuel Traut <manut@linutronix.de>

check_PROGRAMS = test_gpio_init
TESTS = $(check_PROGRAMS)

test_gpio_init_SOURCES = test_gpio_init.c
test_gpio_init_CFLAGS = -Wall -Wextra -Werror -I$(top_srcdir)/include \
	-DTESTCFG=\"$(top_srcdir)/tests/test.cfg\" @LIBCONFIG_CFLAGS@
test_gpio_init_LDFLAGS = ${top_builddir}/src/libgpio.la @LIBCONFIG_LIBS@

EXTRA_DIST = test.cfg

DISTCLEANFILES = *~

MAINTAINERCLEANFILES = Makefile.in
