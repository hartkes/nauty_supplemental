#!/bin/bash

# Makefile for compiling a version of geng from nauty that only outputs planar graphs.
# This is done using geng's PRUNE functionality.
# The prune function uses the planarity.c program in nauty (used by planarg).

# The environment variable NAUTY_DIR needs the path to nauty.
NAUTYPATH=$(NAUTY_DIR)

CC=gcc
CFLAGS=-O4  -mpopcnt -march=native
L1=-DMAXN=WORDSIZE -DWORDSIZE=64
INCLUDE_DIRS=-I${NAUTYPATH}

# We need to compile and link the following source and object files from the nauty directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
NAUTYFILES=geng.c gtoolsL.o nausparseL.o nautyL1.o nautilL1.o naugraphL1.o schreier.o naurng.o planarity.c
NAUTYCOMPILEFILES=$(addprefix $(NAUTYPATH)/,$(NAUTYFILES))

gengL_planar: nonplanar_prune.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o gengL_planar -DPRUNE=nonplanar_prune nonplanar_prune.c $(NAUTYCOMPILEFILES)

clean:
	rm -f gengL_planar
