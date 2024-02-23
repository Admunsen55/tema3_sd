/* Androne Andrei - 312CC */
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//functie care initializeaza un graf neorientat
ndgraph_p initialize_ndgraph (int nr)
{
    int i;
    ndgraph_p graph = (ndgraph_p)malloc(sizeof(ndgraph));
    graph->nr_nodes = nr;
    graph->curr_size = 0;
    graph->list = (ndnode_p*)malloc(sizeof(ndnode_p) * nr);
    //initializam fiecare nod
    for (i = 0; i < nr; i++) {
        graph->list[i] = (ndnode_p)malloc(sizeof(ndnode));
        graph->list[i]->edge = NULL;
        graph->list[i]->index = i;
        graph->list[i]->weight = 0;
        strcpy(graph->list[i]->location, "empty");
    }
    return graph;
}

//functie care elibereaza memoria alocata dinamic pentru un graf
void delete_ndgraph (ndgraph_p graph)
{
    int i, nr = graph->nr_nodes;
    ndnode_p curr, next;
    if (graph == NULL) {
        printf("Graph already deleted !!!\n");
    }
    //eliberam memoria folosita pentru vecinii din lista de adiacenta
    for (i = 0; i < nr; i++) {
        next = graph->list[i];
        while (next != NULL)
        {
            curr = next;
            next = curr->edge;
            free(curr);
        }        
    }
    free(graph->list);
    free(graph);
    graph = NULL;

    return;
}

/* functie care verifica daca un element exista deja in graf. Daca exista,
atunci returneaza pozitia sa in vectorul list de elemente, altfel returneaza -1 */
int check_if_exists_location (ndgraph_p graph, char *s)
{
    int i;
    int cur_size = graph->curr_size;
    for (i = 0; i < cur_size; i++) {
        if (!strcmp(graph->list[i]->location, s)) {
            return i;
        }
    }
    return -1;
}

/* functie care adauga un nod nou in graph. Returneaza pozitia sa 
in vectorul list de elemente */
int add_ndnode (ndgraph_p graph, char *s)
{
    int cur_size = graph->curr_size;
    if (cur_size >= graph->nr_nodes) {
        printf("Graph is full !!!\n");
        return -1;
    }
    strcpy(graph->list[cur_size]->location, s);
    (graph->curr_size)++;
    return cur_size;
}

/* functie care adauga o noua legatura in graph. poz1 si poz2 
reprezinta indicii nodurilor pentru care se face legatura in
lista de adiacenta */
void add_ndedge (ndgraph_p graph, int poz1, int poz2, int w)
{
    char str[20] = "empty";
    ndnode_p nod1 = graph->list[poz1];
    ndnode_p nod2 = graph->list[poz2];
    if (!strcmp(nod1->location, str) || !strcmp(nod1->location, str)) {
        printf("Add nodes before edges !!!\n");
        return;
    }
    ndnode_p cur, next;

    //adaugam legatura nod1->nod2
    next = nod1;
    while (next != NULL) {
        cur = next;
        next = cur->edge;
    }
    //"cur" e acum pointer catre ultimul nod din lista de vecini ai lui nod1
    cur->edge = (ndnode_p)malloc(sizeof(ndnode));
    next = cur->edge;
    strcpy(next->location, nod2->location);
    next->weight = w;
    next->index = poz2;
    next->edge = NULL;

    //adaugam legatura nod2->nod1
    next = nod2;
    while (next != NULL) {
        cur = next;
        next = cur->edge;
    }
    //"cur" e acum pointer catre ultimul nod din lista de vecini ai lui nod2
    cur->edge = (ndnode_p)malloc(sizeof(ndnode));
    next = cur->edge;
    strcpy(next->location, nod1->location);
    next->weight = w;
    next->index = poz1;
    next->edge = NULL;   
    return;
}

/* functie care adauga date in graf (noduri si muchii) in functie
de fiecare linie din fisierul de intrare */
void add_line_data (ndgraph_p graph, char* s1, char *s2, int w) 
{
    int poz1, poz2;

    poz1 = check_if_exists_location(graph, s1);
    poz2 = check_if_exists_location(graph, s2);
    
    /* daca nodurile nu exista deja, intai le adaugam dupa care
    creeam legatura */
    if  (poz1 == -1 && poz2 == -1) {
        poz1 = add_ndnode(graph, s1);
        poz2 = add_ndnode(graph, s2);
        add_ndedge(graph, poz1, poz2, w);
        return;
    }
    if  (poz1 == -1 && poz2 >= 0) {
        poz1 = add_ndnode(graph, s1);
        add_ndedge(graph, poz1, poz2, w);
        return;
    }
    if  (poz1 >= 0 && poz2 == -1) {
        poz2 = add_ndnode(graph, s2);
        add_ndedge(graph, poz1, poz2, w);
        return;
    }
    if  (poz1 >= 0 && poz2 >= 0) {
        add_ndedge(graph, poz1, poz2, w);
        return;
    }
}

/* functie recursiva pentru parcurgere in adancime (folosita pentru a afla dimensiunea
unei componente a grafului) */
int dfs(ndgraph_p graph, int node, int* visited) {
    //initializam dimensiunea cu 1 caci componenta oricum contine primul nod
    int size = 1;
    visited[node] = 1;
    ndnode_p curr = graph->list[node]->edge;

    //parcurgem toti vecinii care nu au fost explorati
    while (curr != NULL) {
        if (!visited[curr->index]) {
            size += dfs(graph, curr->index, visited); 
        }
        curr = curr->edge; 
    }

    return size;
}

//functie folosita pentru a determina urmatorul nod neexeplorat
int find_next (int *vector, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        if (vector[i] == 0) {
            return i;
        }
    }
    //cazul cand toate nodurile au fost explorate
    return -1;
}

//functie care calc
void add_zone_data(ndgraph_p graph, zoned *zone_data) {
    int next_start, size, nr = 0;
    //vector de aparitie pentru a retine vecinii explorati
    int* visited = (int*)calloc(graph->nr_nodes, sizeof(int)); 

    next_start = find_next(visited, graph->nr_nodes);
    while (next_start >= 0) 
    {
        size = dfs(graph, next_start, visited);
        zone_data->starting_zone[nr] = next_start;
        zone_data->nr_nodes_zone[nr] = size;
        nr++;

        next_start = find_next(visited, graph->nr_nodes);
    }
    zone_data->nr_zones = nr;
    
    free(visited); 
}

/* functie care cauta nodul cu legatura minima care inca nu a fost agaugat la 
arborele de acoperire minim */
int min_key(int key[], int traversed_nodes[], int nr) {
    int i, min = INT_MAX, min_index;

    for (i = 0; i < nr; i++) {
        if (traversed_nodes[i] == 0 && key[i] < min) {
            min = key[i];
            min_index = i;
        }
    }

    return min_index;
}

void prim_for_all_zones(ndgraph_p graph, zoned *zone_data, int min_costs[]) 
{
    int i, j, weight, key_node, neigh_index, start_node, sum;
    int nr_nodes = graph->nr_nodes;
    int *path_weights = (int*)malloc(sizeof(int) * nr_nodes);
    int *key = (int*)malloc(sizeof(int) * nr_nodes);
    int *traversed_nodes = (int*)malloc(sizeof(int) * nr_nodes);
    ndnode_p curr;

    for (i = 0; i < nr_nodes; i++) {
        key[i] = INT_MAX;
        traversed_nodes[i] = 0;
    }

    //parcurgem fiecare zona si aflam drumul minim
    for (i = 0; i < zone_data->nr_zones; i++) {
        //"initializam" nodul de plecare pentru zona curenta
        start_node = zone_data->starting_zone[i];
        key[start_node] = 0;

        //parcurgem nodurile din zona curenta
        for (j = 0; j < zone_data->nr_nodes_zone[i]; j++) {
            key_node = min_key(key, traversed_nodes, nr_nodes);
            traversed_nodes[key_node] = 1;
            path_weights[j] = key[key_node];

            /* actualizam vectorul 'key' cu costurile minime catre vecinii neexplorati
            (la fiecare iteratie, 'curr' va retine unul dintre vecinii nodului adaugat) */
            curr = graph->list[key_node]->edge;
            while (curr != NULL) {
                neigh_index = curr->index;
                weight = curr->weight;
                if (traversed_nodes[neigh_index] == 0 && weight < key[neigh_index]) {
                    key[neigh_index] = weight;              
                }
                curr = curr->edge;
            }
        }
        //calculam costul total al drumului (renovarii) pt zona curenta
        sum = 0;
        for (j = 0; j < zone_data->nr_nodes_zone[i]; j++) {
            sum += path_weights[j];
        }
        min_costs[i] = sum;
    }
    free(path_weights);
    free(traversed_nodes);
    free(key);
}
