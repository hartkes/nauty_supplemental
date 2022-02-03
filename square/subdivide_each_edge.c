
/* C program to, for each edge e, output the graph with e subdivided once.
 * Graphs are read in from stdin in graph6 format.
 */

#include "gtools.h"
#include "nauty.h"
#include <assert.h>


int main(int argc, char* argv[])
{
    char *infilename,*outfilename;
    FILE *infile,*outfile;
    int codetype;
    
    graph *g;
    int m,n;  /* same parameters for both g and h */
    DYNALLSTAT(graph,h,h_sz);
    
    int i,j;
    set pos_i,pos_j,pos_n;  // sets with one 1, in position i (or j or n, respectively)
    
    
    infilename = NULL;
    infile = opengraphfile(infilename,&codetype,FALSE,1);
        /* infile is stdin, so no need to close later */
    outfilename = "stdout";
    outfile = stdout;
    
    
    while (TRUE)
    {
        if ((g = readg(infile,NULL,0,&m,&n)) == NULL)
            break;
        
        assert(m==1);
        
        DYNALLOC2(graph,h,h_sz,m,n+1,"malloc of h");
        
        /* copy G into H, with the added vertex n being isolated. */
        for (j=n-1; j>=0; j--)
            h[j]=g[j];
        h[n]=(set)0;
        
        pos_n=((set)1)<<((WORDSIZE-1)-n);  /* in nauty, vertex 0 is the HIGH-order bit */
        pos_j=((set)1)<<((WORDSIZE-1)-(n-1));
        for (j=n-1; j>=0; --j,pos_j<<=1)
        {
            pos_i=pos_j<<1;
            for (i=j-1; i>=0; --i,pos_i<<=1)
            {
                if (g[j] & pos_i)  /* i and j are adjacent in G */
                {
                    /* We create the graph H where the i,j edge is subdivided.
                     * The new vertex will be vertex n. */
                    
                    /* remove the edge between i and j in H */
                    h[j]^=pos_i;
                    h[i]^=pos_j;
                    
                    /* add the edge between i and n, and j and n in H */
                    h[j]|=pos_n;
                    h[i]|=pos_n;
                    h[n]|=(pos_i | pos_j);
                    
                    /* output the graph */
                    writeg6(outfile,h,m,n+1);
                    
                    /* restore H as a copy of G, ready for the next edge to be subdivided. */
                    
                    /* add the edge between i and j in H */
                    h[j]|=pos_i;
                    h[i]|=pos_j;
                    
                    /* remove the edge between i and n, and j and n in H */
                    h[j]^=pos_n;
                    h[i]^=pos_n;
                    h[n]=(set)0;
                }
            }
        }
        
        FREES(g);
    }
    
    DYNFREE(h,h_sz);
}
