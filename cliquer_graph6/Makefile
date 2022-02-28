#!/bin/bash

# Makefile for clique_graph6.
# These programs use nauty and cliquer.

# The environment variable NAUTY_DIR needs the path to nauty.
NAUTYPATH=$(NAUTY_DIR)

# The environment variable CLIQUER_DIR needs the path to cliquer.
CLIQUERPATH=$(CLIQUER_DIR)

CC=gcc
CFLAGS=-O4  -mpopcnt -march=native
L1=-DMAXN=WORDSIZE -DWORDSIZE=64
INCLUDE_DIRS=-I${NAUTYPATH} -I${CLIQUERPATH}

# We need to compile and link the following source and object files from the nauty directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
NAUTYFILES=gtoolsL.o
NAUTYCOMPILEFILES=$(addprefix $(NAUTYPATH)/,$(NAUTYFILES))

# We need to compile and link the following source and object files from the cliquer directory.
# But need to specify the path for each file.  We use the "addprefix" feature of make.
CLIQUERFILES=cliquer.o graph.o reorder.o
CLIQUERCOMPILEFILES=$(addprefix $(CLIQUERPATH)/,$(CLIQUERFILES))

cliquer_graph6: cliquer_graph6.c
	${CC} ${CFLAGS} ${INCLUDE_DIRS} ${L1} -o cliquer_graph6 cliquer_graph6.c $(NAUTYCOMPILEFILES) $(CLIQUERCOMPILEFILES)

clean:
	rm -f cliquer_graph6
