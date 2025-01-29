#include "adjlist.h"
// local prototypes
pnode create_node(char nname);
pnode node_cons(pnode first, pnode second);
pedge create_edge(char to, double weight);
pedge edge_cons(pedge first, pedge second);
pedge _add_edge(pedge edge, char to, double weight);
pedge _rem_edge(pedge edges, char to);
void remove_all_edges_to(pnode G, char name);
void remove_all_edges_from(pnode G, char name);

// create_node: creates node with name nname
pnode create_node(char nname)
{
	pnode new_node = (pnode)calloc(1, sizeof(node));
	new_node->name = nname;
	new_node->d = INFINITY;
	new_node->pi = '-';
	new_node->next_node = NULL;
	new_node->edges = NULL;
	return new_node;
}

// function implementations
bool is_empty(pnode G)
{
	return !G;
}
char get_name(pnode G)
{
	return is_empty(G) ? '-' : G->name;
}
pnode set_name(pnode G, char name)
{
	if (!is_empty(G))
		G->name = name;
	return G;
}
pedge get_edges(pnode G)
{
	return is_empty(G) ? NULL : G->edges;
}
pnode set_edges(pnode G, pedge E)
{
	if (!is_empty(G))
		G->edges = E;
	return G;
}
pnode get_next(pnode G)
{
	return is_empty(G) ? NULL : G->next_node;
}
// see node_cons() for the corresponding "set-next" function
double get_d(pnode G)
{
	return is_empty(G) ? INFINITY : G->d;
}
pnode set_d(pnode G, double d)
{
	if (!is_empty(G))
		G->d = d;
	return G;
}
char get_pi(pnode G)
{
	return is_empty(G) ? '-' : G->pi;
}
pnode set_pi(pnode G, char pi)
{
	if (!is_empty(G))
		G->pi = pi;
	return G;
}
// node_cons: connects two nodes in adjacency list
pnode node_cons(pnode first, pnode second)
{
	if (is_empty(first))
		return NULL;
	first->next_node = second;
	return first;
}
// add_node: adds a new node with name nname to adjacency list G
//           in lexicographical order. If it already exists
//           in graph, nothing is done
pnode add_node(pnode G, char nname)
{
	if (is_empty(G)) {
		G = create_node(nname);
		return G;
	}
	if (find_node(G, nname)) {
		return G;
	}
	pnode new_node = create_node(nname);
	pnode current = G;
	if (nname < get_name(G) ) {	
		node_cons(new_node, G);
		return new_node;
	} 
	while (get_next(current) != NULL && get_name(get_next(current))< nname) {
		current = get_next(current);
	}
	new_node->next_node = get_next(current);
	current->next_node = new_node;
	return G;
}
// rem_node: removes node with name name from adjacency list G
//           if node does not exist, nothing happens
pnode rem_node(pnode G, char name)
{
	if (is_empty(G)) {
		return G;
	}
	pnode current = G;
	pnode previous = NULL;
	if (current->name == name && current !=  NULL) {
		G = current->next_node;
		free(current);
		return G;
	}
	while (current != NULL && current->name != name) {
		previous = current;
		current = current->next_node;
	}
	if (current == NULL) {
		return G;
	}
	previous->next_node = current->next_node;
	free(current);
	return G;
}
// get_node: returns pointer to node with name name from adjacency list G
pnode get_node(pnode G, char name)
{
	while (!is_empty(G)) {
		if (name == get_name(G)) {
			return G; 
		}
		G = get_next(G);
	}
	return NULL;
}
// find_node: returns true if node with name name exists in adjacency list G
//           false otherwise
bool find_node(pnode G, char name)
{
	return is_empty(G)	   ? false :
	       get_name(G) == name ? true :
				     find_node(G->next_node, name);
}
// create_edge: creates edge
pedge create_edge(char to, double weight)
{
	pedge new_edge = (pedge)calloc(1, sizeof(edge));
	new_edge->to = to;
	new_edge->weight = weight;
	return new_edge;
}
bool edge_empty(pedge E)
{
	return !E;
}
char get_to(pedge E)
{
	return edge_empty(E) ? '-' : E->to;
}
pedge set_to(pedge E, char to)
{
	if (!edge_empty(E))
		E->to = to;
	return E;
}
double get_weight(pedge E)
{
	return edge_empty(E) ? INFINITY : E->weight;
}
pedge set_weight(pedge E, double weight)
{
	if (!edge_empty(E))
		E->weight = weight;
	return E;
}
pedge get_next_edge(pedge E)
{
	return edge_empty(E) ? NULL : E->next_edge;
}
// edge_cons: connects two edges in edge list
pedge edge_cons(pedge first, pedge second)
{
	if (edge_empty(first))
		return NULL;
	first->next_edge = second;
	return first;
}
// upd_edge: updates edge E to new weight
pedge upd_edge(pedge E, double weight)
{
	if (!edge_empty(E)) 
		set_weight(E, weight);
	return E;
}
// _add_edge: creates and connects new edge to edge-list
pedge _add_edge(pedge E, char to, double weight)
{
	pedge new_edge = create_edge(to, weight);
	if (edge_empty(E))  {
		return new_edge;
	}
	pedge current = E;
	while (!edge_empty(current->next_edge)) {
		current = current->next_edge;
	}
	current->next_edge = new_edge;
	return E;
}
// add_edge: adds an edge to G by finding correct start node
//           and then calling _add_edge to create new edge
void add_edge(pnode G, char from, char to, double weight)
{
	while(!is_empty(G)) {
		if (from == get_name(G)) {
			G->edges = _add_edge(G->edges, to, weight);
			break;
		}
		G = get_next(G);
	}
}
// _find_edge: finds edge in edge-list
bool _find_edge(pedge E, char to)
{
	while (!edge_empty(E))
	{
		if (get_to(E) == to) {
			return true;
		}
		E = E->next_edge;
	}
	return false;
}
// find_edge: returns true if edge between from and to exists, false otherwise
bool find_edge(pnode G, char from, char to)
{
	while (!is_empty(G)) {
		if (from == G->name) {
			return _find_edge(G->edges, to);
		}
		G = G->next_node;
	}
	return false;
}
// _edge_cardinality: returns the number of edges from one node
int _edge_cardinality(pedge E)
{
	int edge = 0;
	while (!edge_empty(E)) {
		edge++;
		E = E->next_edge;
	}
	return edge;
}
// edge_cardinality: returns the total number of edges in G
int edge_cardinality(pnode G)
{
	int Num_edges = 0;
	while (G != NULL) {
		Num_edges += _edge_cardinality(G->edges);
		G = G->next_node;
	}	
	return Num_edges;
}
// _self_loops: returns the number of edges going back to
//              source node
int _self_loops(pedge E, char src)
{
	// TODO
	int loops = 0;
	while (!edge_empty(E)) {
		if (src == E->to) {
			loops++;
		}
		E = E->next_edge;
	}
	return loops;
}
// self_loops: counts the number of self-loops, i.e. edges to and from
//             the same node
int self_loops(pnode G)
{
	// TODO
	int total_loops = 0;
	while (!is_empty(G)) {
		total_loops += _self_loops(G->edges, G->name);
		G = G->next_node;
	}
	return total_loops;
}
// _rem_edge: removes edge from edge-list
pedge _rem_edge(pedge E, char to)
{
	pedge current = E;
	pedge previous = NULL;
	while(!edge_empty(current)) {
		if (to == current->to) {
			if (edge_empty(previous)) {
				E = current->next_edge;	
			} else {
				previous->next_edge = current->next_edge;
			}
			free(current);
			return E;
		}
		previous = current;
		current = current->next_edge;
	}
	return E;
}
// rem_edge: removes edge from G
void rem_edge(pnode G, char from, char to)
{
	while (!is_empty(G)) {
		if (from == G->name){
		G->edges =_rem_edge(G->edges, to);
		} 
		G = G->next_node;
	}
}
// remove_all_edges_to: removes all edges going towards node with name name
void remove_all_edges_to(pnode G, char name)
{
	
	while (!is_empty(G)) {
		
		G->edges = _rem_edge(G->edges, name);
		G = G->next_node;
	}
} 

// remove_all_edges_from: removes all edges from node with name name
void remove_all_edges_from(pnode G, char name)
{
	while (!is_empty(G)) {
		if (get_name(G) == name) {
			while (!edge_empty(G->edges)) {
				G->edges = _rem_edge(G->edges, G->edges->to);
			}
			break;
		}		
		G = get_next(G);
	}
}
// node_cardinality: returns the number of nodes in G
int node_cardinality(pnode G)
{
	int nodes = 0;
	if (is_empty(G)){
		return 0;
	}
	while (G != NULL)
		{
			nodes++;
			G = get_next(G);
		}
	return nodes;
}
// name_to_pos: returns position of node with name c, -1 if not found
int name_to_pos(pnode G, char c)
{
	int pos = 0;
	while (!is_empty(G)) {
		if (get_name(G) == c) {
			return pos;
		}
		G = get_next(G);
		pos++;
	}
	return -1;
}
// pos_to_name: returns name of node at position pos in G
char pos_to_name(pnode G, int pos)
{
	int position = 0;
	while (!is_empty(G)) {
		if (position == pos) {
			return get_name(G);
		}
		G = get_next(G);
		position++;
	}
	return '-';
}
// list_to_pos: creates adjacency matrix from adjacency list
void list_to_matrix(pnode G, double matrix[MAXNODES][MAXNODES])
{
	int count = 0;
	pnode current = G;
	while (!is_empty(current)) {
		pedge E = get_edges(current);
		while (!edge_empty(E)) {	

			int pos = name_to_pos(G, get_to(E));
			if (pos >= 0 && pos < MAXNODES) {
				matrix[count][pos] = get_weight(E);
			}
			E = get_next_edge(E);
		}
		current = get_next(current);
		count++;
	}
}
