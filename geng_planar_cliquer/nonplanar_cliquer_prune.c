/* nonplanar_cliquer_prune.c
 * PRUNE filter for geng to remove nonplanar graphs.
 * compile with:
 * cc -o geng_planar -O3 -DPRUNE=nonplanar_prune geng.c geng.c gtools.o nauty.o nautil.o naugraph.o schreier.o naurng.o planarity.c nonplanar_cliquer_prune.c
 */

//#include "gtools.h"
#include "nausparse.h"
#include "planarity.h"
#include "cliquer.h"
//#include <stdio.h>



boolean square_independence_bounded(graph *g,int n,int maxn)
{
    /* Construct the graph for cliquer. */
    /* We want the independence number of the square, so we form
     * the complement of the square and compute the clique number.
     */
    graph_t *clg=graph_new(n);
    for (int j=n-1; j>=0; j--)
    {
        set *gj=GRAPHROW(g,j,1);  /* m=1 */
        for (int i=j-1; i>=0; i--)
            if (
               !(
                ISELEMENT(gj,i)  /* i and j are adjacent in G */
                ||
                ((*gj) & (*GRAPHROW(g,i,1)))  /* i and j have a common neighbor in G */
                )
               )
                GRAPH_ADD_EDGE(clg,i,j);  /* also adds edge between j and i */
    }
    
    /* set target upper bound */
    const int geng_maxdeg=5;  /* should be global variable from geng.c */
    int target_upper_bound;
    int final_bound;  /* for graph of size maxn, the square independence number */
    int square_chi_bound;
    if (geng_maxdeg==3)  /* global variable from geng.c */
    {
       square_chi_bound=7;
       final_bound=(maxn-1)/(square_chi_bound-1);
       target_upper_bound=
                    maxn-n<=1
                    ? final_bound+maxn-n
                    : (n+3)/4;  // round up
                    //((maxn-n <=0) ? 0 : 1);
    }
    else if (geng_maxdeg==4)  /* global variable from geng.c */
    {
       square_chi_bound=geng_maxdeg+5;
       final_bound=(maxn-1)/(square_chi_bound-1);
       target_upper_bound=
                    maxn-n<=1
                    ? final_bound+maxn-n
                    : (n+4)/5;  // round up
                    //final_bound + (maxn-n);
    }
    else if (geng_maxdeg==5)  /* global variable from geng.c */
    {
       square_chi_bound=geng_maxdeg+5;
       final_bound=(maxn-1)/(square_chi_bound-1);
       target_upper_bound=
                    maxn-n<=1
                    ? final_bound+maxn-n
                    : (n+5)/6;  // round up
    }
   //printf("D=%d n=%d max=%d final_bound=%d target_upper_bound=%d square_chi_bound=%d\n",geng_maxdeg,n,maxn,final_bound,target_upper_bound,square_chi_bound);
    
    /* call cliquer */
    clique_default_options->time_function=NULL;
    
    set_t max_indep=clique_unweighted_find_single(clg,
                        target_upper_bound+1,target_upper_bound+1,FALSE,NULL);
    if (max_indep==NULL)  /* clg has clique number <=target_upper_bound */
    {
        //return 0;  // looking for counterexample

        /* We check whether the graph has square clique number equal to square_chi_bound */
        /* and reject if so. */
        
        /* we first take the complement of the graph in clg */
        setelement complement=(((setelement)1)<<(n-1))-1;
            /* complement has the bottom n-1 1s set [in positions 0..n-2], and 0s otherwise */
        for (int i=n-1; i>=0; i--)
        {
            clg->edges[i][0]^=complement;
            complement>>=1;
            complement|=((setelement)1)<<(n-1);  // replace the high bit
        }
        
        //printf("clg is good? %d\n",graph_test(clg,NULL));
        
        //printf("square indep num is <= target_upper_bound, testing square clique number\n");
        
        set_t max_clique=clique_unweighted_find_single(clg,
                            square_chi_bound,square_chi_bound,FALSE,NULL);
        //printf("cliquer finished\n");
        if (max_clique!=NULL)  /* clg has clique number >=square_chi_bound */
        {
            set_free(max_clique);
            graph_free(clg);
            return 1;  /* prune this graph */
        }
        else
        {
            //printf("keeping D=%d n=%d max=%d final_bound=%d target_upper_bound=%d square_chi_bound=%d\n",geng_maxdeg,n,maxn,final_bound,target_upper_bound,square_chi_bound);
            graph_free(clg);
            return 0;  /* keep this graph */
        }
    }
    else  /* the square of g has an independent set that is bigger than target_upper_bound */
    {
        set_free(max_indep);
        graph_free(clg);
        return 1;  /* prune this graph */
    }
}



/* taken directly from planarg.c */
/*************************************************************************/

static boolean
isplanar(t_ver_sparse_rep *V, int n, t_adjl_sparse_rep *A, int e,
         int *c, t_ver_sparse_rep **VR, t_adjl_sparse_rep **AR,
         t_embed_sparse_rep  **ER, int *nbr_e_obs,
         boolean planarcheck, boolean nonplanarcheck)
/*
  The input graph is given as an adjacency list:
      V: array of vertices 
      n: size of graph
      A: adjacency list
      e: number of edges
      
  If the graph is planar the embedding is stored  in VR and ER;
  the embedding contains e edges (nbr_e_obs not used)
      
  If the graph is non planar the obstruction is returned in
  VR and AR together with the number of edges in nbr_e_obs.

  In all cases the number of components is return in c.

  planarcheck and nonplanarcheck determine if checking is done.
  The embedding and obstruction outputs are only made if the
  appropriate checking is done.
*/
{
    t_dlcl           **dfs_tree, **back_edges, **mult_edges;
    int              edge_pos, v, w;
    boolean          ans;
    t_ver_edge       *embed_graph;
 
    ans = sparseg_adjl_is_planar(V, n, A, c,
                                 &dfs_tree, &back_edges, &mult_edges,
                                 &embed_graph, &edge_pos, &v, &w);
/*    
    if (!ans && nonplanarcheck)
    {
        embedg_obstruction(V, A, dfs_tree, back_edges,
                           embed_graph, n, &edge_pos,
                           v, w, VR, AR, nbr_e_obs);
    }
    else if (planarcheck)
    {
        embedg_embedding(V, A, embed_graph, n, e, *c, edge_pos, mult_edges,
                         VR, ER);
    }
*/
    
    sparseg_dlcl_delete(dfs_tree, n);
    sparseg_dlcl_delete(back_edges, n);
    sparseg_dlcl_delete(mult_edges, n);
    embedg_VES_delete(embed_graph, n);
 
    return ans;
}



/*************************************************************************/


int nonplanar_cliquer_prune(graph *g,int n,int maxn)
{
/* taken directly from planarg.c */
    char *infilename,*outfilename;
    FILE *infile,*outfile;
    sparsegraph sg;
    boolean badargs;
    boolean verbose,nonplanar,quiet;
    boolean planarcode,nowrite,nocheck;
    int i,j,k,/*n,*/argnum,ne,loops;
    int codetype,outcode;
    t_ver_sparse_rep *VR;
    t_adjl_sparse_rep *AR;
    t_embed_sparse_rep *ER;
    int nbr_c,nbr_e_obs;
    nauty_counter nin,nout,nplan;
    char *arg,sw;
    double t0,tm0,tp,tnp,netotalp,netotalnp;
    DYNALLSTAT(t_ver_sparse_rep,V,V_sz);
    DYNALLSTAT(t_adjl_sparse_rep,A,A_sz);
    
    setword h[128];  /* h is of type (graph *), with m=1 and n<=128 */
    
    /* complement g into h */
    setword complement=~((~(setword)0)>>(n-1));  /* top n-1 bits set */
    for (i=n-1; i>=0; i--)
    {
        h[i]=g[i] ^ complement;
        complement<<=1;
        complement|=((setword)1)<<(WORDSIZE-n);  /* replace bit for vertex n-1 */
    }
    
    //printf("starting PRUNE n=%d maxn=%d\n",n,maxn);
    SG_INIT(sg);
    {
        nauty_to_sg((graph *)h,&sg,1,n);  /* m=1 for graph g passed to prune function */
        ne = (sg.nde+loops)/2;
        ++nin;

        DYNALLOC1(t_ver_sparse_rep,V,V_sz,n,"planarg");
        DYNALLOC1(t_adjl_sparse_rep,A,A_sz,2*ne+1,"planarg");
        k = 0;
        for (i = 0; i < n; ++i)
            if (sg.d[i] == 0)
                V[i].first_edge = NIL;
            else
            {
                V[i].first_edge = k;
                for (j = sg.v[i]; j < sg.v[i]+sg.d[i]; ++j)
                {
                    A[k].end_vertex = sg.e[j];
                    A[k].next = k+1;
                    ++k;
                    if (A[k-1].end_vertex == i)  /* loops go in twice */
                    {
                        A[k].end_vertex = i;
                        A[k].next = k+1;
                        k++;
                    }
                }
                A[k-1].next = NIL;
            }
        
        if (k != 2*ne)
        {
            fprintf(stderr,
               ">E planarg: decoding error; nin=" COUNTER_FMT "\n",nin);
            fprintf(stderr,"n=%d nde=%lu ne=%d loops=%d\n",
                    n,(unsigned long)sg.nde,ne,loops);
            exit(1);
        }
        
        SG_FREE(sg);  // added; would be nice not to convert to sparsegraph
        
        // Do V and A need to be deallocated?  I think no, because they are static?

        VR = NULL;
        AR = NULL;
        ER = NULL;
        if (isplanar(V,n,A,ne,&nbr_c,&VR,&AR,&ER,&nbr_e_obs,
                     0,0))
        {
            return square_independence_bounded(h,n,maxn);
        }
        else
        {
            return 1;  /* nonplanar, so prune this graph */
        }
        
        FREES(VR);
        FREES(AR);
        FREES(ER);
    }
}

