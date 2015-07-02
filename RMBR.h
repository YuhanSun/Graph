/*RMBR.h*/
#ifndef _RMBR_H_
#define _RMBR_H_

#include "Universe.h"
#include "RTree.h"

void Initialize_Visited_vertex();

bool RecUnion(MyRect &rect_v, MyRect &rect_neighbour);
void GenerateRMBR(vector<Entity> &p_entity,vector<vector<int>> &p_graph);

void RMBR_To_Disk(vector<Entity> &p_entity, int range, string filename);
void ReadRmbr(vector<Entity> &p_entity, int &range, string filename);

bool ReachabilityQuery(vector<Entity> &entity,vector<vector<int>> &graph, int start_id, MyRect query_rect);

//for transitive closure testing
void Traverse(int id, vector<vector<int>> &graph, vector<bool> &isvisted, vector<int> &reach_vertices);
vector<vector<int>> GetTransitiveClosure(vector<vector<int>> &graph, vector<Entity> &entity);
vector<set<int>> GetTransitiveClosureDynamic_In_Set(vector<vector<int>> &graph, vector<Entity> &entity);
vector<hash_set<int>> GetTransitiveClosureDynamic(vector<vector<int>> &p_graph, vector<Entity> &p_entity);
vector<set<int>> GetTransitiveClosureDynamic(vector<vector<int>> &graph_outedge, vector<vector<int>> &graph_inedge);
vector<MyRect> GetRMBRFromTransitiveClosure(vector<vector<int>> &transitive_closure, vector<Entity> &entity);
vector<int>GetTransitiveClosureLine(int i, vector<vector<int>> &graph, vector<Entity> &entity);

void ReadTransitiveClosure(vector<vector<int>> &transitive_closure, int &node_count, int &range, string filename);

void TransitiveClosure_To_Disk(vector<vector<int>> &transitive_closure, int range, string filename);
void TransitiveClosureDynamic_To_Disk(vector<set<int>> &transitive_closure_dynamic, int range, string filename);
void TransitiveClosureDynamic_To_Disk(vector<hash_set<int>> &transitive_closure_dynamic, int range, string filename);
void SpatialTransitiveClosureDynamic_To_Disk(vector<set<int>> &transitive_closure_dynamic, int range, string filename, vector<Entity> p_entity);
void SpatialTransitiveClosure_To_Disk(vector<vector<int>> &transitive_closure, int range, string filename, vector<Entity> p_entity);
void SpatialTransitiveClosureLine_To_Disk(vector<int> &transitive_closure_line, int id, string filename, vector<Entity> p_entity);

void TransitiveClosureRMBR_To_Disk(vector<MyRect> RMBR, int range, string filename);

#endif