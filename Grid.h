/*Grid.h*/
#ifndef _GRID.H_
#define _GRID_H_

#include "Universe.h"
//using set to store index and without update
void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<set<int>> &index);
//using bool to store index and with update
void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, vector<vector<bool>> &update);
//using bool to store index and with update stored with set
void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, vector<set<int>> &update);
//using bool to store index and with update stored with vector
void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, vector<vector<int>> &update);
//using bool to store index and without update
void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<vector<bool>> &index);

//using bool to store index 
void GenerateGridIndexSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue);

void GenerateGridPointIndexNotInQueue(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<set<int>> &index);

void GridPointIndexToDiskSkip(vector<vector<bool>> &index, string filename);
void GridPointIndexToDiskSkip(vector<set<int>> &index, string filename);

void GridPointIndexToDisk(vector<vector<bool>> &index, string filename);
void GridPointIndexToDisk(vector<set<int>> &index, string filename);


void TopologicalSortUtil(int v, vector<bool> &visited, queue<int> &Stack, vector<vector<int>> &graph);
void TopologicalSort(vector<vector<int>> &graph, queue<int> &stack);

void ReadGridPointIndexFromDisk(int total_pieces, vector<vector<bool>> &index, string filename);
#endif