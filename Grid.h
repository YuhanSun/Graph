/*Grid.h*/
#ifndef _GRID.H_
#define _GRID_H_

#include "Universe.h"

class MultiLevelGridIndexVector
{
	public:
		hash_map<int, vector<vector<vector<bool>>>> index;
};

class MultiLevelGridIndexSet
{
	public:
		hash_map<int, vector<vector<set<bool>>>> index;
};

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
//use set to store index
void GenerateGridIndexSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue);

//bad performance
void GenerateGridPointIndexNotInQueue(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<set<int>> &index);

//sort first
void GenerateGridPointIndexPartialSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored);
void GenerateGridPointIndexPartialSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored);

int Return_resolution(int grid_id);
int Return_Offset(int pieces);

//based on a threshold
void GenerateMultilevelGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored, int merge_count);
void GenerateMultilevelGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored, int merge_count);

//no threshold
void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue, int merge_count);
void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue, int merge_count);

//bad performace
void SetFalseRecursive(vector<vector<bool>> &index, int id, int grid_id);
void SetFalseRecursive(vector<set<int>> &index, int id, int grid_id);
void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, MultiLevelGridIndexVector &multilevel_index, queue<int> &queue, int merge_count);
void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, MultiLevelGridIndexSet &multilevel_index, queue<int> &queue, int merge_count);


void GridPointIndexToDiskSkip(vector<vector<bool>> &index, string filename);
void GridPointIndexToDiskSkip(vector<set<int>> &index, string filename);

void GridPointIndexToDisk(vector<vector<bool>> &index, string filename);
void GridPointIndexToDisk(vector<set<int>> &index, string filename);
void GridPointIndexToDisk(vector<vector<bool>> &index, string filename, vector<bool> &IsStored);
void GridPointIndexToDisk(vector<set<int>> &index, string filename, vector<bool> &IsStored);
void GridPointIndexToDisk(vector<vector<bool>> &index, string filename, vector<int> stored, int total_count);//for read exist approach
void GridPointIndexToDisk(vector<set<int>> &index, string filename, vector<int> stored, int total_count);//for read exist approach

void GridPointMultilevelIndexToDisk(MultiLevelGridIndexVector &multilevel_index, string filename, int pieces_max);

void ReadGridPointIndexFromDisk(int total_pieces, vector<vector<bool>> &index, string filename);//full storage in memory
void ReadGridPointIndexMultilevelFromDisk(int pieces, vector<vector<bool>> &index, string filename, vector<int> &isstored, int &node_count);//partial storage in memory
void ReadGridPointIndexMultilevelFromDisk(int pieces, vector<set<int >> &index, string filename, vector<int> &isstored, int &node_count);//partial storage in memory


#endif