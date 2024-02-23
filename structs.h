/* Androne Andrei - 312CC */
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct  NonDirectedGraphNode {
    char location[20];
    int weight;
    int index;
    struct NonDirectedGraphNode *edge;
} ndnode, *ndnode_p;

typedef struct NonDirectedGraph {
    int nr_nodes;
    int curr_size;
    ndnode_p *list;
} ndgraph, *ndgraph_p;

/* structura folosita pentru a retine numarul de zone despartite de rauri, nr de locatii
pentru fieacre zona, cat si un vector cu locatiile initiale din fiecare zona */
typedef struct ZoneData {
    int nr_zones;
    int *starting_zone;
    int *nr_nodes_zone;
} zoned;

typedef struct  DirectedGraphNode {
    int location;
    int depth;
    int distance;
    struct DirectedGraphNode *edge;
} dnode, *dnode_p;

typedef struct DirectedGraph {
    int nr_nodes;
    int curr_size;
    dnode_p *list;
} dgraph, *dgraph_p;

#endif 