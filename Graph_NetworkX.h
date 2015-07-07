#ifndef _GRAPH_NETWORKX_H_
#define _GRAPH_NETWORKX_H_
#include "RMBR.h"

void ReadGraphFromEdge(vector<set<int>> &graph, string filename);
void ReadSCC(vector<set<int>> &SCC, string filename);
void ConnectSCCEntity(vector<Entity> &entity, vector<set<int>> &SCC);
void GenerateSCCGraph(vector<Entity> &entity, vector<set<int>> &graph_entity, vector<set<int>> &graph_SCC);
void GenerateSCCGraph(vector<Entity> &entity, vector<vector<int>> &graph_entity, vector<set<int>> &graph_SCC);
void Traverse(int id, vector<set<int>> &graph, vector<bool> &isvisted, vector<int> &reach_vertices);
vector<vector<int>> GetTransitiveClosure(vector<set<int>> &graph);

void GenerateEntityInSCC(int node_count, vector<Entity> &entity_vector, int range, double nonspatial_entity_ratio);//Generate entity which are randomly selected as spatial nodes and arbitary types
void ReadEntityInSCCFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename);
void EntityInSCC_To_Disk(vector<Entity> &entity_vector, int range, string filename);

void AddEdge(vector<set<int>> &graph, int start, int end);
void GenerateArbitaryGraph(vector<set<int>> &graph, int node_count, INT64 edge_count);
void GenerateArbitaryGraph(vector<vector<int>> &graph, int node_count, INT64 edge_count);
void GenerateArbitaryGraph(vector<vector<int>> &graph, int node_count, __int64 edge_count, double a, double b, double c);
void ArbitaryGraphToDisk(vector<set<int>> &graph, string filename);//grpah with no edge type and is for SCC graph
void ArbitaryGraphToDisk(vector<vector<int>> &graph, string filename);
void ReadArbitaryGraphFromDisk(vector<set<int>> &graph, int &node_count, string filename);
void ReadArbitaryGraphFromDisk(vector<vector<int>> &graph, int &node_count, string filename);

void GenerateRMBR(vector<Entity> &p_entity, vector<set<int>> &p_graph);

vector<set<int>> GetTransitiveClosureDynamic_In_Set(vector<set<int>> &graph);
vector<vector<int>> GetTransitiveClosureDynamicNew(vector<vector<int>> &p_graph);

#endif _GRAPH_NETWORKX_H_