
/* C program to test if the square of a graph is complete.
 * graphs are read in from stdin in graph6 format.
 */

#include "gtools.h"
#include "nauty.h"


int main(int argc, char* argv[])
{
    char *infilename,*outfilename;
    FILE *infile,*outfile;
    int codetype;
    
    graph *g;
    int m,n;  /* same parameters for both g and h */
    DYNALLSTAT(graph,h,h_sz);
    
    int i,j;
    set *gi,*gj;
    
    
    infilename = NULL;
    infile = opengraphfile(infilename,&codetype,FALSE,1);
        /* infile is stdin, so no need to close later */
    outfilename = "stdout";
    outfile = stdout;
    
    
    while (TRUE)
    {
        if ((g = readg(infile,NULL,0,&m,&n)) == NULL)
            break;
        
        DYNALLOC2(graph,h,h_sz,m,n,"malloc of h");
        EMPTYGRAPH(h,m,n);
        
        for (j=n-1; j>=0; j--)
        {
            gj=GRAPHROW(g,j,m);
            
            for (i=j-1; i>=0; i--)
            {
                if (i==j)
                    continue;
                if (
                    ISELEMENT(gj,i)  /* i and j are adjacent in G */
                    ||
                    ((*gj) & (*GRAPHROW(g,i,m)))  /* i and j have a common neighbor in G */
                    )
                    ADDONEEDGE(h,i,j,m);  /* also adds to H edge between j and i */
            }
        }
        writeg6(outfile,h,m,n);
        
        FREES(g);
    }
    
    DYNFREE(h,h_sz);
}
