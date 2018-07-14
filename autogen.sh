#!/bin/bash
# SPDX-License-Identifier: LGPL-2.1-or-later
#
# Copyright (C) 2013 Linutronix GmbH
# Author: Manuel Traut <manut@linutronix.de>

mkdir -p m4 && \
    touch ChangeLog && \
    autoreconf -sif
