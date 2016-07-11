/*Universe.h*/
#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_
#include <iostream>
#include <fstream>
using namespace std;
#include <vector>
#include <set>
#include <hash_set>
#include <hash_map>
#include <stack>
#include <queue>
#include "Rand.h"
#include "RTree.h"
#include <iterator>
#include <time.h>
#include <sstream>

struct Location
{
	double x, y;

	Location(double a, double b)
	{
		x = a;
		y = b;
	}
	Location()
	{
		x = 0;
		y = 0;
	}
};

struct MyRect
{
	Location left_bottom;
	Location right_top;

	MyRect(double minx, double miny, double maxx, double maxy)
	{
		left_bottom.x = minx;
		left_bottom.y = miny;
		right_top.x = maxx;
		right_top.y = maxy;
	}

	MyRect()
	{
		left_bottom.x = 0;
		left_bottom.y = 0;
		right_top.x = 0;
		right_top.y = 0;
	}

};

struct Entity
{
	int id;
	bool IsSpatial;
	Location location;
	int type;
	MyRect RMBR;
	int scc_id;
};

struct Rect
{
	Rect()  {}

	Rect(double a_minX, double a_minY, double a_maxX, double a_maxY)
	{
		min[0] = a_minX;
		min[1] = a_minY;

		max[0] = a_maxX;
		max[1] = a_maxY;
	}


	double min[2];
	double max[2];
};

struct edge
{
	int edge_type;
	int vertex;
};

bool MySearchCallback(int id, void* arg);

vector<int> GetHitID();
void ResetHitID(int size);

int GetStartTime();
void SetStartTime(int i);

int GetRunTime();
void SetRunTime(int i);

//Judge whether a location is in a specific rectangle
bool Location_In_Rect(Location m_location, MyRect m_rect);

//Outfile entity to disk for storage
void EntityToDisk(Entity Entity_Matrix[], int node_count, int range, string filename);
void EntityToDisk(vector<Entity> entity_vector, int range, string filename);

//Out file entity to disk for displaying
void OutFile(Entity Entity_Matrix[], int node_count, string filename);
void OutFile(vector<Entity> &entity_vector, string filename);

//Generate entity with specific spatial entity ratio
void GenerateEntity(int node_count, Entity Entity_Matrix[], int range, double nonspatial_entity_ratio);
void GenerateEntity(int node_count, vector<Entity> &entity_vector, int range, double nonspatial_entity_ratio);


//Read entity from disk storage
void ReadEntityFromDisk(int &node_count, Entity Entity_Matrix[], int &range,string filename);
void ReadEntityFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename);

void ReadEntityInSCCFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename);
void ReadEntityInSCCSeperateFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename);
void EntityInSCCSeperate_To_Disk(vector<Entity> &entity_vector, int range, string filename);
void EntityInSCC_To_Disk(vector<Entity> &entity_vector, int range, string filename);

//Entity to new format
void EntityInSCCToNewFormat(int &node_count, vector<Entity> &entity_vector, int &range, string filename, string newfilename);

string getstring(const int i);
int StringtoInt(string str);
vector<string> split(string str, string pattern);

//Topological sort of dag
void TopologicalSortUtil(int v, vector<bool> &visited, queue<int> &Queue, vector<vector<int>> &graph);
void TopologicalSort(vector<vector<int>> &graph, queue<int> &Queue);

//Get in_edge graph
void GenerateInedgeGraph(vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph);

//GenerateGauss
double randomGauss(double mean, double sigma, TRnd &rand);
int randomSkewed(int p, double HsubV, TRnd &rand);

#endif