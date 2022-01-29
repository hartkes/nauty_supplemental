
/* C program to convert one graph6 string from stdin to DIMACS format on stdout.
 * https://users.cecs.anu.edu.au/~bdm/data/formats.html
 * http://dimacs.rutgers.edu/archive/pub/challenge/graph/doc/ccformat.tex
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char *line;  /* line read from stdin */
    size_t len;
    ssize_t lineSize;
    
    char *cur;  /* the current character in the string that we're considering */
    char *beginning_adj_matrix;  /* the beginning of the adjacency matrix in line */
    int n,num_edges;  /* n: number of vertices of graph, and num_edges: number of edges */
    int val,mask;  /* for decoding the graph edges */
    int i,j;
    
    lineSize=getline(&line,&len,stdin);
    
    cur=line;
    if (*cur==126)  /* the graph has at least 63 vertices */
    {
        cur++;  /* advance past the first 126 character */
        if (*cur==126)  /* the graph has at least 258048 vertices */
        {
            cur++;  /* advance past the second 126 character */
            n=*cur-63;
            for (i=1; i<6; i++)  // 6 6-bit words for n
            {
                cur++;
                n<<=6;
                n+=*cur-63;
            }
        }
        else  /* the graph has between 63 and 258047 vertices */
        {
            n=*cur-63;
            for (i=1; i<3; i++)  /* 3 6-bit words for n */
            {
                cur++;
                n<<=6;
                n+=*cur-63;
            }
        }
    }
    else  /* the graph has at most 62 vertices */
        n=*cur-63;
    cur++;  /* move past the information on the number of vertices */
    beginning_adj_matrix=cur;  /* save the beginning of the adjacency matrix */
    
    /* compute the number of edges by reading in the adjacency matrix */
    num_edges=0;
    val=*cur-63;
    mask=1<<5;  /* start with the high bit */
    for (j=0; j<n; j++)  /* adj matrix is bit packed in colex order in graph6 format */
        for (i=0; i<j; i++)
        {
            if (val&mask)  /* i and j are adjacent */
                num_edges++;
            
            mask>>=1;
            if (!mask)  // mask has become 0
            {
                cur++;
                val=*cur-63;
                mask=1<<5;
            }
        }
    
    printf("c g6\n");  /* could include graph6 string, but let's keep it minimal */
    printf("p edge %d %d\n",n,num_edges);
    
    /* print the edges by reading in the adjacency matrix */
    cur=beginning_adj_matrix;  /* reset cur to the beginning of the adjacency matrix */
    val=*cur-63;
    mask=1<<5;  /* start with the high bit */
    for (j=0; j<n; j++)  /* adj matrix is bit packed in colex order in graph6 format */
        for (i=0; i<j; i++)
        {
            if (val&mask)  /* i and j are adjacent */
                printf("e %d %d\n",i+1,j+1);  /* REMEMBER: DIMACS format is 1-indexed! */
            
            mask>>=1;
            if (!mask)  // mask has become 0
            {
                cur++;
                val=*cur-63;
                mask=1<<5;
            }
        }
    
    free(line);
}
