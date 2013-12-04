#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([libgpio], [3.0.0], [manut@linutronix.de])
AM_INIT_AUTOMAKE([silent-rules color-tests])
AC_USE_SYSTEM_EXTENSIONS
AC_GNU_SOURCE
LT_PREREQ([2.2])
LT_INIT([dlopen])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/gpio.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O

# Checks for libraries.
PKG_CHECK_MODULES([LIBCONFIG], [libconfig >= 1.4],,
  AC_MSG_ERROR([libconfig 1.4 or newer not found.])
)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdio.h stdint.h stdlib.h string.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_TYPE_SSIZE_T

AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([memset munmap])

AC_CONFIG_FILES([Makefile
                 libgpio.pc
                 include/Makefile
                 cfg/Makefile
                 src/Makefile
                 examples/Makefile])
AC_OUTPUT
