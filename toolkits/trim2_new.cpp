#include <stdio.h>
#include <stdlib.h>

#include "core/graph.hpp"

void compute(Graph<Empty> *graph) {
	double exec_time = 0;
	exec_time -= get_time();

	VertexId * in_degree = graph->alloc_vertex_array<VertexId>(); // only the in_degree information stored in master node is correct
	VertexSubset * active = graph->alloc_vertex_subset();
	active->fill();
	VertexSubset * next_active = graph->alloc_vertex_subset();
	VertexId actives, next_actives;
	actives = graph->vertices;

	VertexId i_i;
	for (i_i = 0; 1; i_i ++) {
		if (graph->partition_id == 0) {
			printf("active(%d) %u\n", i_i, actives);
		}
		graph->fill_vertex_array(in_degree, (VertexId) 0);
		//next_active->copy_from(active);
		next_active->clear();
		next_actives = graph->process_edges<VertexId, VertexId>(
				[&](VertexId src) { // sparse signal
					graph->emit(src, src);
				}, 

				[&](VertexId src, VertexId msg, VertexAdjList<Empty> outgoing_adj) { // sparse slot
					VertexId activated = 0;
					for (AdjUnit<Empty> * ptr = outgoing_adj.begin; ptr != outgoing_adj.end; ++ ptr) {
						VertexId dst = ptr->neighbour;
						if (in_degree[dst] < 2) {
							/*
							volatile old_val, new_val;
							do {
								old_val = in_degree[dst];
								new_val = old_val + 1;
							} while (!cas(&in_degree[dst], old_val, new_val));
							if (new_val == 2) {
								activated ++;
								next_active->set_bit(dst);
							}
							*/
							VertexId old_val = __sync_fetch_and_add(&in_degree[dst], (VertexId) 1);
							if (old_val == 1) {
								activated ++;
								next_active->set_bit(dst);
							}
						}
					}
					return activated;
				}, 

				[&](VertexId dst, VertexAdjList<Empty> incoming_adj) {
					VertexId count = 0;
					for (AdjUnit<Empty> * ptr = incoming_adj.begin; ptr != incoming_adj.end; ptr ++) {
						VertexId src = ptr->neighbour;
						if (active->get_bit(src)) {
							count += 1;
							if (count >= 2) {
								break;
							}
						}
					}
					graph->emit(dst, count);
				}, 

				[&](VertexId dst, VertexId msg) {
					if (in_degree[dst] >= 2) return 0;
					/*
					volatile old_val, new_val;
					do {
						old_val = in_degree[dst];
						new_val = old_val + msg;
					} while (!cas(&in_degree[dst], old_val, new_val));
					return (old_val < 2 && new_val >= 2);
					*/
					VertexId old_val = __sync_fetch_and_add(&in_degree[dst], msg);
					if (old_val < 2 && old_val + msg >= 2) {
						next_active->set_bit(dst);
						return 1;
					}
					return 0;
				}, 

				active

				);

		/*
		if (graph->partition_id == 0) {
			printf("next actives = %u\n", next_actives);
		}
		*/

		assert(next_actives <= actives);
		if (next_actives == actives) {
			break;
		}
		actives = next_actives;
		std::swap(active, next_active);
	}

	exec_time += get_time();
	if (graph->partition_id == 0) {
		printf("exec_time = %lf(s)\n", exec_time);
		printf("number of vertices with no less than 2 in-degree = %u\n", actives);
	}

	graph->dealloc_vertex_array(in_degree);
	delete active;
	delete next_active;
}

int main(int argc, char ** argv) {
	MPI_Instance mpi(&argc, &argv);

	if (argc < 3) {
		printf("trim2_new [file] [vertices]\n");
		exit(-1);
	}

	Graph<Empty> * graph;
	graph = new Graph<Empty>();
	graph->load_directed(argv[1], std::atoi(argv[2]));

	compute(graph);
	for (int run = 0; run < 5; ++ run) {
		compute(graph);
	}

	delete graph;
	return 0;
}
