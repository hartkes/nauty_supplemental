
/* C program to test if the square of a graph is complete.
 * graphs are read in from stdin in graph6 format.
 */

#include "gtools.h"

int main(int argc, char* argv[])
{
    char *infilename,*outfilename;
    FILE *infile,*outfile;
    int codetype;
    
    graph *g;
    int m,n;
    
    int i,j;
    set *gi,*gj;
    
    
    infilename = NULL;
    infile = opengraphfile(infilename,&codetype,FALSE,1);
    outfilename = "stdout";
    outfile = stdout;
    
    
    while (TRUE)
    {
        if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
        
        for (j=n-1; j>=0; j--)
        {
            gj=GRAPHROW(g,j,m);
            for (i=j-1; i>=0; i--)
            {
                if (
                    (ISELEMENT(gj,i)==0)  /* i and j are not adjacent */
                    &&
                    (((*gj) & (*GRAPHROW(g,i,m)))==0)  /* no common neighbor between i and j */
                    )
                    goto cont;  /* continue while loop */
            }
        }
        /* if we reach the end of the for loops, then every pair of vertices is within distance 2.
         * Hence, the square is complete, and we output the graph.
         */
        writelast(outfile);
        
cont:
        FREES(g);
        continue;
    }
    
}
