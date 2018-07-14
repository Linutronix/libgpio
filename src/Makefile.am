# SPDX-License-Identifier: LGPL-2.1-or-later
#
# Copyright (C) 2013 Linutronix GmbH
# Author: Manuel Traut <manut@linutronix.de>

lib_LTLIBRARIES = libgpio.la
# 1) If the library source code has changed at all since the last update, then
#    increment revision ("c:r:a" becomes "c:r+1:a").
# 2) If any interfaces have been added, removed, or changed since the last
#    update, increment current, and set revision to 0.
# 3) If any interfaces have been added since the last public release,
#    then increment age.
# 4) If any interfaces have been removed or changed since the last public
#    release, then set age to 0.
libgpio_la_LDFLAGS = -version-info 3:0:0 -lconfig
libgpio_la_SOURCES = gpio.c config.c
libgpio_la_CFLAGS = -Wall -Wextra -Werror -I$(top_srcdir)/include \
	-DCFG_FILE=\"$(sysconfdir)/gpio.cfg\"

DISTCLEANFILES = *~

MAINTAINERCLEANFILES = Makefile.in
