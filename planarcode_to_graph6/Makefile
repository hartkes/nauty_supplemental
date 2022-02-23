#!/bin/bash

# Makefile for compiling a programs for creating and testing squares of graphs.
# These programs use nauty.

# The environment variable NAUTY_DIR needs the path to nauty.
NAUTYPATH=$(NAUTY_DIR)

CC=gcc
CFLAGS=-O4  -mpopcnt -march=native
L1=-DMAXN=WORDSIZE -DWORDSIZE=64
INCLUDE_DIRS=-I${NAUTYPATH}

# We need to compile and link the following source and object files from the nauty directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
NAUTYFILES=gtoolsL.o
NAUTYCOMPILEFILES=$(addprefix $(NAUTYPATH)/,$(NAUTYFILES))

planarcode_to_graph6: planarcode_to_graph6.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o planarcode_to_graph6 planarcode_to_graph6.c $(NAUTYCOMPILEFILES)

clean:
	rm -f planarcode_to_graph6
