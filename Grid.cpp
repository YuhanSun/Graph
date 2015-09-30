#include "stdafx.h"
#include "Grid.h"

void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<set<int>> &index)
{
	//get in-edge graph
	vector<vector<int>> in_edge_graph;
	in_edge_graph.resize(graph.size());
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			int neighbor = graph[i][j];
			in_edge_graph[neighbor].push_back(i);
		}
	}

	index.resize(p_entity.size());
	hash_map<int, int> grid_assign;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;


	queue<int> queue;
	hash_set<int> hashset;
	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			int grid_id = m*pieces_x + n;
			grid_assign[i] = grid_id;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				index[in_neighbor].insert(grid_id);
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
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

			//grid union
			int size = index[in_neighbor].size();
			set<int>::iterator end = index[id].end();
			for (set<int>::iterator iter = index[id].begin(); iter != end; iter++)
			{
				index[in_neighbor].insert(*iter);
			}
			if (index[in_neighbor].size() != size)
				changed = true;//end of grid union

			if (changed)
			{
				if (hashset.find(in_neighbor)==hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
		}
	}
}

void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, vector<vector<bool>> &update)
{
	int grid_count = pieces_x*pieces_y;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	queue<int> queue;
	hash_set<int> hashset;

	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			int grid_id = m*pieces_x + n;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				index[in_neighbor][grid_id] = true;
				update[in_neighbor][grid_id] = true;
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
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

			//grid union
			for (int j = 0; j < update[id].size(); j++)
			{
				if (update[id][j]&&(!index[in_neighbor][j]))
				{
					index[in_neighbor][j] = true;
					update[in_neighbor][j] = true;
					changed = true;
				}
			}

			if (changed)
			{
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
		}
		for (int i = 0; i < update[id].size(); i++)
			update[id][i] = false;
	}
}

void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, vector<set<int>> &update)
{
	int grid_count = pieces_x*pieces_y;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	queue<int> queue;
	hash_set<int> hashset;

	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			int grid_id = m*pieces_x + n;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				index[in_neighbor][grid_id] = true;
				update[in_neighbor].insert(grid_id);
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
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

			//grid union
			set<int>::iterator end = update[id].end();
			for (set<int>::iterator iter = update[id].begin(); iter != end; iter++)
			{
				if (!index[in_neighbor][*iter])
				{
					index[in_neighbor][*iter] = true;
					update[in_neighbor].insert(*iter);
					changed = true;
				}
			}

			if (changed)
			{
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
		}
		update[id].clear();
		update[id].swap(update[id]);
	}
}

void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, vector<vector<int>> &update)
{
	int grid_count = pieces_x*pieces_y;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	queue<int> queue;
	hash_set<int> hashset;

	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			int grid_id = m*pieces_x + n;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				index[in_neighbor][grid_id] = true;
				update[in_neighbor].push_back(grid_id);
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
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

			//grid union
			for (int j = 0; j < update[id].size(); j++)
			{
				int grid_id = update[id][j];
				if (!index[in_neighbor][grid_id])
				{
					index[in_neighbor][grid_id] = true;
					/*boolean flag = false;
					for (int k = 0; k < update[in_neighbor].size(); k++)
					{
						if (update[in_neighbor][k] == grid_id)
						{
							flag = true;
							break;
						}
					}
					if (!flag)
						update[in_neighbor].push_back(grid_id);*/
					update[in_neighbor].push_back(grid_id);
					changed = true;
				}
			}

			if (changed)
			{
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
		}
		update[id].clear();
		vector<int> clear;
		update[id].swap(clear);
	}
}


void GenerateGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<vector<bool>> &index)
{
	int grid_count = pieces_x*pieces_y;

	//get in-edge graph
	vector<vector<int>> in_edge_graph;
	in_edge_graph.resize(graph.size());
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			int neighbor = graph[i][j];
			in_edge_graph[neighbor].push_back(i);
		}
	}

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;


	queue<int> queue;
	hash_set<int> hashset;
	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			int grid_id = m*pieces_x + n;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				index[in_neighbor][grid_id] = true;
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
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

			//grid union
			for (int j = 0; j < grid_count; j++)
			{
				if ((!index[in_neighbor][j]) && index[id][j])
				{
					index[in_neighbor][j] = true;
					changed = true;
				}
			}

			if (changed)
			{
				if (hashset.find(in_neighbor) == hashset.end())
				{
					queue.push(in_neighbor);
					hashset.insert(in_neighbor);
				}
			}
		}
	}
}

void GenerateGridPointIndexNotInQueue(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &graph, vector<Entity> &p_entity, vector<set<int>> &index)
{
	index.resize(p_entity.size());
	hash_map<int, int> grid_assign;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			grid_assign[i] = m*pieces_x + n;
		}
	}

	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			int out_neighbor = graph[i][j];
			if (p_entity[out_neighbor].IsSpatial)
			{
				index[i].insert(grid_assign[out_neighbor]);
			}
		}
	}

	bool changed = true;

	while (changed)
	{
		changed = false;
		for (int i = 0; i < graph.size(); i++)
		{
			for (int j = 0; j < graph[i].size(); j++)
			{
				int neighbor_id = graph[i][j];
				set<int>::iterator end = index[neighbor_id].end();
				int size = index[i].size();
				for (set<int>::iterator iter = index[neighbor_id].begin(); iter != end; iter++)
				{
					index[i].insert(*iter);
				}
				if (size != index[i].size())
					changed = true;
			}
		}
	}
}

void GridPointIndexToDiskSkip(vector<vector<bool>> &index, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", index.size());

	for (int i = 0; i < index.size(); i++)
	{
		int count = 0;
		for (int j = 0; j < index[i].size(); j++)
		{
			if (index[i][j])
				count++;
		}

		if (count == 0)
			continue;

		printf("%d %d ", i, count);
		for (int j = 0; j < index[i].size(); j++)
		{
			if (index[i][j])
				printf("%d ", j);
		}
		printf("\n");
	}
	fclose(stdout);
}

void GridPointIndexToDiskSkip(vector<set<int>> &index, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", index.size());

	for (int i = 0; i < index.size(); i++)
	{
		int count = index[i].size();

		if (count == 0)
			continue;

		printf("%d %d ", i, count);
		set<int>::iterator end = index[i].end();
		for (set<int>::iterator iter = index[i].begin(); iter != end;iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	fclose(stdout);
}

void GridPointIndexToDisk(vector<vector<bool>> &index, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", index.size());

	for (int i = 0; i < index.size(); i++)
	{
		int count = 0;
		for (int j = 0; j < index[i].size(); j++)
		{
			if (index[i][j])
				count++;
		}

		printf("%d %d ", i, count);
		for (int j = 0; j < index[i].size(); j++)
		{
			if (index[i][j])
				printf("%d ", j);
		}
		printf("\n");
	}
	fclose(stdout);
}

void GridPointIndexToDisk(vector<set<int>> &index, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", index.size());

	for (int i = 0; i < index.size(); i++)
	{
		int count = index[i].size();

		printf("%d %d ", i, count);
		set<int>::iterator end = index[i].end();
		for (set<int>::iterator iter = index[i].begin(); iter != end; iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	fclose(stdout);
}

// A recursive function used by topologicalSort
void TopologicalSortUtil(int v, vector<bool> &visited, queue<int> &queue, vector<vector<int>> &graph)
{
	// Mark the current node as visited.
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	for (int i = 0; i < graph[v].size();i++)
		if (!visited[graph[v][i]])
			TopologicalSortUtil(graph[v][i], visited, queue, graph);

	// Push current vertex to stack which stores result
	queue.push(v);
}

// The function to do Topological Sort. It uses recursive topologicalSortUtil()
void TopologicalSort(vector<vector<int>> &graph, queue<int> &queue)
{
	// Mark all the vertices as not visited
	vector<bool> visited = vector<bool>(graph.size());

	for (int i = 0; i < graph.size(); i++)
		visited[i] = false;

	// Call the recursive helper function to store Topological Sort
	// starting from all vertices one by one
	for (int i = 0; i < graph.size(); i++)
		if (visited[i] == false)
			TopologicalSortUtil(i, visited, queue, graph);
}

void GenerateGridIndexSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue)
{
	int grid_count = pieces_x*pieces_y;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	vector<bool> IsUpdate = vector<bool>(in_edge_graph.size());

	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			int grid_id = m*pieces_x + n;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				index[in_neighbor][grid_id] = true;
				IsUpdate[in_neighbor] = true;
			}
		}
	}

	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();
		if (IsUpdate[id])
		{
			for (int i = 0; i < in_edge_graph[id].size(); i++)
			{
				int in_neighbor = in_edge_graph[id][i];

				//grid union
				for (int j = 0; j < grid_count; j++)
				{
					if (index[id][j])
					{
						index[in_neighbor][j] = true;
					}
				}
			}
		}
	}
}

void ReadGridPointIndexFromDisk(int total_pieces, vector<vector<bool>> &index, string filename)
{
	string root = "data/";
	root += (filename);
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	int node_count;
	scanf("%d\n", &node_count);

	index.resize(node_count);
	for (int i = 0; i < node_count; i++)
		index[i].resize(total_pieces);

	for (int i = 0; i < node_count; i++)
	{
		int id, count;
		scanf("%d %d ", &id, &count);
		if (count == 0)
			continue;
		for (int j = 0; j < count; j++)
		{
			int reachgridid;
			scanf("%d ", &reachgridid);
			index[i][reachgridid] = true;
		}
	}
}