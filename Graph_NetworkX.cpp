#include "stdafx.h"
#include "Graph_NetworkX.h"

void ReadGraphFromEdge(vector<set<int>> &graph, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	int start, end;
	while (true)
	{
		int count = scanf("%d %d {}", &start, &end);
		if (count == 2)
			graph[start].insert(end);
		else
			break;
	}
}

void ReadSCC(vector<set<int>> &SCC, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	int entity_id;
	int scc_count;

	scanf("%d\n", &scc_count);
	SCC.resize(scc_count);

	int i = 0;
	while (true)
	{
		char c = getchar();
		if (c != '[')
			break;
		scanf("%d", &entity_id);
		SCC[i].insert(entity_id);
		c = getchar();
		while (c == ',')
		{
			scanf(" %d", &entity_id);
			SCC[i].insert(entity_id);
			c = getchar();
		}
		i++;
		c = getchar();
	}
}

void ConnectSCCEntity(vector<Entity> &entity, vector<set<int>> &SCC)
{
	for (int i = 0; i < SCC.size(); i++)
	{
		set<int>::iterator end = SCC[i].end();
		for (set<int>::iterator iter = SCC[i].begin(); iter != end; iter++)
		{
			int entity_id = *iter;
			entity[entity_id].scc_id = i;
		}
	}
}

void GenerateSCCGraph (vector<Entity> &entity, vector<set<int>> &graph_entity, vector<set<int>> &graph_SCC)
{
	for (int i = 0; i < graph_entity.size(); i++)
	{
		int start_scc_id = entity[i].scc_id;
		set<int>::iterator end = graph_entity[i].end();
		for (set<int>::iterator iter = graph_entity[i].begin(); iter != end; iter++)
		{
			int end_graph_id = *iter;
			int end_scc_id = entity[end_graph_id].scc_id;
			if (start_scc_id == end_scc_id)
				continue;
			graph_SCC[start_scc_id].insert(end_scc_id);
		}
	}
}

void GenerateSCCGraph(vector<Entity> &entity, vector<vector<int>> &graph_entity, vector<set<int>> &graph_SCC)
{
	for (int i = 0; i < graph_entity.size(); i++)
	{
		int start_scc_id = entity[i].scc_id;
		for (int j = 0; j < graph_entity[i].size(); j++)
		{
			int end_graph_id = graph_entity[i][j];
			int end_scc_id = entity[end_graph_id].scc_id;
			if (start_scc_id == end_scc_id)
				continue;
			graph_SCC[start_scc_id].insert(end_scc_id);
		}
	}
}

void Traverse(int id, vector<set<int>> &graph, vector<bool> &isvisted, vector<int> &reach_vertices)
{
	set<int>::iterator end = graph[id].end();
	for (set<int>::iterator iter = graph[id].begin(); iter != end; iter++)
	{
		int out_id = *iter;
		if (!isvisted[out_id])
		{
			isvisted[out_id] = true;
			reach_vertices.push_back(out_id);
			Traverse(out_id, graph, isvisted, reach_vertices);
		}
	}
}

vector<vector<int>> GetTransitiveClosure(vector<set<int>> &graph)
{
	int node_count = graph.size();

	vector<bool> isvisited;
	isvisited.resize(node_count);

	vector<vector<int>> transitive_closure;
	transitive_closure.resize(node_count);
	for (int i = 0; i < node_count; i++)
	{
		for (int j = 0; j < node_count; j++)
			isvisited[j] = false;

		transitive_closure[i].reserve(10);
		Traverse(i, graph, isvisited, transitive_closure[i]);
	}
	return transitive_closure;
}

void GenerateEntityInSCC(int node_count, vector<Entity> &entity_vector, int range, double nonspatial_entity_ratio)
{
	TRnd Rnd = time(0);
	entity_vector.resize(node_count);
	int spatial_count = 0;
	int total = node_count * (1 - nonspatial_entity_ratio);

	for (int i = 0; i < node_count; i++)
	{
		entity_vector[i].id = i;
		entity_vector[i].IsSpatial = false;

		entity_vector[i].location.x = -1;
		entity_vector[i].location.y = -1;

		entity_vector[i].type = 0;
	}

	while (true)
	{
		int id = Rnd.GetUniDev()*node_count;
		if (entity_vector[id].IsSpatial)
			continue;
		else
		{
			spatial_count++;
			entity_vector[id].IsSpatial = true;
			entity_vector[id].location.x = Rnd.GetUniDev() * range;
			entity_vector[id].location.y = Rnd.GetUniDev() * range;
		}
		if (spatial_count == total)
			break;
	}
}

void ReadEntityInSCCFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d %d", &node_count, &range);
	entity_vector.resize(node_count);
	for (int i = 0; i < node_count; i++)
	{
		scanf("%d %d %lf %lf %d %d %lf %lf %lf %lf", &(entity_vector[i].id), &(entity_vector[i].IsSpatial), &(entity_vector[i].location.x), &(entity_vector[i].location.y), &(entity_vector[i].type), &(entity_vector[i].scc_id), &(entity_vector[i].RMBR.left_bottom.x), &(entity_vector[i].RMBR.left_bottom.y), &(entity_vector[i].RMBR.right_top.x), &(entity_vector[i].RMBR.right_top.y));
	}
	fclose(stdin);
}

void EntityInSCC_To_Disk(vector<Entity> &entity_vector, int range, string filename)
{
	string root = "data/";
	root += (filename + "/spatial_entity.txt");
	char *ch1 = (char *)root.data();
	freopen(ch1, "w", stdout);

	printf("%d %d\n", entity_vector.size(), range);
	for (int i = 0; i < entity_vector.size(); i++)
	{
		if (entity_vector[i].IsSpatial)
			printf("%d %d %f %f %d %d %lf %lf %lf %lf\n", entity_vector[i].id, entity_vector[i].IsSpatial, entity_vector[i].location.x, entity_vector[i].location.y, entity_vector[i].type, entity_vector[i].scc_id, entity_vector[i].RMBR.left_bottom.x, entity_vector[i].RMBR.left_bottom.y, entity_vector[i].RMBR.right_top.x, entity_vector[i].RMBR.right_top.y);
	}
	fclose(stdout);

	root = "data/";
	root += (filename + "/nonspatial_entity.txt");
	char *ch2 = (char *)root.data();
	freopen(ch2, "w", stdout);

	for (int i = 0; i < entity_vector.size(); i++)
	{
		if (!entity_vector[i].IsSpatial)
			printf("%d %d %f %f %d %d %lf %lf %lf %lf\n", entity_vector[i].id, entity_vector[i].IsSpatial, entity_vector[i].location.x, entity_vector[i].location.y, entity_vector[i].type, entity_vector[i].scc_id, entity_vector[i].RMBR.left_bottom.x, entity_vector[i].RMBR.left_bottom.y, entity_vector[i].RMBR.right_top.x, entity_vector[i].RMBR.right_top.y);
	}
	fclose(stdout);
}

void AddEdge(vector<set<int>> &graph, int start, int end)
{
	graph[start].insert(end);
}

void AddEdge(vector<vector<int>> &graph, int start, int end)
{
	graph[start].push_back(end);
}

void GenerateArbitaryGraph(vector<set<int>> &graph, int node_count, INT64 edge_count)
{
	int count = 0;
	graph.resize(node_count);

	TRnd Rnd = time(0);
	int i, j;

	while (true)
	{
		i = Rnd.GetUniDev()*node_count;
		j = Rnd.GetUniDev()*node_count;
		if (i == j)
			continue;
		AddEdge(graph, i, j);
		count++;
		if (count == edge_count)
			break;
	}
}

void GenerateArbitaryGraph(vector<vector<int>> &graph, int node_count, INT64 edge_count)
{
	int count = 0;
	graph.resize(node_count);

	int ratio = edge_count / node_count;
	for (int i = 0; i < node_count; i++)
	{
		graph[i].reserve(3 * ratio);
	}

	TRnd Rnd = time(0);
	int i, j;

	while (true)
	{
		i = Rnd.GetUniDev()*node_count;
		j = Rnd.GetUniDev()*node_count;
		if (i == j)
			continue;
		AddEdge(graph, i, j);
		count++;
		if (count == edge_count)
			break;
	}
}

void GenerateArbitaryGraph(vector<vector<int>> &graph, int node_count, INT64 edge_count, double a, double b, double c)
{
	int count = 0;
	int k = log2(node_count);
	node_count = pow(2, k);

	graph.resize(node_count);
	int ratio = edge_count / node_count;
	for (int i = 0; i < node_count; i++)
	{
		graph[i].reserve(ratio * 3);
	}

	TRnd Rnd = time(0);

	for (INT64 m = 0; m < edge_count; m++)
	{

		int i = 0, j = 0;
		for (INT64 t = 0; t < k; t++)
		{
			double prob = Rnd.GetUniDev();
			if (prob >= a&&prob < (a + b))
				j = j + pow(2, k - 1 - t);
			else
			{
				if (prob >= (a + b) && prob < (a + b + c))
				{
					i = i + pow(2, k - 1 - t);
				}
				else
				{
					if (prob >= (a + b + c))
					{
						i = i + pow(2, k - 1 - t);
						j = j + pow(2, k - 1 - t);
					}
				}
			}
		}
		if (i == j)
			continue;
		count += 1;
		AddEdge(graph, i, j);
	}
}

void ArbitaryGraphToDisk(vector<set<int>> &graph, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", graph.size());
	for (int i = 0; i < graph.size(); i++)
	{
		printf("%d %d ", i, graph[i].size());
		set<int>::iterator end = graph[i].end();
		for (set<int>::iterator iter = graph[i].begin(); iter != end; iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	fclose(stdout);
}


void ArbitaryGraphToDisk(vector<vector<int>> &graph, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", graph.size());
	for (int i = 0; i < graph.size(); i++)
	{
		printf("%d %d ", i, graph[i].size());
		for (int j = 0; j < graph[i].size(); j++)
		{
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}
	fclose(stdout);
}

void ReadArbitaryGraphFromDisk(vector<set<int>> &graph, int &node_count, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d\n", &node_count);
	graph.resize(node_count);

	for (int i = 0; i < node_count; i++)
	{
		int id, count, outid;
		scanf("%d %d ", &id, &count);
		for (int j = 0; j < count; j++)
		{
			scanf("%d ", &outid);
			graph[i].insert(outid);
		}
	}
}

void ReadArbitaryGraphFromDisk(vector<vector<int>> &graph, int &node_count, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d\n", &node_count);
	graph.resize(node_count);

	for (int i = 0; i < node_count; i++)
	{
		int id, count, outid;
		scanf("%d %d ", &id, &count);
		graph[i].resize(count);
		for (int j = 0; j < count; j++)
		{
			scanf("%d ", &outid);
			graph[i][j] = outid;
		}
	}
}

void GenerateRMBR(vector<Entity> &p_entity, vector<set<int>> &p_graph)
{
	bool changed = true;
	for (int i = 0; i < p_entity.size(); i++)
	{
		p_entity[i].RMBR.left_bottom.x = -1;
		p_entity[i].RMBR.left_bottom.y = -1;
		p_entity[i].RMBR.right_top.x = -1;
		p_entity[i].RMBR.right_top.y = -1;
	}
	while (changed)
	{
		changed = false;
		for (int i = 0; i < p_entity.size(); i++)
		{
			set<int>::iterator end = p_graph[i].end();
			for (set<int>::iterator iter = p_graph[i].begin(); iter != end; iter++)
			{
				int neibour_id = *iter;
				if (p_entity[neibour_id].IsSpatial)
				{
					MyRect rec;
					rec.left_bottom.x = p_entity[neibour_id].location.x;
					rec.left_bottom.y = p_entity[neibour_id].location.y;
					rec.right_top.x = p_entity[neibour_id].location.x;
					rec.right_top.y = p_entity[neibour_id].location.y;

					changed |= RecUnion(p_entity[i].RMBR, rec);
				}
				changed |= RecUnion(p_entity[i].RMBR, p_entity[neibour_id].RMBR);
			}
		}
	}
}

vector<set<int>> GetTransitiveClosureDynamic_In_Set(vector<set<int>> &p_graph)
{
	int p_node_count = p_graph.size();

	vector<set<int>> transitive_closure;
	transitive_closure.resize(p_node_count);

	for (int i = 0; i < p_graph.size(); i++)
	{
		set<int>::iterator end = p_graph[i].end();
		for (set<int>::iterator iter = p_graph[i].begin(); iter != end; iter++)
		{
			int neighbor_id = *iter;
			transitive_closure[i].insert(neighbor_id);
		}
	}

	bool changed = true;

	while (changed)
	{
		changed = false;

		for (int i = 0; i < p_graph.size(); i++)
		{
			set<int>::iterator iter_line = transitive_closure[i].begin();
			set<int>::iterator end_line = transitive_closure[i].end();

			for (; iter_line != end_line; iter_line++)
			{
				int neighbor = *iter_line;

				set<int>::iterator iter_neighbor = transitive_closure[neighbor].begin();
				set<int>::iterator end_neighbor = transitive_closure[neighbor].end();

				for (; iter_neighbor != end_neighbor; iter_neighbor++)
				{
					int reach_node = *iter_neighbor;
					if (transitive_closure[i].find(reach_node) == transitive_closure[i].end())
					{
						changed = true;
						transitive_closure[i].insert(reach_node);
					}
				}
			}
		}
	}
	return transitive_closure;
}

vector<vector<int>> GetTransitiveClosureDynamicNew(vector<vector<int>> &p_graph)
{
	int p_node_count = p_graph.size();

	vector<vector<int>> transitive_closure;
	transitive_closure.resize(p_node_count);

	for (int i = 0; i < p_graph.size(); i++)
	{
		transitive_closure[i].reserve(p_graph[i].size()*6);
		for (int j = 0; j < p_graph[i].size(); j++)
		{
			transitive_closure[i].push_back(p_graph[i][j]);
		}
	}

	bool changed = true;

	while (changed)
	{
		changed = false;

		for (int i = 0; i < p_graph.size(); i++)
		{
			hash_set<int> current_tran;
			hash_set<int>::iterator end = current_tran.end();
			for (int j = 0; j < transitive_closure[i].size(); j++)
				current_tran.insert(transitive_closure[i][j]);
			for (int j = 0; j < p_graph[i].size(); j++)
			{
				int reach_node = p_graph[i][j];
				for (int k = 0; k < transitive_closure[reach_node].size(); k++)
				{
					int addin = transitive_closure[reach_node][k];
					if (current_tran.find(addin) == end)
					{
						transitive_closure[i].push_back(addin);
						changed = true;
					}
				}
			}
		}
	}
	return transitive_closure;
}