
/* C program to convert planar code to graph6 format.
 * Graphs are read in from stdin in planar code format.
 */

#include "nauty.h"
#include "gtools.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
    int n,i,j;
    DYNALLSTAT(graph,g,g_sz);
    const char header[]=">>planar_code<<";
    int first_iteration=1;
    
    while (1)
    {
        if ((n=getchar()) == EOF)
            // getchar returns a character, or EOF which is indicated by 257
            // See https://en.wikibooks.org/wiki/C_Programming/stdio.h/getchar for a common mistake.
            break;  // end of file, end program
        
        if (first_iteration)  /* advance past the header */
            for (i=0; i<15; i++)
                if (n==header[i])
                    n=getchar();
                else
                    break;
        
        // n is the number of vertices
        if (n>WORDSIZE)
        {
            printf("n=%d is greater than wordsize, exiting.\n",n);
            exit(99);
        }
        DYNALLOC2(graph,g,g_sz,1,n,"malloc");  /* m=1 */
        EMPTYGRAPH(g,1,n);
        
        for (i=0; i<n; i++)
            while ((j=getchar()) != 0)
                ADDONEARC(g,i,j-1,1);  /* vertices are 1-indexed in planar code */
        
        printf("%s",ntog6(g,1,n));  /* ntog6 adds the \n */
    }
    
    DYNFREE(g,g_sz);
}
