#!/bin/bash
mkdir -p m4 && \
    touch ChangeLog && \
    autoreconf -sif
