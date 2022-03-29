#!/bin/bash

# Makefile for compiling a version of geng from nauty that only outputs planar graphs.
# This is done using geng's PRUNE functionality.
# The prune function uses the planarity.c program in nauty (used by planarg).
# and cliquer.

# The environment variable NAUTY_DIR needs the path to nauty.
NAUTYPATH=$(NAUTY_DIR)

# The environment variable CLIQUER_DIR needs the path to cliquer.
CLIQUERPATH=$(CLIQUER_DIR)

CC=gcc
CFLAGS=-O4  -mpopcnt -march=native -std=c99
L1=-DMAXN=WORDSIZE -DWORDSIZE=64
INCLUDE_DIRS=-I${NAUTYPATH} -I${CLIQUERPATH}

# We need to compile and link the following source and object files from the nauty directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
NAUTYFILES=geng.c gtoolsL.o nausparseL.o nautyL1.o nautilL1.o naugraphL1.o schreier.o naurng.o planarity.c
NAUTYCOMPILEFILES=$(addprefix $(NAUTYPATH)/,$(NAUTYFILES))

# We need to compile and link the following source and object files from the cliquer directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
CLIQUERFILES=cliquer.o graph.o reorder.o
CLIQUERCOMPILEFILES=$(addprefix $(CLIQUERPATH)/,$(CLIQUERFILES))

gengL_planar_cliquer_complement: nonplanar_cliquer_prune.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o gengL_planar_cliquer_complement -DPRUNE=nonplanar_cliquer_prune nonplanar_cliquer_prune.c $(NAUTYCOMPILEFILES) $(CLIQUERCOMPILEFILES)

clean:
	rm -f gengL_planar_cliquer_complement
