#include "stdafx.h"
#include "Real_Graph.h"

void ReadID(hash_map<int, int> &id_index, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	int id;
	int index = 0;
	while (true)
	{
		int count = scanf("%d\n", &id);
		if (count != 1)
			break;
		id_index[id] = index;
		index++;
	}
	fclose(stdin);
}
void ReadArbitaryGraphFromDisk(vector<vector<int>> &real_graph, hash_map<int, int> &id_index, string filename, int average_degree)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	real_graph.resize(id_index.size());
	for (int i = 0; i < id_index.size(); i++)
		real_graph[i].reserve(average_degree * 2);
	int start_id, end_id, start_index, end_index;
	while (true)
	{
		int count = scanf("%d %d", &start_id, &end_id);
		if (count != 2)
			break;
		start_index = id_index[start_id];
		end_index = id_index[end_id];
		real_graph[start_index].push_back(end_index);
	}
}
