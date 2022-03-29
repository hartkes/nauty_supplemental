/* PLUGIN file to use with plantri.c
 * maxdeg_poly.c, based on maxdeg.c
 */

static int maxdeg = 6;
static int maxdeg_poly = 4;

#define FILTER maxdeg_poly_filter
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
maxdeg_poly_filter(int nbtot, int nbop, int doflip)
{
	register int i;

	for (i = 0; i < nv; ++i)
	   if (degree[i] > maxdeg_poly) return FALSE;
	
	return TRUE;
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

	if (excess == 0) return TRUE;

	if (d3 > 2) return FALSE;
	if (d3 == 2 && !commonedge(d3a,d3b)) return FALSE;
	if (d3 > 0 && excess >= levs) return FALSE;
	
	i = d3 + d3 + d4;
	if (i > 0 && excess > levs - i + 2) return FALSE;

	return TRUE;
}
