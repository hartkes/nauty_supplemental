
/* C program to call cliquer.
 * graphs are read in from stdin in graph6 format.
 */

#include "gtools.h"
#include "nauty.h"
#include "cliquer.h"


inline
uint64_t bit_reversal(uint64_t x)
{
    x = __builtin_bswap64(x);  /* reverse order of bytes ("byte swap") */
    x = ((x & 0xaaaaaaaaaaaaaaaa) >> 1) | ((x & 0x5555555555555555) << 1);
    x = ((x & 0xcccccccccccccccc) >> 2) | ((x & 0x3333333333333333) << 2);
    x = ((x & 0xf0f0f0f0f0f0f0f0) >> 4) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
    return x;
}


int main(int argc, char* argv[])
{
    if (argc<2)
    {
        printf("SYNTAX: clique_graph6 <lower_bound>\n");
        printf("Reads graph6 strings in on stdin, prints out the graph6 strings of graphs with clique number >=lower_bound\n");
        exit(99);
    }
    
    int lower_bound;
    sscanf(argv[1],"%d",&lower_bound);
    
    char *infilename,*outfilename;
    FILE *infile,*outfile;
    int codetype;
    
    graph *g;  /* for nauty */
    int m,n;
    
    graph_t *clg;  /* for cliquer */
    
    
    int i,j;
    set *gi,*gj;
    
    
    infilename = NULL;
    infile = opengraphfile(infilename,&codetype,FALSE,1);
        /* infile is stdin, so no need to close later */
    outfilename = "stdout";
    outfile = stdout;
    
    while (TRUE)
    {
        if ((g = readg(infile,NULL,0,&m,&n)) == NULL)  // dynamic allocation of g
            break;
        
        /* We need to bit reverse the adjacency matrix of g to form clg.
         * Since nauty puts vertex 0 in the most significant bit position,
         * and cliquer puts vertex 0 in the least significant bit position.
         */
        
        clg=graph_new(n);
        
        for (int i=n-1; i>=0; i--)
            /* we assume that m=1 */
            clg->edges[i][0]=bit_reversal(g[i]);
        
        clique_default_options->time_function=NULL;
        
        set_t max_clique=clique_unweighted_find_single(clg,
                            lower_bound,0,FALSE,NULL);
        if (max_clique==NULL)  /* clg has clique number <lower_upper_bound */
            ;
        else
        {
            writeg6(outfile,g,m,n);
            //printf("has a clique of size at least %d\n",lower_bound);
            set_free(max_clique);
        }
        
        graph_free(clg);
        FREES(g);
    }
}
