/* PLUGIN file to use with plantri.c
 * maxdeg_poly.c, based on maxdeg.c
 * 
 * This should be used with the -p switch to generate polytopes.
 * And -c3 -m3 (3-connected graphs).
 * The -D switch gives a maximum degree for the 3-connected triangulations that are generated in phase 1.
 * Then -H switch gives a maximum degree for the polytopes that are generated by removing edges from the triangulation.
 * Combinations of max face size (-f) and maximum number of non-triangular faces incident to a vertex can provide a useful bound on -D so that all possible outputs are generated.
 * For example: generating max deg 5 graphs with maximum face size 5, and at most two 4-faces adjacent to any vertex: -D7 -f4 -H5
 */

#include "cliquer.h"

static int maxdeg = 6;
static int maxdeg_poly = 4;

#define FILTER maxdeg_poly_filter_cliquer
#define PRE_FILTER_POLY square_independence_bounded()
#define PRE_FILTER_SIMPLE maxdeg_prune()

/*******************************************************************/ 

/* The following adds the switch D to those normally present.
   and specifies a subset of the switches as permitted. */

#define PLUGIN_SWITCHES  INTSWITCH('D',maxdeg) INTSWITCH('H',maxdeg_poly)

#undef SWITCHES
#define SWITCHES "[-D# -H# -cmpf -uagsh -odG -v]"
#define HELPMESSAGE \
  fprintf(stderr,"Specify the allowed maximum degree for triangulation with -D#, for polytope with -H#.\n")
#define PLUGIN_INIT \
  if ((minconnec != 3 && minconnec >= 0 \
       || minimumdeg != 3 && minimumdeg >= 0)  && !qswitch) \
  { \
     fprintf(stderr,">E -c is only allowed with -q\n"); \
     exit(1); \
  }

/*********************************************************************/

static int
square_independence_bounded()
{
	/* We now check whether the square has independence number at most 2, using cliquer. */
	
	/* create cliquer graph from plantri graph */
    graph_t *clg=graph_new(nv);  /* graph is empty */
    for (int v=nv-1; v>=0; v--)
    {
		EDGE *e,*elast;

		e = elast = firstedge[v];
		do
		{
			GRAPH_ADD_EDGE(clg,v,e->end);  /* also adds edge between e->end and v */
			e = e->next;
		} while (e != elast);
    }
    
    /*
    if (!graph_test(clg,NULL))
	{
		printf("clg is not good!\n");
		exit(5);
	}
	*/
    
    /* form square */
	graph_t *clh=graph_new(nv);
	for (int v=nv-1; v>=0; v--)
	{
		setelement neighbors=clg->edges[v][0];
		setelement second_neighborhood=0;
		while (neighbors)  /* neighbors of v remaining */
		{
			second_neighborhood|=clg->edges[
					__builtin_ctzll(neighbors)  // count trailing zeroes
				][0];
			neighbors&=(neighbors-1);  // clear the bottom bit
		}
		clh->edges[v][0]=(clg->edges[v][0] | second_neighborhood)  /* neighbors of v in the square are the union of the nbrs of v in G and the second neighborhood */
		                 ^ ((setelement)1<<v);  // but clear the bit so no loop
	}
	
	/*
    if (!graph_test(clh,NULL))
	{
		printf("clh is not good after forming square!\n");
		exit(5);
	}
	*/
	
	/* form complement */
	setelement complement=(((setelement)1)<<(nv-1))-1;
		/* complement has the bottom n-1 1s set [in positions 0..n-2], and 0s otherwise */
	for (int v=nv-1; v>=0; v--)
	{
		clh->edges[v][0]^=complement;
		complement>>=1;
		complement|=((setelement)1)<<(nv-1);  // replace the high bit
	}
    
    /*
    if (!graph_test(clh,NULL))
	{
		printf("clh is not good after complement!\n");
		exit(5);
	}
	*/
	
    /* call cliquer */
    clique_default_options->time_function=NULL;
    
    set_t max_indep=clique_unweighted_find_single(clh,
                        3,3,
						FALSE,NULL);
    if (max_indep==NULL)  /* clg has clique number <=2 */
    {
		graph_free(clg);
		graph_free(clh);
		return TRUE;  /* keep this graph */
    }
    else  /* the square of g has an independent set that is bigger than target_upper_bound */
    {
        set_free(max_indep);
        graph_free(clg);
		graph_free(clh);
        return FALSE;  /* prune this graph */
    }
}


static int
maxdeg_poly_filter_cliquer(int nbtot, int nbop, int doflip)
{
	register int i;

	for (i = 0; i < nv; ++i)
	   if (degree[i] > maxdeg_poly) return FALSE;
	
	/* max deg of the polytope is <= maxdeg_poly */
	
	/* We now check whether the square has independence number at most 2. */
	return square_independence_bounded();
}


/*********************************************************************
The following is used to prune the search tree at levels below maxnv.
Consider the expansion operations E3, E4, E5.  The basic ideas are:
(1) Only E5 can reduce the degree of a vertex, then only by 1.
(2) If there are 3 or more vertices of degree 3, E5 will never be used.
(3) Similarly if there are 2 vertices of degree 3 but they don't have
	two common neighbours.
(4) The quantity (2 * #degree-3 + #degree-4) is reduced by at most one
    by E3 and E4.  E5 can reduce it by 2 as long as it becomes 0. */

static int
commonedge(int a, int b)
/* Test that vertices a,b of degree 3 are at the opposite
   points of two adjacent faces */
{
	EDGE *e;

	e = firstedge[a];
	if (e->invers->next->next->end == b) return TRUE;
	e = e->next;
	if (e->invers->next->next->end == b) return TRUE;
	e = e->next;
	if (e->invers->next->next->end == b) return TRUE;
	return FALSE;
}

static int
maxdeg_prune()
{
	register int i,levs,excess,d3,d4;
	int d3a,d3b;

	levs = maxnv - nv;    /* Number of expansions yet to perform */
	excess = d3 = d4 = 0;

	for (i = 0; i < nv; ++i)
	{
	    if (degree[i] == 3)
	    {
		++d3;
		d3a = d3b;
		d3b = i;
	    }
	    else if (degree[i] == 4)
		++d4;
	    else if (degree[i] > maxdeg)
		excess += degree[i] - maxdeg;
	}

	if (excess == 0) return square_independence_bounded();  /*TRUE;*/

	if (d3 > 2) return FALSE;
	if (d3 == 2 && !commonedge(d3a,d3b)) return FALSE;
	if (d3 > 0 && excess >= levs) return FALSE;
	
	i = d3 + d3 + d4;
	if (i > 0 && excess > levs - i + 2) return FALSE;

	return square_independence_bounded();  /*TRUE;*/
}
