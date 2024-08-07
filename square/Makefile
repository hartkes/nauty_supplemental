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

all: squareg injective_g exact_distance2_g is_square_complete subdivide_each_edge

squareg: squareg.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o squareg squareg.c $(NAUTYCOMPILEFILES)

injective_g: injective_g.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o injective_g injective_g.c $(NAUTYCOMPILEFILES)

exact_distance2_g: exact_distance2_g.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o exact_distance2_g exact_distance2_g.c $(NAUTYCOMPILEFILES)

is_square_complete: is_square_complete.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o is_square_complete is_square_complete.c $(NAUTYCOMPILEFILES)

subdivide_each_edge: subdivide_each_edge.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o subdivide_each_edge subdivide_each_edge.c $(NAUTYCOMPILEFILES)

clean:
	rm -f squareg injective_g exact_distance2_g is_square_complete subdivide_each_edge
