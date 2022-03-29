#!/bin/bash

# Makefile for plantri_md_poly.
# These programs use plantri.

# The environment variable PLANTRI_DIR needs the path to plantri.
PLANTRIPATH=$(PLANTRI_DIR)

CC=gcc
CFLAGS=-O4
INCLUDE_DIRS=-I${PLANTRIPATH} -I.
# include local directory for plugin file

# We need to compile and link the following source and object files from the plantri directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
PLANTRIFILES=plantri.c
PLANTRICOMPILEFILES=$(addprefix $(PLANTRIPATH)/,$(PLANTRIFILES))

plantri_md_poly: maxdeg_poly.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} -o plantri_md_poly '-DPLUGIN="maxdeg_poly.c"' $(PLANTRICOMPILEFILES)

clean:
	rm -f plantri_md_poly
