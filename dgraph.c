/* Androne Andrei - 312CC */
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>

//functie care initializeaza un graf orientat
dgraph_p initialize_dgraph(int nr)
{
    int i;
    dgraph_p graph = (dgraph_p)malloc(sizeof(dgraph));
    graph->nr_nodes = nr;
    graph->curr_size = 0;
    graph->list = (dnode_p*)malloc(sizeof(dnode_p) * nr);

    //initializam fiecare nod
    for (i = 0; i < nr; i++)
    {
        graph->list[i] = (dnode_p)malloc(sizeof(dnode));
        graph->list[i]->location = i;
        graph->list[i]->distance = 0;
        graph->list[i]->depth = -1;
        graph->list[i]->edge = NULL; // Initialize the edge pointer to NULL
    }

    return graph;
}

//functie care elibereaza memoria alocata dinamic pentru un graf
void delete_dgraph (dgraph_p graph)
{
    int i, nr = graph->nr_nodes;
    dnode_p curr, next;
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

/* functie care verifica daca un nod exista deja in graf. Daca exista,
atunci returneaza pozitia sa in vectorul list de elemente, altfel returneaza -1 */
int check_if_exists_nod (dgraph_p graph, char **location_names, char *s)
{
    int i;
    int cur_size = graph->curr_size;
    for (i = 0; i < cur_size; i++) {
        if (!strcmp(location_names[i], s)) {
            return i;
        }
    }
    return -1;
}

/* functie care adauga un nod nou in graph. Returneaza pozitia sa 
in vectorul list de elemente */
int add_dnode (dgraph_p graph, char **location_names, char *s)
{
    int cur_size = graph->curr_size;
    if (cur_size >= graph->nr_nodes) {
        printf("Graph is full !!!\n");
        return -1;
    }
    location_names[cur_size] = (char*)malloc(sizeof(char) * (strlen(s) + 1));
    strcpy(location_names[cur_size], s);
    (graph->curr_size)++;
    return cur_size;
}

/* functie care adauga o noua legatura in graph. poz1 si poz2 
reprezinta indicii nodurilor pentru care se face legatura in
lista de adiacenta */
void add_dedge (dgraph_p graph, int poz1, int poz2, int dist)
{
    dnode_p nod1 = graph->list[poz1];
    if (poz1 >= graph->curr_size || poz2 >= graph->curr_size) {
        printf("Add nodes before edges !!!\n");
        return;
    }
    dnode_p cur, next;

    //adaugam legatura nod1->nod2
    next = nod1;
    while (next != NULL) {
        cur = next;
        next = cur->edge;
    }
    //"cur" e acum pointer catre ultimul nod din lista de vecini ai lui nod1
    cur->edge = (dnode_p)malloc(sizeof(dnode));
    next = cur->edge;
    next->location = poz2;
    next->depth = -1;
    next->distance = dist;
    next->edge = NULL;

    return;
}

/* functie care adauga legaturile in graf in functie
de primele M linii din fisierul de intrare */
void add_edges_data (dgraph_p graph, char **location_names, char* s1, char *s2, int dist) 
{    
    int poz1, poz2;

    poz1 = check_if_exists_nod(graph, location_names, s1);
    poz2 = check_if_exists_nod(graph, location_names, s2);
    
    /* daca nodurile nu exista deja, intai le adaugam dupa care
    creeam legatura */
    if  (poz1 == -1 && poz2 == -1) {
        poz1 = add_dnode(graph, location_names, s1);
        poz2 = add_dnode(graph, location_names, s2);
        add_dedge(graph, poz1, poz2, dist);
        return;
    }
    if  (poz1 == -1 && poz2 >= 0) {
        poz1 = add_dnode(graph, location_names, s1);
        add_dedge(graph, poz1, poz2, dist);
        return;
    }
    if  (poz1 >= 0 && poz2 == -1) {
        poz2 = add_dnode(graph, location_names, s2);
        add_dedge(graph, poz1, poz2, dist);
        return;
    }
    if  (poz1 >= 0 && poz2 >= 0) {
        add_dedge(graph, poz1, poz2, dist);
        return;
    }
}

//functie care seteaza adancimea unui nod cu o anumita valoare
void set_depth (dgraph_p graph, char **location_names, char *str, int depth)
{
    int i, location_index;
    dnode_p cur, next;
    if (graph->curr_size < graph->nr_nodes) {
        printf("Depth set before all nodes were added !!!\n");
    }
    //memoram indicele corespunaztor locatiei
    location_index = check_if_exists_nod(graph, location_names, str);
    if (location_index == -1) {
        printf("Add node before setting depth !!!\n");
        return;
    }

    for (i = 0; i < graph->curr_size; i++) {
        //actualizam adancimea din lista de adiacenta
        if (graph->list[i]->location == location_index) {
            graph->list[i]->depth = depth;
        }

        //cautam nodul printre vecinii fiecrui alt nod pentru a schimba adancimea
        next = graph->list[i];
        while (next != NULL) {
            cur = next;
            next = cur->edge;
            if (cur->location == location_index) {
                cur->depth = depth;
            }
        }
    }

    return;
}

//functie care gaseste indicele de locatie al insulei si corabiei
void find_island_and_ship (dgraph_p graph, char **location_names, int *island, int *ship)
{
    int i;
    *ship = -1;
    *island = -1;
    for (i = 0; i < graph->curr_size; i++) {
        if (!strcmp(location_names[i], "Insula")) {
            *island = i;
        }
        if (!strcmp(location_names[i], "Corabie")) {
            *ship = i;
        }
        if ((*island) != -1 && (*ship) != -1) {
            return;
        }
    }
}

/* functie care realizeaza o parcurgere in adancime pana intalneste un nod anume (returneaza 0 daca 
nu-l intalneste, 1 daca il intalneste*/
int dfs_con(dgraph_p graph, int* visited, int current_node, int target_node)
{
    //marcam nodul curent ca fiind vizitat
    visited[current_node] = 1;

    //daca nodurile coincid atunci sunt conectate
    if (current_node == target_node) {
        return 1;
    }

    dnode_p cur = graph->list[current_node]->edge;

    while (cur != NULL) {
        //daca nodul vecin nu e vizitat
        if (!visited[cur->location]) {
            //verificam recursiv daca nodul vecin e conectat la nodul destinatie
            int connected = dfs_con(graph, visited, cur->location, target_node);
            if (connected) {
                return 1;
            }
        }
        cur = cur->edge;
    }

    return 0; 
}

//functie care verifica daca se poate realiza drumul de la un nod la altul
int dfs_check_connection(dgraph_p graph, int node1, int node2)
{
    int* visited = (int*)calloc(graph->curr_size, sizeof(int));
    //DFS de la nod1->nod2
    int connected = dfs_con(graph, visited, node1, node2);

    free(visited);
    return connected;
}

/* functie care cauta nodul cu drumul minim care inca nu a fost agaugat la 
arborele de acoperire minim */
int min_key_float(float *key, int *visited, int nr) {
    int i, min_index = 0;
    float min = FLT_MAX;

    for (i = 0; i < nr; i++) {
        if (visited[i] == 0 && key[i] < min) {
            min = key[i];
            min_index = i;
        }
    }

    return min_index;
}

/* functie care foloseste algoritmul dijkstra pentru a calcula costurile (prin formula 
data in enunt ) si distantele minime de la un nod de start pana la toate celelalte noduri.
returneaza distanta minima pana la nodul de stop, si pune in previous[] nodurile parcurse
anterior fiecarui nod in algorimtul dijkstra (vom folosi ulterior previous[] pentru a 
reconstitui drumul optim in alte functii) */
int dijkstra(dgraph_p graph, int start_node, int stop_node, int *previous) //returneaza distanta
{
    int i, distance;
    int key_node, neigh_index, nr_nodes = graph->nr_nodes;
    int* visited = (int*)malloc(sizeof(int) * nr_nodes);
    int* distances = (int*)malloc(sizeof(int) * nr_nodes);
    float cost, *key = (float*)malloc(sizeof(float) * nr_nodes);

    for (i = 0; i < nr_nodes; i++) {
        previous[i] = -1;
    }

    // folosim key[] pentru a retine costurile minime 
    for (i = 0; i < nr_nodes; i++) {
        key[i] = FLT_MAX;
        distances[i] = INT_MAX;
        visited[i] = 0;
    }

    // initializam "cheia" nodului de start (insula in cazul problemei)
    key[start_node] = 0;
    distances[start_node] = 0;

    for (i = 0; i < nr_nodes - 1; i++) {
        //gasimnodul cu cost minim dintre cele nevizitate
        key_node = min_key_float(key, visited, nr_nodes);
        visited[key_node] = 1;

        //actualizam distantele si costul din keys pentru vecinii nodului curent
        dnode_p cur = graph->list[key_node]->edge;
        while (cur != NULL) {
            neigh_index = cur->location;
            cost = (float)(cur->distance) / (cur->depth);
            if (!visited[neigh_index] && key[key_node] != FLT_MAX) {
                if (cost + key[key_node] < key[neigh_index]) {
                    key[neigh_index] = cost + key[key_node];
                    distances[neigh_index] = cur->distance + distances[key_node];
                    previous[neigh_index] = key_node;
                }
            }
            cur = cur->edge;
        }
    }

    distance = distances[stop_node];

    free(distances);
    free(key);
    free(visited);

    return distance;
}

//functie care afiseaza drumul parcurs pana la un nod
void track_path(int* previous, int node, int nr_nodes, char **location_names, FILE *f)
{
    //creeam un vector in care salvam nodurile parcurse
    int *reverse_order = (int*)malloc(sizeof(int) * nr_nodes);
    int i, index = 0;

    //construim drumul in ordine inversa
    while (node != -1) {
        reverse_order[index] = node;
        node = previous[node];
        index++;
    }

    //afisam drumul in ordinea corecta in fisier
    for (i = index - 1; i >= 0; i--) {
        fprintf(f, "%s ", location_names[reverse_order[i]]);
    }
    fprintf(f, "\n");

    free(reverse_order);
}

int min_depth (dgraph_p graph, int node, int *previous)
{
    //creeam un vector in care salvam nodurile parcurse
    int *reverse_order = (int*)malloc(sizeof(int) * graph->nr_nodes);
    int i, index = 0, min = INT_MAX;

    //construim drumul in ordine inversa
    while (node != -1) {
        reverse_order[index] = node;
        node = previous[node];
        index++;
    }

    //calculam minimul exceptand corabia si insula
    for (i = index - 2; i >= 1; i--) {
        if (graph->list[reverse_order[i]]->depth < min) {
            min = graph->list[reverse_order[i]]->depth;
        }
    }

    free(reverse_order);
    return min;
}