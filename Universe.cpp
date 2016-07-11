#include "stdafx.h"
#include "Universe.h"

vector<int>hit_id;
int start, runtime;

bool MySearchCallback(int id, void* arg)
{
	//	cout << "Hit data rect " << id << "\n";
	hit_id.push_back(id);
	return true; // keep going
}

vector<int> GetHitID()
{
	return hit_id;
}

void ResetHitID(int size)
{
	hit_id.clear();
	hit_id.reserve(size);
}

int GetStartTime()
{
	return start;
}

void SetStartTime(int i)
{
	start = i;
}

int GetRunTime()
{
	return runtime;
}

void SetRunTime(int i)
{
	runtime = i;
}

bool Location_In_Rect(Location m_location, MyRect m_rect)
{
	if (m_location.x<m_rect.left_bottom.x || m_location.x>m_rect.right_top.x || m_location.y<m_rect.left_bottom.y || m_location.y>m_rect.right_top.y)
		return false;
	else
		return true;
}

void OutFile(Entity Entity_Matrix[], int node_count, string filename)
{
	ofstream file;
	file.open("data/" + filename);
	for (int i = 0; i < node_count; i++)
	{
		file << i << "    " << Entity_Matrix[i].IsSpatial << "    " << Entity_Matrix[i].location.x << "  " << Entity_Matrix[i].location.y << "    " << Entity_Matrix[i].type;
		file << endl;
	}
	file.close();
}

void OutFile(vector<Entity> &entity_vector, string filename)
{
	ofstream file;
	file.open("data/" + filename);
	for (int i = 0; i < entity_vector.size(); i++)
	{
		file << i << "    " << entity_vector[i].IsSpatial << "    " << entity_vector[i].location.x << "  " << entity_vector[i].location.y << "    " << entity_vector[i].type;
		file << endl;
	}
	file.close();
}

void EntityToDisk(Entity Entity_Matrix[], int node_count, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);
	
	printf("%d %d\n", node_count,range);
	for (int i = 0; i < node_count; i++)
	{
		printf("%d %d %f %f %d\n", i, Entity_Matrix[i].IsSpatial, Entity_Matrix[i].location.x, Entity_Matrix[i].location.y, Entity_Matrix[i].type);
	}
	fclose(stdout);
}

void EntityToDisk(vector<Entity> entity_vector, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);

	printf("%d %d\n", entity_vector.size(), range);
	for (int i = 0; i < entity_vector.size(); i++)
	{
		printf("%d %d %f %f %d\n", i, entity_vector[i].IsSpatial, entity_vector[i].location.x, entity_vector[i].location.y, entity_vector[i].type);
	}
	fclose(stdout);
}

void GenerateEntity(int node_count, Entity Entity_Matrix[], int range, double nonspatial_entity_ratio)
{
	TRnd Rnd = time(0);
	for (int i = 0; i < node_count * nonspatial_entity_ratio; i++)
	{
		Entity_Matrix[i].id = i;
		Entity_Matrix[i].IsSpatial = false;

		Entity_Matrix[i].location.x = -1;
		Entity_Matrix[i].location.y = -1;

		Entity_Matrix[i].type = (Rnd.GetUniDev()<0.1 ? 0 : 1);
	}


	for (int i = node_count * nonspatial_entity_ratio; i < node_count; i++)
	{
		Entity_Matrix[i].id = i;
		Entity_Matrix[i].IsSpatial = true;

		Entity_Matrix[i].location.x = Rnd.GetUniDev() * range;
		Entity_Matrix[i].location.y = Rnd.GetUniDev() * range;

		Entity_Matrix[i].type = (Rnd.GetUniDev()>0.5 ? 100 : 101);//100   restaurant     101    theatre
	}
}

void GenerateEntity(int node_count, vector<Entity> &entity_vector, int range, double nonspatial_entity_ratio)
{
	TRnd Rnd = time(0);
	entity_vector.resize(node_count);
	//nonspatial entity
	for (int i = 0; i < node_count * nonspatial_entity_ratio; i++)
	{
		entity_vector[i].id = i;
		entity_vector[i].IsSpatial = false;

		entity_vector[i].location.x = -1;
		entity_vector[i].location.y = -1;

		double x = Rnd.GetUniDev();
		
		entity_vector[i].type = 0;
		/*
		if (x < 0.25)
		{
			entity_vector[i].type = 0;//0.1
			continue;
		}
		if (x < 0.5)
		{
			entity_vector[i].type = 1;//0.1
			continue;
		}
		if (x < 0.75)
		{
			entity_vector[i].type = (x - 0.5) / 0.025 + 2;//0.01
			continue;
		}
		if (x < 0.8)
		{
			entity_vector[i].type = (x - 0.75) / 0.005 + 13;//0.002
			continue;
		}
		if (x < 0.85)
		{
			entity_vector[i].type = (x - 0.8) / 0.0025 + 23;//0.001
			continue;
		}
		if (x < 0.86)
		{
			entity_vector[i].type = (x - 0.85) / 0.0005 + 43;//0.0002
			continue;
		}
		else
			entity_vector[i].type = 99;
		*/
	}
	

	//spatial entity
	for (int i = node_count * nonspatial_entity_ratio; i < node_count; i++)
	{
		entity_vector[i].id = i;
		entity_vector[i].IsSpatial = true;

		entity_vector[i].location.x = Rnd.GetUniDev() * range;
		entity_vector[i].location.y = Rnd.GetUniDev() * range;

		entity_vector[i].type = (Rnd.GetUniDev()>0.5 ? 100 : 101);//100   restaurant     101    theatre
	}
}

void ReadEntityFromDisk(int &node_count, Entity Entity_Matrix[], int &range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d %d", &node_count, &range);
	for (int i = 0; i < node_count; i++)
	{
		scanf("%d %d %lf %lf %d", &(Entity_Matrix[i].id), &(Entity_Matrix[i].IsSpatial), &(Entity_Matrix[i].location.x), &(Entity_Matrix[i].location.y), &(Entity_Matrix[i].type));
	}
	fclose(stdin);
}

void ReadEntityFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d %d", &node_count, &range);
	entity_vector.resize(node_count);
	for (int i = 0; i < node_count; i++)
	{
		scanf("%d %d %lf %lf %d", &(entity_vector[i].id), &(entity_vector[i].IsSpatial), &(entity_vector[i].location.x), &(entity_vector[i].location.y), &(entity_vector[i].type));
	}
	fclose(stdin);
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

void ReadEntityInSCCSeperateFromDisk(int &node_count, vector<Entity> &entity_vector, int &range, string filename)
{
	string root = "data/";
	root += (filename + "/spatial_entity.txt");
	char *ch = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d %d", &node_count, &range);
	entity_vector.resize(node_count);
	while (true)
	{
		int id;
		int count = scanf("%d ", &id);
		entity_vector[id].id = id;
		if (count != 1)
			break;
		scanf("%d %lf %lf %d %d %lf %lf %lf %lf", &(entity_vector[id].IsSpatial), &(entity_vector[id].location.x), &(entity_vector[id].location.y), &(entity_vector[id].type), &(entity_vector[id].scc_id), &(entity_vector[id].RMBR.left_bottom.x), &(entity_vector[id].RMBR.left_bottom.y), &(entity_vector[id].RMBR.right_top.x), &(entity_vector[id].RMBR.right_top.y));
	}
	fclose(stdin);

	root = "data/";
	root += (filename + "/nonspatial_entity.txt");
	char *ch2 = (char *)root.data();
	freopen(ch, "r", stdin);

	scanf("%d %d", &node_count, &range);
	while (true)
	{
		int id;
		int count = scanf("%d ", &id);
		entity_vector[id].id = id;
		if (count != 1)
			break;
		scanf("%d %lf %lf %d %d %lf %lf %lf %lf", &(entity_vector[id].IsSpatial), &(entity_vector[id].location.x), &(entity_vector[id].location.y), &(entity_vector[id].type), &(entity_vector[id].scc_id), &(entity_vector[id].RMBR.left_bottom.x), &(entity_vector[id].RMBR.left_bottom.y), &(entity_vector[id].RMBR.right_top.x), &(entity_vector[id].RMBR.right_top.y));
	}
	fclose(stdin);
}

void EntityInSCCSeperate_To_Disk(vector<Entity> &entity_vector, int range, string filename)
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

	printf("%d %d\n", entity_vector.size(), range);
	for (int i = 0; i < entity_vector.size(); i++)
	{
		if (!entity_vector[i].IsSpatial)
			printf("%d %d %f %f %d %d %lf %lf %lf %lf\n", entity_vector[i].id, entity_vector[i].IsSpatial, entity_vector[i].location.x, entity_vector[i].location.y, entity_vector[i].type, entity_vector[i].scc_id, entity_vector[i].RMBR.left_bottom.x, entity_vector[i].RMBR.left_bottom.y, entity_vector[i].RMBR.right_top.x, entity_vector[i].RMBR.right_top.y);
	}
	fclose(stdout);
}

void EntityInSCC_To_Disk(vector<Entity> &entity_vector, int range, string filename)
{
	string root = "data/";
	root += filename;
	char *ch1 = (char *)root.data();
	freopen(ch1, "w", stdout);

	printf("%d %d\n", entity_vector.size(), range);
	for (int i = 0; i < entity_vector.size(); i++)
	{
		printf("%d %d %f %f %d %d %lf %lf %lf %lf\n", entity_vector[i].id, entity_vector[i].IsSpatial, entity_vector[i].location.x, entity_vector[i].location.y, entity_vector[i].type, entity_vector[i].scc_id, entity_vector[i].RMBR.left_bottom.x, entity_vector[i].RMBR.left_bottom.y, entity_vector[i].RMBR.right_top.x, entity_vector[i].RMBR.right_top.y);
	}
	fclose(stdout);
}

void EntityInSCCToNewFormat(int &node_count, vector<Entity> &entity_vector, int &range, string filename, string newfilename)
{
	ReadEntityInSCCFromDisk(node_count, entity_vector, range, filename);
	string root = "data/";
	root += newfilename;
	char *ch1 = (char *)root.data();
	freopen(ch1, "w", stdout);

	printf("%d\n", entity_vector.size());
	for (int i = 0; i < entity_vector.size(); i++)
	{
		printf("%d,%d", i, entity_vector[i].IsSpatial);
		if (entity_vector[i].IsSpatial)
			printf(",%lf,%lf\n", entity_vector[i].location.x, entity_vector[i].location.y);
		else
			printf("\n");
	}
	fclose(stdout);
}


string getstring(const int i)
{
	stringstream newstr;
	newstr << i;
	return newstr.str();
}

int StringtoInt(string str)
{
	return atoi(str.c_str());
}

vector<string> split(string str, string pattern)
{
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}

// A recursive function used by topologicalSort
void TopologicalSortUtil(int v, vector<bool> &visited, queue<int> &queue, vector<vector<int>> &graph)
{
	// Mark the current node as visited.
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	for (int i = 0; i < graph[v].size(); i++)
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

void GenerateInedgeGraph(vector<vector<int>> &graph, vector<vector<int>> &in_edge_graph)
{
	int node_count = graph.size();
	in_edge_graph.resize(node_count);
	for (int i = 0; i < node_count; i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			int neighbor = graph[i][j];
			in_edge_graph[neighbor].push_back(i);
		}
	}
}

double randomGauss(double mean, double sigma, TRnd &rand)
{
	double v1, v2;
	double s;
	double x;

	do
	{
		v1 = rand.GetUniDev() * 2 - 1;
		v2 = rand.GetUniDev() * 2 - 1;
		s = v1*v1 + v2*v2;
	} while (s >= 1.0);

	x = v1 * sqrt(-2. * log(s) / s);

	/*  x is normally distributed with mean 0 and sigma 1.  */
	x = x * sigma + mean;

	return x;
}

int randomSkewed(int p, double HsubV, TRnd &rand)
{
	double r = rand.GetUniDev()*HsubV;

	double sum = 1.0; 
	int i = 1;
	while (sum<r){
		i++;
		sum += 1.0 / pow((double)i, p);
	}
	return i;
}