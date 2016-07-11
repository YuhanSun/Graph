#include "stdafx.h"
#include "RMBR.h"

set<int> visited_vertex;

void Initialize_Visited_vertex()
{
	visited_vertex.clear();
}

bool RecUnion(MyRect &rect_v, MyRect &rect_neighbour)
{
	bool flag = false;

	if (rect_v.left_bottom.x < 0 && rect_neighbour.left_bottom.x < 0)
		return false;

	if (rect_v.left_bottom.x < 0)
	{
		rect_v.left_bottom.x = rect_neighbour.left_bottom.x;
		rect_v.left_bottom.y = rect_neighbour.left_bottom.y;
		rect_v.right_top.x = rect_neighbour.right_top.x;
		rect_v.right_top.y = rect_neighbour.right_top.y;
		return true;
	}

	if (rect_neighbour.left_bottom.x < 0)
		return false;

	if (rect_neighbour.left_bottom.x < rect_v.left_bottom.x)
	{
		rect_v.left_bottom.x = rect_neighbour.left_bottom.x;
		flag = true;
	}
	if (rect_neighbour.left_bottom.y < rect_v.left_bottom.y)
	{
		rect_v.left_bottom.y = rect_neighbour.left_bottom.y;
		flag = true;
	}

	if (rect_neighbour.right_top.x > rect_v.right_top.x)
	{
		rect_v.right_top.x = rect_neighbour.right_top.x;
		flag = true;
	}
	if (rect_neighbour.right_top.y > rect_v.right_top.y)
	{
		rect_v.right_top.y = rect_neighbour.right_top.y;
		flag = true;
	}

	return flag;
}

void GenerateRMBR(vector<Entity> &p_entity, vector<vector<int>> &p_graph)
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
			for (int j = 0; j < p_graph[i].size() / 2; j++)
			{
				int k = 2 * j + 1;
				int neibour_id = p_graph[i][k];
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

void GenerateRMBR(vector<Entity> &p_entity, vector<vector<int>> &in_edge_graph, queue<int>& queue)
{
	for (int i = 0; i < p_entity.size(); i++)
	{
		p_entity[i].RMBR.left_bottom.x = -1;
		p_entity[i].RMBR.left_bottom.y = -1;
		p_entity[i].RMBR.right_top.x = -1;
		p_entity[i].RMBR.right_top.y = -1;
	}

	while (!queue.empty())
	{
		int id = queue.front();
		queue.pop();

		if (p_entity[id].IsSpatial)
		{
			MyRect rec;
			rec.left_bottom.x = p_entity[id].location.x;
			rec.left_bottom.y = p_entity[id].location.y;
			rec.right_top.x = p_entity[id].location.x;
			rec.right_top.y = p_entity[id].location.y;
			for (int i = 0; i < in_edge_graph[id].size(); i++)
				RecUnion(p_entity[in_edge_graph[id][i]].RMBR, rec);
		}
		for (int i = 0; i < in_edge_graph[id].size(); i++)
			RecUnion(p_entity[in_edge_graph[id][i]].RMBR, p_entity[id].RMBR);
	}
}

void RMBR_To_Disk(vector<Entity> &p_entity, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", p_entity.size(), range);
	for (int i = 0; i < p_entity.size(); i++)
	{
		printf("%d %f %f %f %f\n", i, p_entity[i].RMBR.left_bottom.x, p_entity[i].RMBR.left_bottom.y, p_entity[i].RMBR.right_top.x, p_entity[i].RMBR.right_top.y);
	}
	fclose(stdout);
}

void ReadRmbr(vector<Entity> &p_entity, int &range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	int node_count = 0;

	scanf("%d %d", &node_count, &range);

	if (node_count != p_entity.size())
		MessageBox(NULL, _T("Entity file and RMBR file node count are not consistent"), _T("Error"), MB_OK);
	
	for (int i = 0; i < node_count; i++)
	{
		int id;
		scanf("%d", &id);
		scanf("%lf %lf %lf %lf", &(p_entity[id].RMBR.left_bottom.x), &(p_entity[id].RMBR.left_bottom.y), &(p_entity[id].RMBR.right_top.x), &(p_entity[id].RMBR.right_top.y));
	}
	fclose(stdin);
}

bool ReachabilityQuery(vector<Entity> &entity, vector<vector<int>> &graph, int start_id, MyRect query_rect)
{
	MyRect RMBR = entity[start_id].RMBR;

	if (RMBR.left_bottom.x == -1)
		return false;

	if (RMBR.left_bottom.x > query_rect.right_top.x || RMBR.left_bottom.y > query_rect.right_top.y || RMBR.right_top.x < query_rect.left_bottom.x || RMBR.right_top.y < query_rect.left_bottom.y)
		return false;

	if (RMBR.left_bottom.x >= query_rect.left_bottom.x && RMBR.left_bottom.y >= query_rect.left_bottom.y && RMBR.right_top.x <= query_rect.right_top.x && RMBR.right_top.y <= query_rect.right_top.y)
		return true;

	for (int i = 0; i < graph[start_id].size() / 2; i++) 
	{
		int outneighbor_id = graph[start_id][i * 2 + 1];

		if (visited_vertex.find(outneighbor_id) != visited_vertex.end())
			continue;

		if (entity[outneighbor_id].IsSpatial)
			if (Location_In_Rect(entity[outneighbor_id].location, query_rect))
				return true;

		visited_vertex.insert(outneighbor_id);

		bool result = ReachabilityQuery(entity, graph, outneighbor_id, query_rect);
		if (result)
			return true;
	}
	return false;
}

////for transitive closure testing
vector<vector<int>> GetTransitiveClosure(vector<vector<int>> &graph)
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
		Traverse(i, graph, isvisited,transitive_closure[i]);
	}

	return transitive_closure;
}

vector<set<int>> GetTransitiveClosureDynamic_In_Set(vector<vector<int>> &p_graph)
{
	int p_node_count = p_graph.size();

	vector<set<int>> transitive_closure;
	transitive_closure.resize(p_node_count);

	for (int i = 0; i < p_graph.size(); i++)
	{
		for (int j = 0; j < p_graph[i].size() / 2; j++)
		{
			int k = 2 * j + 1;
			int neibour_id = p_graph[i][k];

			transitive_closure[i].insert(neibour_id);
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

vector<hash_set<int>> GetTransitiveClosureDynamic(vector<vector<int>> &p_graph)
{
	int p_node_count = p_graph.size();

	vector<hash_set<int>> transitive_closure;
	transitive_closure.resize(p_node_count);

	for (int i = 0; i < p_graph.size(); i++)
	{
		for (int j = 0; j < p_graph[i].size() / 2; j++)
		{			
			int k = 2 * j + 1;
			int neibour_id = p_graph[i][k];

			transitive_closure[i].insert(neibour_id);
		}
	}

	bool changed = true;

	while (changed)
	{
		changed = false;

		for (int i = 0; i < p_graph.size(); i++)
		{
			hash_set<int>::iterator iter_line = transitive_closure[i].begin();
			hash_set<int>::iterator end_line = transitive_closure[i].end();

			for (; iter_line != end_line; iter_line++)
			{
				int neighbor = *iter_line;

				hash_set<int>::iterator iter_neighbor = transitive_closure[neighbor].begin();
				hash_set<int>::iterator end_neighbor = transitive_closure[neighbor].end();

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

vector<set<int>> GetTransitiveClosureDynamic(vector<vector<int>> &graph_outedge, vector<vector<int>> &graph_inedge)
{
	int p_node_count = graph_outedge.size();

	vector<set<int>> transitive_closure;
	transitive_closure.resize(p_node_count);

	queue<int> updated_nodes_queue;
	set<int> updated_nodes_set;

	for (int i = 0; i < graph_outedge.size(); i++)
	{
		for (int j = 0; j < graph_outedge[i].size() / 2; j++)
		{
			int k = 2 * j + 1;
			int neibour_id = graph_outedge[i][k];

			transitive_closure[i].insert(neibour_id);
		}

		if (transitive_closure[i].size() > 0)
		{
			updated_nodes_queue.push(i);
			updated_nodes_set.insert(i);
		}
	}

	while (!updated_nodes_queue.empty())
	{
		int id = updated_nodes_queue.front();
		updated_nodes_queue.pop();
		updated_nodes_set.erase(id);
		for (int i = 0; i < graph_outedge.size(); i++)
		{
			bool changed = false;

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

			if (changed)
			{
				for (int j = 0; j < graph_inedge[i].size() / 2; j++)
				{
					int k = 2 * j + 1;
					int in_neighbor = graph_inedge[i][k];
					if (updated_nodes_set.find(in_neighbor) == updated_nodes_set.end())
					{
						updated_nodes_set.insert(in_neighbor);
						updated_nodes_queue.push(in_neighbor);
					}
				}
			}
		}
	}
	return transitive_closure;
}

vector<int>GetTransitiveClosureLine(int i, vector<vector<int>> &graph)
{
	int node_count = graph.size();
	vector<bool> isvisited;
	isvisited.resize(node_count);

	vector<int> transitiveclosure_line;
	transitiveclosure_line.reserve(node_count);

	Traverse(i, graph, isvisited, transitiveclosure_line);

	return transitiveclosure_line;
}

void Traverse(int id, vector<vector<int>> &graph, vector<bool> &isvisted, vector<int> &reach_vertices)
{
	for (int i = 0; i < graph[id].size() / 2; i++)
	{
		int out_id = graph[id][2 * i + 1];
		if (!isvisted[out_id])
		{
			isvisted[out_id] = true;
			reach_vertices.push_back(out_id);
			Traverse(out_id, graph, isvisted, reach_vertices);
		}
	}
}

vector<MyRect> GetRMBRFromTransitiveClosure(vector<vector<int>> &transitive_closure, vector<Entity> &entity)
{
	int node_count = transitive_closure.size();

	vector<MyRect> RMBR;
	RMBR.resize(node_count);
	for (int i = 0; i < transitive_closure.size(); i++)
	{
		double min_x = -1;
		double min_y = -1;
		double max_x = -1;
		double max_y = -1;

		for (int j = 0; j < transitive_closure[i].size(); j++)
		{
			int id = transitive_closure[i][j];
			if (entity[id].IsSpatial)
			{
				if (min_x == -1)
				{
					min_x = entity[id].location.x;
					max_x = entity[id].location.x;
					min_y = entity[id].location.y;
					max_y = entity[id].location.y;
				}
				else
				{
					if (entity[id].location.x < min_x)
						min_x = entity[id].location.x;
					if (entity[id].location.x > max_x)
						max_x = entity[id].location.x;
					if (entity[id].location.y < min_y)
						min_y = entity[id].location.y;
					if (entity[id].location.y > max_y)
						max_y = entity[id].location.y;
				}
			}
		}

		RMBR[i].left_bottom.x = min_x;
		RMBR[i].left_bottom.y = min_y;
		RMBR[i].right_top.x = max_x;
		RMBR[i].right_top.y = max_y;
	}

	return RMBR;
}

void ReadTransitiveClosure(vector<vector<int>> &transitive_closure, int &node_count, int &range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);


	scanf("%d %d", &node_count, &range);

	transitive_closure.resize(node_count);

	for (int i = 0; i < node_count; i++)
	{
		int id;
		int count;
		scanf("%d %d ", &id, &count);
		transitive_closure[i].resize(count);
		for (int j = 0; j < count; j++)
		{
			scanf("%d ", &transitive_closure[i][j]);
		}
	}
	fclose(stdin);
}

void TransitiveClosure_To_Disk(vector<vector<int>> &transitive_closure, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", transitive_closure.size(), range);
	for (int i = 0; i < transitive_closure.size(); i++)
	{
		printf("%d %d ", i, transitive_closure[i].size());
		for (int j = 0; j < transitive_closure[i].size(); j++)
		{
			printf("%d ", transitive_closure[i][j]);
		}
		printf("\n");
	}
	fclose(stdout);
}

void TransitiveClosureDynamic_To_Disk(vector<set<int>> &transitive_closure_dynamic, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", transitive_closure_dynamic.size(), range);
	for (int i = 0; i < transitive_closure_dynamic.size(); i++)
	{
		printf("%d %d ", i, transitive_closure_dynamic[i].size());
		set<int>::iterator end = transitive_closure_dynamic[i].end();
		for (set<int>::iterator iter = transitive_closure_dynamic[i].begin(); iter != end; iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	fclose(stdout);
}

void TransitiveClosureDynamic_To_Disk(vector<hash_set<int>> &transitive_closure_dynamic, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", transitive_closure_dynamic.size(), range);
	for (int i = 0; i < transitive_closure_dynamic.size(); i++)
	{
		printf("%d %d ", i, transitive_closure_dynamic[i].size());
		hash_set<int>::iterator end = transitive_closure_dynamic[i].end();
		for (hash_set<int>::iterator iter = transitive_closure_dynamic[i].begin(); iter != end; iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	fclose(stdout);
}

void TransitiveClosureLine_To_Disk(vector<int> &transitive_closure_line, int id, string filename, int node_count)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "a", stdout);

	printf("%d %d ", id, transitive_closure_line.size());

	for (int j = 0; j < transitive_closure_line.size(); j++)
	{
		int id = transitive_closure_line[j];
		printf("%d ", id);
	}

	printf("\n");

	fclose(stdout);
}



void SpatialTransitiveClosure_To_Disk(vector<vector<int>> &transitive_closure, int range, string filename, vector<Entity> p_entity)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", transitive_closure.size(), range);
	for (int i = 0; i < transitive_closure.size(); i++)
	{
		int count = 0;
		for (int j = 0; j < transitive_closure[i].size(); j++)
		{
			int id = transitive_closure[i][j];
			if (p_entity[id].IsSpatial)
				count++;
		}
		printf("%d %d ", i, count);

		for (int j = 0; j < transitive_closure[i].size(); j++)
		{
			int id = transitive_closure[i][j];
			if (p_entity[id].IsSpatial)
				printf("%d ", id);
		}

		printf("\n");
	}
	fclose(stdout);
}

void SpatialTransitiveClosureDynamic_To_Disk(vector<set<int>> &transitive_closure_dynamic, int range, string filename, vector<Entity> p_entity)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", transitive_closure_dynamic.size(), range);
	for (int i = 0; i < transitive_closure_dynamic.size(); i++)
	{
		int count = 0;
		set<int>::iterator iter_end = transitive_closure_dynamic[i].end();
		for (set<int>::iterator iter = transitive_closure_dynamic[i].begin(); iter != iter_end; iter++)
		{
			int id = *iter;
			if (p_entity[id].IsSpatial)
				count++;
		}

		printf("%d %d ", i, count);

		for (set<int>::iterator iter = transitive_closure_dynamic[i].begin(); iter != iter_end; iter++)
		{
			int id = *iter;
			if (p_entity[id].IsSpatial)
				printf("%d ", id);
		}

		printf("\n");
	}
	fclose(stdout);
}


void SpatialTransitiveClosureLine_To_Disk(vector<int> &transitive_closure_line, int id, string filename, int node_count, double nonspatial_ratio)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "a", stdout);

	int count = 0;
	for (int j = 0; j < transitive_closure_line.size(); j++)
	{
		int id = transitive_closure_line[j];
		if (node_count*nonspatial_ratio >= id)
			count++;
	}
	printf("%d %d ", id, count);

	for (int j = 0; j < transitive_closure_line.size(); j++)
	{
		int id = transitive_closure_line[j];
		if (node_count*nonspatial_ratio >= id)
			printf("%d ", id);
	}

	printf("\n");

	fclose(stdout);
}


void TransitiveClosureRMBR_To_Disk(vector<MyRect> RMBR, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", RMBR.size(), range);
	for (int i = 0; i < RMBR.size(); i++)
	{
		printf("%d %f %f %f %f\n", i, RMBR[i].left_bottom.x, RMBR[i].left_bottom.y, RMBR[i].right_top.x, RMBR[i].right_top.y);
	}
	fclose(stdout);
}