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

void GenerateGridPointIndexPartialSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored)
{
	int grid_count = pieces_x*pieces_y;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	vector<int> ReachCount = vector<int>(in_edge_graph.size());
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
				if (index[in_neighbor][grid_id] == false)
				{
					index[in_neighbor][grid_id] = true;
					ReachCount[in_neighbor] += 1;
					if (ReachCount[in_neighbor]>threshold)
						IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}		
			}
		}
	}

	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();
		if (IsUpdate[id])
		{
			if (IsStored[id])
			{
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];

					//grid union
					for (int j = 0; j < grid_count; j++)
					{
						if (index[id][j] && (!index[in_neighbor][j]))
						{
							IsUpdate[in_neighbor] = true;
							index[in_neighbor][j] = true;
							ReachCount[in_neighbor] += 1;
							if (ReachCount[in_neighbor] > threshold)
								IsStored[in_neighbor] = false;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];
					IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
		else
			IsStored[id] = false;
	}
}

void GenerateGridPointIndexPartialSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored)
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
				set<int>::iterator end = index[in_neighbor].end();
				if (index[in_neighbor].find(grid_id) == end)
				{
					index[in_neighbor].insert(grid_id);
					if (index[in_neighbor].size()>threshold)
						IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
	}

	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();
		if (IsUpdate[id])
		{
			if (IsStored[id])
			{
				set<int>::iterator id_end = index[id].end();
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];
					for (set<int>::iterator iter = index[id].begin(); iter != id_end; iter++)
					{
						//grid union
						if (index[in_neighbor].find(*iter) == index[in_neighbor].end())
						{
							IsUpdate[in_neighbor] = true;
							index[in_neighbor].insert(*iter);
							if (index[in_neighbor].size() > threshold)
								IsStored[in_neighbor] = false;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];
					IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
		else
			IsStored[id] = false;
	}
}

void GenerateMultilevelGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored, int merge_count)
{
	int grid_count = pieces_x*pieces_y;

	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	vector<int> ReachCount = vector<int>(in_edge_graph.size());
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
				if (index[in_neighbor][grid_id] == false)
				{
					index[in_neighbor][grid_id] = true;
					ReachCount[in_neighbor] += 1;
					if (ReachCount[in_neighbor]>threshold)
						IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
	}

	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();
		if (IsUpdate[id])
		{
			if (IsStored[id])
			{
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];

					//grid union
					for (int j = 0; j < grid_count; j++)
					{
						if (index[id][j] && (!index[in_neighbor][j]))
						{
							IsUpdate[in_neighbor] = true;
							index[in_neighbor][j] = true;
							ReachCount[in_neighbor] += 1;
							if (ReachCount[in_neighbor] > threshold)
								IsStored[in_neighbor] = false;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];
					IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
		else
			IsStored[id] = false;
	}

	int offset = 0;
	for (int i = pieces_x; i >=2 ; i/=2)
	{
		offset += i*i;
		for (int id = 0; id < in_edge_graph.size(); id++)
		{
			if (IsStored[id])
			{
				for (int m = 0; m < i; m += 2)
				{
					for (int n = 0; n < i; n += 2)
					{
						int grid_id = m*i + n + offset - i*i;
						int true_count = 0;
						if (index[id][grid_id])
							true_count++;
						if (index[id][grid_id + 1])
							true_count++;
						if (index[id][grid_id + i])
							true_count++;
						if (index[id][grid_id + i + 1])
							true_count++;
						if (true_count >= merge_count)
						{
							int mm = m / 2, nn = n / 2;
							int high_level_grid_id = mm*i / 2 + nn;
							index[id][high_level_grid_id + offset] = true;
							index[id][grid_id] = false;
							index[id][grid_id + 1] = false;
							index[id][grid_id + i] = false;
							index[id][grid_id + i + 1] = false;
						}
					}
				}
			}			
		}
	}
}

int Return_resolution(int grid_id)
{
	if (grid_id < 16384)
		return 128;
	else
	{
		if (grid_id < 20480)
			return 64;
		else
		{
			if (grid_id < 21504)
				return 32;
			else
			{
				if (grid_id < 21760)
					return 16;
				else
				{
					if (grid_id < 21824)
						return 8;
					else
					{
						if (grid_id < 21840)
							return 4;
						else
							return 2;
					}
				}
			}
		}
	}
}

int Return_Offset(int pieces)
{
	if (pieces == 128)
		return 0;
	if (pieces == 64)
		return 16384;
	if (pieces == 32)
		return 20480;
	if (pieces == 16)
		return 21504;
	if (pieces == 8)
		return 21760;
	if (pieces == 4)
		return 21824;
	if (pieces == 2)
		return 21840;
}

void GenerateMultilevelGridPointIndex(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue, int threshold, vector<bool> &IsStored, int merge_count)
{

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
				set<int>::iterator end = index[in_neighbor].end();
				if (index[in_neighbor].find(grid_id) == end)
				{
					index[in_neighbor].insert(grid_id);
					if (index[in_neighbor].size()>threshold)
						IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
	}

	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();
		if (IsUpdate[id])
		{
			if (IsStored[id])
			{
				set<int>::iterator id_end = index[id].end();
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];
					for (set<int>::iterator iter = index[id].begin(); iter != id_end; iter++)
					{
						//grid union
						if (index[in_neighbor].find(*iter) == index[in_neighbor].end())
						{
							IsUpdate[in_neighbor] = true;
							index[in_neighbor].insert(*iter);
							if (index[in_neighbor].size() > threshold)
								IsStored[in_neighbor] = false;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < in_edge_graph[id].size(); i++)
				{
					int in_neighbor = in_edge_graph[id][i];
					IsStored[in_neighbor] = false;
					IsUpdate[in_neighbor] = true;
				}
			}
		}
		else
			IsStored[id] = false;
	}


	for (int j = 0; j < in_edge_graph.size(); j++)
	{
		if (IsStored[j])
		{
			for (set<int>::iterator iter = index[j].begin(); iter != index[j].end();)
			{
				int reach_grid_id = *iter;
				int pieces = Return_resolution(reach_grid_id);
				int offset = Return_Offset(pieces);
				int id = reach_grid_id - offset;
				int m = id / pieces, n = id - m*pieces;
				int mm = m / 2, nn = n / 2;
				m = mm * 2, n = nn * 2;
				int base = m*pieces + n + offset;

				int true_count = 0;
				set<int>::iterator end = index[j].end();
				if (index[j].find(base) != end)
					true_count++;
				if (index[j].find(base + 1) != end)
					true_count++;
				if (index[j].find(base + pieces) != end)
					true_count++;
				if (index[j].find(base + pieces + 1) != end)
					true_count++;
				if (true_count >= merge_count)
				{
					while ((*iter == base || *iter == base + 1 || *iter == base + pieces + 1 || *iter == base + pieces) && iter != end)
					{
						iter++;
					}
					index[j].erase(base);
					index[j].erase(base + 1);
					index[j].erase(base + pieces);
					index[j].erase(base + pieces + 1);
					index[j].insert(offset + pieces*pieces + mm*pieces / 2 + nn);
				}
				else
					iter++;
			}		
		}
	}
}

void SetFalseRecursive(vector<vector<bool>> &index, int id, int grid_id)
{
	int pieces = Return_resolution(grid_id);
	int offset = Return_Offset(pieces);
	if (pieces == 128)
	{
		index[id][grid_id] = false;
	}
	else
	{
		if (index[id][grid_id])
			index[id][grid_id] = false;
		else
		{
			int off_id = grid_id - offset;
			int m = off_id / pieces;
			int n = off_id - m*pieces;
			int mm = m * 2, nn = n * 2;
			int base = mm*pieces * 2 + nn + offset - pieces*pieces * 4;
			SetFalseRecursive(index, id, base);
			SetFalseRecursive(index, id, base + 1);
			SetFalseRecursive(index, id, base + pieces * 2);
			SetFalseRecursive(index, id, base + pieces * 2 + 1);
		}
	}
}

void SetFalseRecursive(vector<set<int>> &index, int id, int grid_id)
{
	int pieces = Return_resolution(grid_id);
	int offset = Return_Offset(pieces);
	if (pieces == 128)
	{
		index[id].erase(grid_id);
	}
	else
	{
		if (index[id].find(grid_id)!=index[id].end())
			index[id].erase(grid_id);
		else
		{
			int off_id = grid_id - offset;
			int m = off_id / pieces;
			int n = off_id - m*pieces;
			int mm = m * 2, nn = n * 2;
			int base = mm*pieces * 2 + nn + offset - pieces*pieces * 4;
			SetFalseRecursive(index, id, base);
			SetFalseRecursive(index, id, base + 1);
			SetFalseRecursive(index, id, base + pieces * 2);
			SetFalseRecursive(index, id, base + pieces * 2 + 1);
		}
	}
}

void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<vector<bool>> &index, queue<int> &queue, int merge_count)
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
						IsUpdate[in_neighbor] = true;
					}
				}
			}
		}
	}
	int offset = 0;
	for (int i = pieces_x; i > 2; i /= 2)
	{
		offset += i*i;
		for (int id = 0; id < in_edge_graph.size(); id++)
		{
			for (int m = 0; m < i; m += 2)
			{
				for (int n = 0; n < i; n += 2)
				{
					int grid_id = m*i + n + offset - i*i;
					int true_count = 0;
					if (index[id][grid_id])
						true_count++;
					if (index[id][grid_id + 1])
						true_count++;
					if (index[id][grid_id + i])
						true_count++;
					if (index[id][grid_id + i + 1])
						true_count++;
					if (true_count >= merge_count)
					{
						int mm = m / 2, nn = n / 2;
						int high_level_grid_id = mm*i / 2 + nn;
						index[id][high_level_grid_id + offset] = true;
						SetFalseRecursive(index, id, grid_id);
						SetFalseRecursive(index, id, grid_id + 1);
						SetFalseRecursive(index, id, grid_id + i);
						SetFalseRecursive(index, id, grid_id + i + 1);
					}
				}
			}
		}
	}
}

void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue, int merge_count)
{
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
				index[in_neighbor].insert(grid_id);
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
				set<int>::iterator end = index[id].end();
				for (set<int>::iterator iter = index[id].begin(); iter != end; iter++)
				{
					if (index[in_neighbor].find(*iter) == index[in_neighbor].end())
					{
						index[in_neighbor].insert(*iter);
						IsUpdate[in_neighbor] = true;
					}
				}
			}
		}
	}
	for (int j = 0; j < in_edge_graph.size(); j++)
	{
		for (set<int>::iterator iter = index[j].begin(); iter != index[j].end();)
		{
			int reach_grid_id = *iter;
			int pieces = Return_resolution(reach_grid_id);
			int offset = Return_Offset(pieces);
			int id = reach_grid_id - offset;
			int m = id / pieces, n = id - m*pieces;
			int mm = m / 2, nn = n / 2;
			m = mm * 2, n = nn * 2;
			int base = m*pieces + n + offset;

			int true_count = 0;
			set<int>::iterator end = index[j].end();
			if (index[j].find(base) != end)
				true_count++;
			if (index[j].find(base + 1) != end)
				true_count++;
			if (index[j].find(base + pieces) != end)
				true_count++;
			if (index[j].find(base + pieces + 1) != end)
				true_count++;
			if (true_count >= merge_count)
			{
				while ((*iter == base || *iter == base + 1 || *iter == base + pieces + 1 || *iter == base + pieces) && iter != end)
				{
					iter++;
				}
				SetFalseRecursive(index, j, base);
				SetFalseRecursive(index, j, base + 1);
				SetFalseRecursive(index, j, base + pieces);
				SetFalseRecursive(index, j, base + pieces + 1);

				index[j].insert(offset + pieces*pieces + mm*pieces / 2 + nn);
			}
			else
				iter++;
		}
	}
}

void GenerateMultilevelGridPointIndexFull(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, MultiLevelGridIndexVector &multilevel_index, queue<int> &queue, int merge_count)
{
	double resolution_x = (right_top.x - left_bottom.x) / pieces_x;
	double resolution_y = (right_top.y - left_bottom.y) / pieces_y;

	vector<bool> IsUpdate = vector<bool>(in_edge_graph.size());

	for (int i = 0; i < p_entity.size(); i++)
	{
		if (p_entity[i].IsSpatial)
		{
			int m = (p_entity[i].location.x - left_bottom.x) / resolution_x;
			int n = (p_entity[i].location.y - left_bottom.y) / resolution_y;
			//int grid_id = m*pieces_x + n;
			for (int j = 0; j < in_edge_graph[i].size(); j++)
			{
				int in_neighbor = in_edge_graph[i][j];
				multilevel_index.index[pieces_x][in_neighbor][m][n] = true;
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
				for (int j = 0; j < pieces_x; j++)
				{
					for (int k = 0; k < pieces_y; k++)
					{
						if (multilevel_index.index[pieces_x][id][j][k])
						{
							multilevel_index.index[pieces_x][in_neighbor][j][k] = true;
							IsUpdate[in_neighbor] = true;
						}
					}
				}
					
				/*for (int j = 0; j < grid_count; j++)
				{
					if (index[id][j])
					{
						index[in_neighbor][j] = true;
						IsUpdate[in_neighbor] = true;
					}
				}*/
			}
		}
	}
	/*int offset = 0;
	for (int i = pieces_x; i >= 2; i /= 2)
	{
		offset += i*i;
		for (int id = 0; id < in_edge_graph.size(); id++)
		{
			for (int m = 0; m < i; m += 2)
			{
				for (int n = 0; n < i; n += 2)
				{
					int grid_id = m*i + n + offset - i*i;
					int true_count = 0;
					if (index[id][grid_id])
						true_count++;
					if (index[id][grid_id + 1])
						true_count++;
					if (index[id][grid_id + i])
						true_count++;
					if (index[id][grid_id + i + 1])
						true_count++;
					if (true_count >= merge_count)
					{
						int mm = m / 2, nn = n / 2;
						int high_level_grid_id = mm*i / 2 + nn;
						index[id][high_level_grid_id + offset] = true;
						index[id][grid_id] = false;
						index[id][grid_id + 1] = false;
						index[id][grid_id + i] = false;
						index[id][grid_id + i + 1] = false;
					}
				}
			}
		}
	}*/
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

void GridPointIndexToDisk(vector<vector<bool>> &index, string filename, vector<bool> &IsStored)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", index.size());

	for (int i = 0; i < index.size(); i++)
	{
		if (IsStored[i])
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
		}
		else
			continue;
		printf("\n");
	}
	fclose(stdout);
}

void GridPointIndexToDisk(vector<set<int>> &index, string filename, vector<bool> &IsStored)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", index.size());

	for (int i = 0; i < index.size(); i++)
	{
		if (IsStored[i])
		{
			printf("%d %d ", i, index[i].size());
			set<int>::iterator end = index[i].end();
			for (set<int>::iterator iter = index[i].begin(); iter != end; iter++)
			{
				printf("%d ", *iter);
			}
		}
		else
			continue;
		printf("\n");
	}
	fclose(stdout);
}

void GridPointIndexToDisk(vector<vector<bool>> &index, string filename,vector<int> stored, int total_count)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", total_count);

	for (int i = 0; i < stored.size(); i++)
	{
		int id = stored[i];
		int count = 0;
		for (int j = 0; j < index[i].size(); j++)
		{
			if (index[i][j])
				count++;
		}
		printf("%d %d ", id, count);
		for (int j = 0; j < index[i].size(); j++)
		{
			if (index[i][j])
				printf("%d ", j);
		}
		printf("\n");
	}

	fclose(stdout);
}

void GridPointIndexToDisk(vector<set<int>> &index, string filename, vector<int> stored, int total_count)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", total_count);

	for (int i = 0; i < stored.size(); i++)
	{
		int id = stored[i];
		int count = index[i].size();

		printf("%d %d ", id, count);

		set<int>::iterator end = index[i].end();
		for (set<int>::iterator iter = index[i].begin(); iter != end; iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	fclose(stdout);
}

void GridPointMultilevelIndexToDisk(MultiLevelGridIndexVector &multilevel_index, string filename, int pieces_max)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d\n", multilevel_index.index[pieces_max].size());

	for (int i = 0; i < multilevel_index.index[pieces_max].size(); i++)
	{
		int count = 0;
		for (int pieces = pieces_max; pieces >= 128; pieces /= 2)
		{
			for (int j = 0; j < pieces; j++)
			{
				for (int k = 0; k < pieces; k++)
				{
					if (multilevel_index.index[pieces][i][j][k])
						count++;
				}
			}
		}
		printf("%d %d ", i, count);
		int offset = 0;
		for (int pieces = pieces_max; pieces >= 128; pieces /= 2)
		{
			offset += pieces*pieces;
			for (int j = 0; j < pieces; j++)
			{
				for (int k = 0; k < pieces; k++)
				{
					if (multilevel_index.index[pieces][i][j][k])
						printf("%d ", j*pieces + k + offset - pieces*pieces);
				}
			}
		}
		printf("\n");
	}
	fclose(stdout);
	/*for (int i = 0; i < index.size(); i++)
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
	fclose(stdout);*/
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
						IsUpdate[in_neighbor] = true;
					}
				}
			}
		}
	}
}

void GenerateGridIndexSequence(Location left_bottom, Location right_top, int pieces_x, int pieces_y, vector<vector<int>> &in_edge_graph, vector<Entity> &p_entity, vector<set<int>> &index, queue<int> &queue)
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
				index[in_neighbor].insert(grid_id);
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
				set<int>::iterator end = index[id].end();
				for (set<int>::iterator iter = index[id].begin(); iter != end; iter++)
				{
					if (index[in_neighbor].find(*iter) == index[in_neighbor].end())
					{
						index[in_neighbor].insert(*iter);
						IsUpdate[in_neighbor] = true;
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
	fclose(stdin);
}

void ReadGridPointIndexMultilevelFromDisk(int pieces, vector<vector<bool>> &index, string filename,vector<int> &isstored,  int &node_count)
{
	if (isstored.size()>0)
	{
		for (int i = 0; i < index.size(); i++)
			for (int j = 0; j < index[i].size(); j++)
				index[i][j] = false;
		ifstream ifile("data/" + filename);
		string temp;
		getline(ifile, temp);
		node_count = StringtoInt(temp);

		int line_count = 0;
		while (!ifile.eof())
		{
			string temp;
			getline(ifile, temp);
			if (temp == "")
				break;
			vector<string> v = split(temp, " ");

			int id, count;
			id = StringtoInt(v[0]);
			count = StringtoInt(v[1]);
			for (int i = 0; i < count; i++)
			{
				int reachgridid = StringtoInt(v[i + 2]);
				index[line_count][reachgridid] = true;
			}
			line_count++;
		}
		ifile.close();
	}
	else
	{
		ifstream ifile("data/" + filename);

		int line_count = 0;
		string temp;
		getline(ifile, temp);
		while (!ifile.eof())
		{
			getline(ifile, temp);
			if (temp == "")
				break;
			line_count++;
		}
		ifile.close();
		index.resize(line_count);
		isstored.resize(line_count);

		ifile.open("data/" + filename);

		getline(ifile, temp);
		node_count = StringtoInt(temp);
		int grid_count = 0;
		for (int i = pieces; i >= 2; i /= 2)
		{
			grid_count += i*i;
		}
		for (int i = 0; i < index.size(); i++)
			index[i].resize(grid_count);

		line_count = 0;
		while (!ifile.eof())
		{
			string temp;
			getline(ifile, temp);
			if (temp == "")
				break;
			vector<string> v = split(temp, " ");

			int id, count;
			id = StringtoInt(v[0]);
			count = StringtoInt(v[1]);
			isstored[line_count] = id;
			for (int i = 0; i < count; i++)
			{
				int reachgridid = StringtoInt(v[i + 2]);
				index[line_count][reachgridid] = true;
			}
			line_count++;
		}
		ifile.close();
	}
	
}

void ReadGridPointIndexMultilevelFromDisk(int pieces, vector<set<int>> &index, string filename, vector<int> &isstored, int &node_count)
{
	if (index.size()>0)
	{
		ifstream ifile("data/" + filename);
		string temp;
		getline(ifile, temp);
		node_count = StringtoInt(temp);

		for (int i = 0; i < index.size(); i++)
			index[i].clear();

		int line_count = 0;
		while (!ifile.eof())
		{
			string temp;
			getline(ifile, temp);
			if (temp == "")
				break;
			vector<string> v = split(temp, " ");

			int id, count;
			id = StringtoInt(v[0]);
			count = StringtoInt(v[1]);
			isstored.push_back(id);
			for (int i = 0; i < count; i++)
			{
				int reachgridid = StringtoInt(v[i + 2]);
				index[line_count].insert(reachgridid);
			}
			line_count++;
		}
		ifile.close();
	}
	else
	{
		ifstream ifile("data/" + filename);

		int line_count = 0;
		string temp;
		getline(ifile, temp);
		while (!ifile.eof())
		{
			getline(ifile, temp);
			if (temp == "")
				break;
			line_count++;
		}
		ifile.close();
		index.resize(line_count);
		isstored.resize(line_count);

		ifile.open("data/" + filename);
		getline(ifile, temp);
		node_count = StringtoInt(temp);

		line_count = 0;
		while (!ifile.eof())
		{
			string temp;
			getline(ifile, temp);
			if (temp == "")
				break;
			vector<string> v = split(temp, " ");

			int id, count;
			id = StringtoInt(v[0]);
			count = StringtoInt(v[1]);
			isstored[line_count] = id;
			for (int i = 0; i < count; i++)
			{
				int reachgridid = StringtoInt(v[i + 2]);
				index[line_count].insert(reachgridid);
			}
			line_count++;
		}
		ifile.close();
	}
}