#include <stdio.h>
#include <stdlib.h>

#include "core/graph.hpp"

void compute(Graph<Empty> * graph) {
	double exec_time = 0;
	exec_time -= get_time();

	//VertexId * degree = graph->alloc_vertex_array<VertexId>();
  	VertexId * degree = graph->alloc_vertex_array<VertexId>();
	VertexSubset * active = graph->alloc_vertex_subset();
	active->fill();
	graph->fill_vertex_array(degree, (VertexId) 0);

	graph->process_edges<VertexId, VertexId> (
			[&](VertexId src) {
				graph->emit(src, src);
			},

			[&](VertexId src, VertexId msg, VertexAdjList<Empty> outgoing_adj) {
				//VertexId count = 0;
				for (AdjUnit<Empty> * ptr = outgoing_adj.begin; ptr != outgoing_adj.end; ptr ++) {
          				VertexId dst = ptr->neighbour;
					if (degree[dst] < 2) {
						write_add(&degree[dst], (VertexId)1);
					}
				}
				return 0;
			},

			[&](VertexId dst, VertexAdjList<Empty> incoming_adj) {
				VertexId count = 0;
				for (AdjUnit<Empty> * ptr = incoming_adj.begin; ptr != incoming_adj.end; ptr ++) {
					count ++;
					if (count >= 2) {
						break;
					}
				}
				//printf("%d\n", count);
				graph->emit(dst, count);
			},

			[&](VertexId dst, VertexId msg) {
				if (degree[dst] < 2) {
					write_add(&degree[dst], msg);
				}
				return degree[dst] >= 2;
			},

			active

			);

	exec_time += get_time();

	if (graph->partition_id == 0) {
    		printf("exec_time=%lf(s)\n", exec_time);
	}

	graph->gather_vertex_array(degree, 0);
	if (graph->partition_id == 0) {
		VertexId valid_vertex = 0;
		for (VertexId v_i = 0; v_i < graph->vertices; v_i ++) {
			if (degree[v_i] >= 2) {
				valid_vertex += 1;
			}
		}
		printf("number of vertices with no less than 2 neighbours = %u\n", valid_vertex);
	}

	graph->dealloc_vertex_array(degree);
	delete active;
}

int main(int argc, char **argv) {
	MPI_Instance mpi(&argc, &argv);

	if (argc < 3) {
		printf("trim2 [file] [vertices]\n");
		exit(-1);
	}

	Graph<Empty> * graph;
	graph = new Graph<Empty>();
	graph->load_directed(argv[1], std::atoi(argv[2]));

	compute(graph);
	/*
	for (int run = 0; run < 5; ++ run) {
		compute(graph);
	}
	*/

	delete graph;

	return 0;
}
