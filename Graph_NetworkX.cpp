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
	fclose(stdin);
}

void ReadGraphFromEdge(vector<vector<int>> &graph, int &node_count, string filename, int average_degree)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d", &node_count);
	graph.resize(node_count);

	for (int i = 0; i < node_count; i++)
		graph[i].reserve(3 * average_degree);
	int start, end;
	while (true)
	{
		int count = scanf("%d %d", &start, &end);
		if (count == 2)
			graph[start].push_back(end);
		else
			break;
	}
	fclose(stdin);
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
	fclose(stdin);
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

void GenerateZipfEntityInSCC(int node_count, vector<Entity> &entity_vector, int range, double nonspatial_entity_ratio)
{
	TRnd Rnd1 = time(0);
	Sleep(20000);
	TRnd Rnd2 = time(0);
	Sleep(20000);
	TRnd Rnd3 = time(0);
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

	int i;
	float HsubV = 0.0;
	for (i = 1; i <= range*100; i++) {
		HsubV += 1.0 / pow((double)i, 1);
	}

	while (true)
	{
		int id = Rnd1.GetUniDev()*node_count;
		if (entity_vector[id].IsSpatial)
			continue;
		else
		{
			double x = randomSkewed(1, HsubV, Rnd2) / 100.0;
			double y = randomSkewed(1, HsubV, Rnd3) / 100.0;
			if (x < range && x>0 && y < range && y>0)
			{
				entity_vector[id].IsSpatial = true;
				entity_vector[id].location.x = x;
				entity_vector[id].location.y = y;
				spatial_count++;
			}
			
		}
		if (spatial_count == total)
			break;
	}
}

void GenerateClusteredEntityInSCC(int node_count, vector<Entity> &entity_vector, int range, double nonspatial_entity_ratio, vector<Location> centers, vector<double> sigmas, vector<double> proportion)
{
	entity_vector.resize(node_count);

	for (int i = 0; i < node_count; i++)
	{
		entity_vector[i].id = i;
		entity_vector[i].IsSpatial = false;

		entity_vector[i].location.x = -1;
		entity_vector[i].location.y = -1;

		entity_vector[i].type = 0;
	}

	for (int center_index = 0; center_index < centers.size(); center_index++)
	{
		Location center = centers[center_index];
		double sigma = sigmas[center_index];
		int total_count = node_count* (1 - nonspatial_entity_ratio) *proportion[center_index] + 0.5;
		TRnd Rnd = time(0);
		int spatial_count = 0;

		while (true)
		{
			int id = Rnd.GetUniDev()*node_count;
			if (entity_vector[id].IsSpatial)
				continue;
			else
			{
				
				double x = randomGauss(center.x, sigma, Rnd);
				double y = randomGauss(center.y, sigma, Rnd);
				if (x <= 1000 && x >= 0 && y <= 1000 && y >= 0)
				{
					spatial_count++;
					entity_vector[id].IsSpatial = true;
					entity_vector[id].location.x = x;
					entity_vector[id].location.y = y;
				}	
			}
			if (spatial_count == total_count)
				break;
		}
	}
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

void ArbitaryGraphEdgeToDisk(vector<vector<int>> &graph, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", graph.size());
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			printf("%d %d", i, graph[i][j]);
			printf("\n");
		}	
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
	fclose(stdin);
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
	fclose(stdin);
}

void GraphToNewformat(vector<vector<int>> &graph, int &node_count, string filename, string newfilename)
{
	ReadArbitaryGraphFromDisk(graph, node_count, filename);
	string root = "data/";
	root += newfilename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", node_count);

	for (int i = 0; i < node_count; i++)
	{
		printf("%d,%d", i, graph[i].size());
		for (int j = 0; j < graph[i].size(); j++)
			printf(",%d", graph[i][j]);
		printf("\n");
	}
	fclose(stdout);
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

void GenerateRMBRByQueue(vector<Entity> &p_entity, vector<set<int>> &p_graph)
{
	vector<vector<int>> in_edge_graph;
	in_edge_graph.resize(p_graph.size());
	for (int i = 0; i < p_graph.size(); i++)
	{
		set<int>::iterator end = p_graph[i].end();
		for (set<int>::iterator iter = p_graph[i].begin(); iter != end; iter++)
		{
			in_edge_graph[*iter].push_back(i);
		}
	}
	
	hash_set<int> hashset;
	queue<int> queue;
	for (int i = 0; i < p_entity.size(); i++)
	{
		p_entity[i].RMBR.left_bottom.x = -1;
		p_entity[i].RMBR.left_bottom.y = -1;
		p_entity[i].RMBR.right_top.x = -1;
		p_entity[i].RMBR.right_top.y = -1;

		if (p_entity[i].IsSpatial)
		{
			queue.push(i);
			hashset.insert(i);
		}
	}
	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();
		hashset.erase(id);
		for (int i = 0; i < in_edge_graph[id].size(); i++)
		{
			boolean changed = false;
			int in_neighbor = in_edge_graph[id][i];
			
			if (p_entity[id].IsSpatial)
			{
				MyRect rec;
				rec.left_bottom.x = p_entity[id].location.x;
				rec.left_bottom.y = p_entity[id].location.y;
				rec.right_top.x = p_entity[id].location.x;
				rec.right_top.y = p_entity[id].location.y;

				changed |= RecUnion(p_entity[in_neighbor].RMBR, rec);
			}
			changed |= RecUnion(p_entity[in_neighbor].RMBR, p_entity[id].RMBR);

			if (changed)
			{
				if (hashset.find(in_neighbor) == hashset.end())
				{
					hashset.insert(in_neighbor);
					queue.push(in_neighbor);
				}
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

void GetTransitiveClosureDynamicByMatrix()
{
	vector<vector<bool>> transitive_closure;
	char *ch = "data/RMBR/DAG/18_16/dag.txt";
	freopen(ch, "r", stdin);

	int node_count;
	scanf("%d\n", &node_count);
	transitive_closure.resize(node_count);
	for (int i = 0; i < node_count; i++)
	{
		transitive_closure[i].resize(node_count);
	}
	for (int i = 0; i < node_count; i++)
	{
		int id, count;
		scanf("%d %d ", &id, &count);
		for (int j = 0; j < count; j++)
		{
			int neighbor;
			scanf("%d ", &neighbor);
			transitive_closure[i][neighbor] = true;
		}
	}

	fclose(stdin);

	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i = 0; i < node_count; i++)
		{
			for (int j = i; j < node_count; j++)
			{
				if (transitive_closure[i][j])
				{
					for (int k = j; k < node_count; k++)
					{
						if (transitive_closure[i][k])
							continue;
						else
							if (transitive_closure[j][k])
							{
								transitive_closure[i][k] = true;
								changed = true;
							}
					}
				}
			}
		}
	}

	char *ch2 = "data/RMBR/DAG/18_18/transitive_closure.txt";
	freopen(ch2, "w", stdout);
	printf("%d %d\n", node_count, 1000);

	for (int i = 0; i < node_count; i++)
	{
		int count = 0;
		for (int j = 0; j < node_count; j++)
		{
			if (transitive_closure[i][j])
				count++;
		}
		printf("%d %d ", i, count);
		for (int j = 0; j < node_count; j++)
		{
			if (transitive_closure[i][j])
				printf("%d ", j);
		}
		printf("\n");
	}
	fclose(stdout);
}

void GenerateDAG(vector<set<int>> &dag, int node_count, INT64 edge_count)
{
	int count = 0;
	dag.resize(node_count);

	TRnd Rnd = time(0);
	int i, j;

	while (true)
	{
		i = Rnd.GetUniDev()*node_count;
		j = Rnd.GetUniDev()*node_count;
		if (i == j)
			continue;
		if (i > j)
		{
			int a = i;
			i = j;
			j = a;
		}
		int currentsize = dag[i].size();
		dag[i].insert(j);
		if (currentsize == dag[i].size())
			continue;
		count++; 
		if (count == edge_count)
			break;
	}
}

void GenerateDAG(vector<vector<int>> &dag, int node_count, INT64 edge_count)
{
	int count = 0;
	dag.resize(node_count);

	int ratio = edge_count / node_count;

	for (int i = 0; i < node_count; i++)
	{
		dag[i].reserve(ratio * 3);
	}

	TRnd Rnd = time(0);
	int i, j;

	for (int k = 0; k < edge_count;k++)
	{
		i = Rnd.GetUniDev()*node_count;
		j = Rnd.GetUniDev()*node_count;
		if (i == j)
			continue;
		if (i > j)
		{
			int a = i;
			i = j;
			j = a;
		}
		int currentsize = dag[i].size();
		dag[i].push_back(j);
	}
}

/*
void GenerateDAG(vector<set<int>> &dag, vector<vector<int>> &node_distribution, int node_count, int level_count, int average_degree)
{
	node_distribution.resize(level_count);
	node_distribution.reserve(node_count / level_count *1.5);
	TRnd Rnd = time(0);
	for (int i = 0; i < node_count; i++)
	{
		int i_level = Rnd.GetUniDev() *level_count;
		node_distribution[i_level].push_back(i);
	}

	for (int i = 1; i < level_count - 1; i++)
	{
		int higher_count = 0, lower_count = 0;
		for (int j = 0; j < i; j++)
			higher_count += node_distribution[j].size();
		for (int j = i + 1; j < level_count; j++)
			lower_count += node_distribution[j].size();

		for (int j = 0; j < node_distribution[i].size(); j++)
		{
			int start_index = Rnd.GetUniDev()*higher_count;
			for (int k = 0; k < i; k++)
			{
				if (start_index>node_distribution[k].size())
				{
					start_index -= node_distribution[k].size();
					continue;
				}
				else

			}
		}
	}

}*/

vector<int> GetTransitiveClosureLineArbitary(int i, vector<vector<int>> &graph)
{
	int node_count = graph.size();
	vector<bool> isvisited;
	isvisited.resize(node_count);

	vector<int> transitiveclosure_line;
	transitiveclosure_line.reserve(node_count);

	TraverseArbitary(i, graph, isvisited, transitiveclosure_line);

	return transitiveclosure_line;
}

vector<int> GetTransitiveClosureLineArbitary(int i, vector<set<int>> &graph)
{
	int node_count = graph.size();
	vector<bool> isvisited;
	isvisited.resize(node_count);

	vector<int> transitiveclosure_line;
	transitiveclosure_line.reserve(node_count);

	TraverseArbitary(i, graph, isvisited, transitiveclosure_line);

	return transitiveclosure_line;
}

void TraverseArbitary(int id, vector<vector<int>> &graph, vector<bool> &isvisted, vector<int> &reach_vertices)
{
	for (int i = 0; i < graph[id].size(); i++)
	{
		int out_id = graph[id][i];
		if (!isvisted[out_id])
		{
			isvisted[out_id] = true;
			reach_vertices.push_back(out_id);
			TraverseArbitary(out_id, graph, isvisted, reach_vertices);
		}
	}
}

void TraverseArbitary(int id, vector<set<int>> &graph, vector<bool> &isvisted, vector<int> &reach_vertices)
{
	set<int>::iterator end = graph[id].end();

	for (set<int>::iterator iter = graph[id].begin(); iter != end; iter++)
	{
		int out_id = *iter;
		if (!isvisted[out_id])
		{
			isvisted[out_id] = true;
			reach_vertices.push_back(out_id);
			TraverseArbitary(out_id, graph, isvisted, reach_vertices);
		}
	}
}

void ReadTransitiveClosureFromDisk(vector<vector<int>> &transitive_closure, int node_count, string filepath)
{
	string m_path = "data/" + filepath;
	char* ch = (char*)m_path.data();
	freopen(ch, "r", stdin);
	transitive_closure.resize(node_count);
	for (int i = 0; i < node_count; i++)
	{
		int start_id, count;
		scanf("%d %d ", &start_id, &count);
		transitive_closure[i].reserve(count);
		for (int j = 0; j < count; j++)
		{
			int end_id;
			scanf("%d ", &end_id);
			transitive_closure[i].push_back(end_id);
		}
	}
}

bool Reach(int src, int end, vector<vector<int>> &graph)
{
	if (src == end)
		return true;
	vector<bool> isvisited = vector<bool>(graph.size());
	isvisited[src] = true;
	queue<int> q;
	q.push(src);
	while (!q.empty())
	{
		int head = q.front();
		q.pop();
		for (int i = 0; i < graph[head].size(); i++)
		{
			int out_neighbor = graph[head][i];
			if (isvisited[out_neighbor])
				continue;
			else
			{
				if (out_neighbor == end)
					return true;
				else
				{
					q.push(out_neighbor);
					isvisited[out_neighbor] = true;
				}
			}
		}
	}
	return false;
}
