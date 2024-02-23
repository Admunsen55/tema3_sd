/* Androne Andrei - 312CC */
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ndgraph_p initialize_ndgraph (int nr);
void delete_ndgraph (ndgraph_p graph);
void add_line_data (ndgraph_p graph,  char* s1, char *s2, int w); 
void add_zone_data (ndgraph_p graph, zoned *zone_data);
void prim_for_all_zones (ndgraph_p graph, zoned *zone_data, int min_costs[]);

dgraph_p initialize_dgraph (int nr);
void delete_dgraph (dgraph_p graph);
void add_edges_data (dgraph_p graph, char **location_names, char* s1, char *s2, int dist);
void set_depth (dgraph_p graph, char **location_names, char *str, int depth);
void find_island_and_ship (dgraph_p graph, char **location_names, int *island, int *ship);
int dfs_check_connection(dgraph_p graph, int node1, int node2);
int dijkstra(dgraph_p graph, int start_node, int stop_node, int *previous);
void track_path(int* previous, int node, int nr_nodes, char **location_names, FILE *f);
int min_depth (dgraph_p graph, int node, int *previous);

//functie folosita pentru "qsort"
int compare (const void *a, const void *b)
{
	int num1 = *(const int*)a;
    int num2 = *(const int*)b;

	return num1 - num2;
}

int main(int argc, char const *argv[])
{
	FILE *f_in, *f_out;
	/* verificam numarul de argumente in linia de comanda si faptul ca fisierul a putut fi
	deschis */
	if (argc != 2) {
		printf("Incorrect arguments number in command line: %d !!!\n", argc);
		return 0;
	} 

	f_in = fopen("tema3.in", "r");
	f_out = fopen("tema3.out", "w");

	int nr_nodes, nr_edges;
	int i;
	char aux_name_1[20];
	char aux_name_2[20];
	fscanf(f_in, "%d", &nr_nodes);
	fscanf(f_in, "%d", &nr_edges);

	//cerinta 1
	if (!strcmp(argv[1], "1")) {
		int weight;			
		//initializam graful
		ndgraph_p graph;	
		graph = initialize_ndgraph(nr_nodes);					

		/* min_costs = vector alocat dinamic care va contine costurile de renovare
		pentru zonele despartite de apa */
		int *min_costs;
		zoned zone_data;

		//citim datele din fisier
		for (i = 0; i < nr_edges; i++) {
			fscanf(f_in, "%s", aux_name_1);
			fscanf(f_in, "%s", aux_name_2);
			fscanf(f_in, " %d", &weight);

			add_line_data(graph, aux_name_1, aux_name_2, weight);
		}
		zone_data.nr_nodes_zone = (int*)malloc(sizeof(int) * nr_nodes);
		zone_data.starting_zone = (int*)malloc(sizeof(int) * nr_nodes);
		add_zone_data(graph, &zone_data);
		zone_data.nr_nodes_zone = realloc(zone_data.nr_nodes_zone, 
zone_data.nr_zones * sizeof(int));
		zone_data.starting_zone = realloc(zone_data.starting_zone, 
zone_data.nr_zones * sizeof(int));		
		// punem in vectorul "min_costs" costurile minime 
		min_costs = (int*)malloc(sizeof(int) * zone_data.nr_zones);
		prim_for_all_zones(graph, &zone_data, min_costs);
		//sortam in ordine crescatoare costurile de renovare
		qsort(min_costs, zone_data.nr_zones, sizeof(int), compare);

		//afisam in fisier datele cerute
		fprintf(f_out, "%d\n", zone_data.nr_zones);
		for (i = 0; i < zone_data.nr_zones; i++) {
			fprintf(f_out, "%d\n", min_costs[i]);
		}
			
		//eliberam memoria
		free(min_costs);
		free(zone_data.starting_zone);
		free(zone_data.nr_nodes_zone);
		delete_ndgraph(graph);

	//cerinta 2
	} else if (!strcmp(argv[1], "2")) {
		int check1, check2;
		int distance, depth, treasure_weight;
		int island_location, ship_location;
		dgraph_p graph;
		graph = initialize_dgraph(nr_nodes);
		/* vector in care elementul i va reprezenta nodul parcurs inainte
		de nodul i in algoritmul lui Dijkstra (folosit ulterior pentru a
		reconstitui drumul) */
		int *previous = (int*)malloc(sizeof(int) * nr_nodes);
		/* vector de siruri de caractere in care fiecare indice corespunde
		cu indicele din nodul nu numele respectiv */
		char **location_names;
		location_names = (char**)malloc(sizeof(char*) * nr_nodes);

		for (i = 0; i < nr_edges; i++) {
			fscanf(f_in, "%s", aux_name_1);
			fscanf(f_in, "%s", aux_name_2);
			fscanf(f_in, " %d", &distance);
			add_edges_data(graph, location_names, aux_name_1, aux_name_2, distance);
		}
		for (i = 0; i < nr_nodes; i++) {
			fscanf(f_in, "%s", aux_name_1);
			fscanf(f_in, " %d", &depth);
			set_depth(graph, location_names, aux_name_1, depth);
		}
		fscanf(f_in, "%d", &treasure_weight);
		find_island_and_ship(graph, location_names, &island_location, &ship_location);

		//verificam daca se poate realiza drumul dus-intors
		check1 = dfs_check_connection(graph, island_location, ship_location);
		check2 = dfs_check_connection(graph, ship_location, island_location);
		//verificam drumul dus
		if (!check2) {
			fprintf(f_out, "Echipajul nu poate ajunge la insula\n");
			goto free_memory;
		}
		//verificam drumul intors
		if (!check1) {
			fprintf(f_out, "Echipajul nu poate transporta comoara inapoi la corabie\n");
			goto free_memory;
		}

		distance = dijkstra(graph, island_location, ship_location, previous);
		track_path(previous, ship_location, nr_nodes, location_names, f_out);
		fprintf(f_out, "%d\n", distance);

		depth = min_depth(graph, ship_location, previous);
		fprintf(f_out, "%d\n", depth);
		fprintf(f_out, "%d\n", treasure_weight / depth);

free_memory:		
		free(previous);
		for (i = 0; i < nr_nodes; i++) {
			free(location_names[i]);
		}
		free(location_names);
		delete_dgraph(graph);
	} else {
		printf("Invalid request !!!\n");
	}
	//inchidem fisierele
	fclose(f_in);
	fclose(f_out);
	return 0;
}