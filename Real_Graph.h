#ifndef REAL_GRAPH_H
#define REAL_GRAPH_H

#include "Universe.h"
void ReadID(hash_map<int, int> &id_index, string filename);
void ReadArbitaryGraphFromDisk(vector<vector<int>> &real_graph, hash_map<int, int> &id_index, string filename, int average_degree);

#endif