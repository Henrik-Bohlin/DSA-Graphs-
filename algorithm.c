#include "algorithm.h"
// queue - defines a queue as a pointer to an array of pointers to nodes
typedef pnode *queue;
// local prototypes
void init_single_source(pnode G, char s);
void relax(pnode u, pnode v, int w);
int Q_insert(queue Q, pnode u, int index);
int Q_is_empty(queue Q, int size);
pnode Q_extract_min(queue Q, int size);
bool Q_exists(queue Q, int qsize, char name);
int name_to_pos(pnode G, char c);
void list_to_matrix(pnode G, double matrix[MAXNODES][MAXNODES]);
//--------------------------------------------------------------------------
// init single source, relax and min are to be used by algorithms
//--------------------------------------------------------------------------
void init_single_source(pnode G, char s)
{
	for (pnode node = G; !is_empty(node); node = get_next(node)) {
		if (get_name(node) == s) // start node
			set_d(node, 0);
		else
			set_d(node, INFINITY);
		set_pi(node, '-');
	}
}
void relax(pnode u, pnode v, int w)
{
	if (get_d(v) > (get_d(u) + w)) {
		set_d(v, get_d(u) + w);
		set_pi(v, get_name(u));
	}
}
double min(double a, double b)
{
	return a < b ? a : b;
}
//--------------------------------------------------------------------------
// Q functions - used by other algorithms
//--------------------------------------------------------------------------
int Q_insert(queue Q, pnode u, int index)
{
	Q[index]=u;
	return index + 1;
}
int Q_is_empty(queue Q, int size)
{
	return size == 0 ;
}
pnode Q_extract_min(queue Q, int size)
{
	double MIN = INFINITY;
	pnode best = NULL;
	int MIN_index = -1;
	for (int i=0;i<size;i++) {
		if (Q[i]->d < MIN) {
			MIN = Q[i]->d;
			best = Q[i];
			MIN_index = i;
		}
	}
	if (MIN_index != -1) {
		for (int i= MIN_index; i<size-1; i++) {
			Q[i] = Q[i+1];
		}
		size--;
	}
	return best;
}
bool Q_exists(queue Q, int qsize, char name)
{
	for (int i=0; i < qsize; i++) {
		if (name == Q[i]->name) {
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------
// Dijkstra's algorithm, single source shortest path tree generator
// a -> b(1) -> c(5)
// b -> c(2)           --> d: [  0,   1,   3]
// c                       e: [  -,   a,   b]
//--------------------------------------------------------------------------
void dijkstra(pnode G, char s, double *d, char *e)
{
	init_single_source(G, s);           
    queue Q = (queue)malloc(MAXNODES * sizeof(pnode));  
    int size = 0;  

    // Fyll kön med alla noder
    for (pnode node = G; !is_empty(node); node = get_next(node)) {
        size = Q_insert(Q, node, size);  
    }

    // Huvudloop för Dijkstra
    while (!Q_is_empty(Q, size)) {
        pnode u = Q_extract_min(Q, size);  
        size--;  // Storleken minskas efter nodutdragning
        pedge edge = get_edges(u);
        
        while (edge != NULL) {
            pnode v = get_node(G, edge->to);
            double old_d = get_d(v);
            relax(u, v, edge->weight);
            
            // Om uppdatering skett, lägg tillbaka noden i kön
            if (get_d(v) < old_d) {
                size = Q_insert(Q, v, size);
            }
            edge = edge->next_edge;
        }

        // Spara avstånd och föregångare
        int pos = name_to_pos(G, get_name(u));
        d[pos] = get_d(u);
        e[pos] = get_pi(u);
    }
    free(Q);
}
//--------------------------------------------------------------------------
// Prim's algorithm - Minimum Spanning Tree generator
// start_node: a
// a -> b(1) -> c(5)
// b -> c(2)           --> d: [inf,   1,   2]
// c                       e: [  -,   a,   b]
//--------------------------------------------------------------------------
void prim(pnode G, char start_node, double *d, char *e)
{
	init_single_source(G, start_node);

	//int qsize = node_cardinality(G);

	// Skapa en prioritetskö
    queue Q = (queue)malloc(MAXNODES * sizeof(pnode));
    int size = 0;
    
    // Fyll kön med alla noder
    for (pnode node = G; !is_empty(node); node = get_next(node)) {
        size = Q_insert(Q, node, size);
    }
    
    // Huvudloop för Prim's algoritm
    while (!Q_is_empty(Q, size)) {
        pnode u = Q_extract_min(Q, size); // Hämta noden med lägst `d`
		// Loopa över alla grannar till `u`
		pedge edge = get_edges(u);
			
		while (edge != NULL) {
			pnode v = get_node(G, edge->to);

		
				// Om noden är i kön och kanten har lägre vikt
			if (Q_exists(Q, size, get_name(v)) && edge->weight < get_d(v)) {
				set_pi(v, get_name(u));  // Uppdatera föregångaren
				set_d(v, edge->weight); // Uppdatera vikten (d) 
				}
				edge = edge->next_edge;
			}
			size--;
		}

    // Spara resultaten i d och e 

		for (pnode node = G; !is_empty(node); node = get_next(node)) {
			if (node->name == start_node) {
				set_d(node, INFINITY);
			}
        	int pos = name_to_pos(G, get_name(node));
        	d[pos] = get_d(node);
        	e[pos] = get_pi(node);
    	} 
    // Frigör könens minne
    free(Q);
}

//--------------------------------------------------------------------------
// Floyd's algorithm: returns matrix of distances
// a -> b(1)
// |                   |0  |1  |3  |
// b -> c(2)    -->    |INF|0  |2  |
// |                   |INF|INF|0  |
// c
//--------------------------------------------------------------------------
void floyd(pnode G, double W[MAXNODES][MAXNODES])
{
	// Konvertera listan till en viktsmatris
    list_to_matrix(G, W); 

    // Floyd-Warshall algoritmen enligt din specifikation
    for (int k = 0; k < MAXNODES; k++) {
        for (int i = 0; i < MAXNODES; i++) {
            for (int j = 0; j < MAXNODES; j++) {
                // Undvik oändlighetsproblem vid addition
                if (W[i][k] != INFINITY && W[k][j] != INFINITY) {
                    if (W[i][j] > W[i][k] + W[k][j]) {
                        W[i][j] = W[i][k] + W[k][j];  // Uppdatera kortaste vägen
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------------
// Warshall's algorithm: returns matrix of closures, i.e. if paths exists
// a -> b(1)
// |                   |T  |T  |T  |
// b -> c(2)    -->    |F  |T  |T  |
// |                   |F  |F  |T  |
// c
//--------------------------------------------------------------------------
void warshall(pnode G, double W[MAXNODES][MAXNODES])
{
	list_to_matrix(G, W);  // Konvertera listan till en matris

    // Konvertera matrisen till binär (True/False) representation
    for (int i = 0; i < MAXNODES; i++) {
        for (int j = 0; j < MAXNODES; j++) {
            if (W[i][j] != INFINITY) {
                W[i][j] = 1;  // Markera att det finns en väg
            } else {
                W[i][j] = 0;  // Ingen väg
            }
        }
    }

    // Warshall-algoritmen
    for (int k = 0; k < MAXNODES; k++) {
        for (int i = 0; i < MAXNODES; i++) {
            for (int j = 0; j < MAXNODES; j++) {
                if (W[i][k] && W[k][j]) {
                    W[i][j] = 1;  // Om det finns en väg genom k, markera den
                }
            }
        }
    }
}
