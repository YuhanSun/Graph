
// graph_2015_1_24_mfcDlg.cpp : implementation file
//
#include "stdafx.h"
#include "graph_2015_1_24_mfc.h"
#include "graph_2015_1_24_mfcDlg.h"
#include "afxdialogex.h"
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_DATA_SIZE = 10000000000;

//vector<vector<int>> graph = ReadVectorFromDisk("size/graph20.txt");
//vector<vector<int>> graph_v = ReadVectorFromDisk("size/graph19.txt");
vector<vector<int>> graph_v;
vector<vector<int>> graph_inedge_v;
vector<Entity> m_entity;
int m_range;
int m_node_count;
int m_edge_count;
double m_nonspatial_ratio = 0.2;

typedef RTree<int, double, 2, double> MyTree;
MyTree tree;

vector<int> graph_a[1048576];
int node_count = pow(2,23);
int range = -1;
//ofstream filetime;
TRnd rnd = time(0);

bool RBSearch_RTree_TransitiveClosure(int start_id, MyRect rect, vector<vector<int>> &transitive_closure);

bool RBSearch_RTree_TransitiveClosure(int start_id, MyRect rect, vector<vector<int>> &transitive_closure)
{
	ResetHitID(m_node_count);
	Rect search_rect = Rect(rect.left_bottom.x, rect.left_bottom.y, rect.right_top.x, rect.right_top.y);
	tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

	vector<int> hit_id = GetHitID();
	for (int i = 0; i < hit_id.size(); i++)
	{
		int id = hit_id[i];
		for (int j = 0; j < transitive_closure[start_id].size(); j++)
		{
			if (id == transitive_closure[start_id][j])
				return true;
		}
	}
	return false;

}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cgraph_2015_1_24_mfcDlg dialog



Cgraph_2015_1_24_mfcDlg::Cgraph_2015_1_24_mfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cgraph_2015_1_24_mfcDlg::IDD, pParent)
	, m_starttype(0)
	, m_entity_name(_T(""))
	, m_step(0)
	, m_startspatialrange(0)
	, m_endspatialrangeratio(0)
	, m_duplicate_times(0)
	, m_start_vertex(0)
	, m_status_value(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//  m_status_value = 0;
}

void Cgraph_2015_1_24_mfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_starttype);
	DDX_CBString(pDX, IDC_COMBO1, m_entity_name);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Text(pDX, IDC_EDITSpatialRangeRatioStep, m_step);
	DDX_Text(pDX, IDC_EDITStartSpatialRangeRatio, m_startspatialrange);
	DDX_Text(pDX, IDC_EDITEndSpatialRangeRatio, m_endspatialrangeratio);
	DDX_Text(pDX, IDC_EDIT_Duplicate_times, m_duplicate_times);
	DDX_Text(pDX, IDC_EDIT_startvertex, m_start_vertex);
}

BEGIN_MESSAGE_MAP(Cgraph_2015_1_24_mfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_test, &Cgraph_2015_1_24_mfcDlg::OnBnClickedtest)
	ON_BN_CLICKED(IDOK, &Cgraph_2015_1_24_mfcDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_HopTest, &Cgraph_2015_1_24_mfcDlg::OnBnClickedHoptest)
	ON_BN_CLICKED(IDC_SizeTest, &Cgraph_2015_1_24_mfcDlg::OnBnClickedSizetest)
	ON_BN_CLICKED(IDC_BUTTONGenerateGraph, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtongenerategraph)
	ON_BN_CLICKED(IDC_BUTTONRatioTest, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonratiotest)
//	ON_BN_CLICKED(IDC_BUTTONCOMPARISON, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtoncomparison)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cgraph_2015_1_24_mfcDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &Cgraph_2015_1_24_mfcDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTONSpatialFirst, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonspatialfirst)
	ON_BN_CLICKED(IDC_BUTTONSpatialFirstTest, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonspatialfirsttest)
	ON_BN_CLICKED(IDC_BUTTONSocialFirstTest, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonsocialfirsttest)
	ON_EN_CHANGE(IDC_EDITStartSpatialRangeRatio, &Cgraph_2015_1_24_mfcDlg::OnEnChangeEditstartspatialrangeratio)
	ON_EN_CHANGE(IDC_EDITSpatialRangeRatioStep, &Cgraph_2015_1_24_mfcDlg::OnEnChangeEditspatialrangeratiostep)
	ON_EN_CHANGE(IDC_EDITEndSpatialRangeRatio, &Cgraph_2015_1_24_mfcDlg::OnEnChangeEditendspatialrangeratio)
	ON_BN_CLICKED(IDC_BUTTONInitialize, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtoninitialize)
	ON_EN_CHANGE(IDC_EDIT_Duplicate_times, &Cgraph_2015_1_24_mfcDlg::OnEnChangeEditDuplicatetimes)
	ON_BN_CLICKED(IDC_BUTTONCOMPARISON, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtoncomparison)
	ON_BN_CLICKED(IDC_BUTTON_RMBR, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRmbr)
	ON_EN_CHANGE(IDC_EDIT_startvertex, &Cgraph_2015_1_24_mfcDlg::OnEnChangeEditstartvertex)
	ON_BN_CLICKED(IDC_RMBR_Initizlize, &Cgraph_2015_1_24_mfcDlg::OnBnClickedRmbrInitizlize)
//	ON_BN_CLICKED(IDC_BUTTON6, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_ReachQuery, &Cgraph_2015_1_24_mfcDlg::OnBnClickedReachquery)
ON_BN_CLICKED(IDC_BUTTON_Generation_Test, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGenerationTest)
ON_BN_CLICKED(IDC_BUTTON_Read, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRead)
END_MESSAGE_MAP()


// Cgraph_2015_1_24_mfcDlg message handlers

BOOL Cgraph_2015_1_24_mfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_combo.AddString(_T("entity1"));
	m_combo.AddString(_T("entity2"));
	m_combo.AddString(_T("entity3"));
	m_combo.SetCurSel(2);


//	ReadEntityFromDisk(node_count, entity_vector, range,"size/entity23.txt");
//	ReadArrayVectorFromDisk(graph_a, "size/graph20.txt");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cgraph_2015_1_24_mfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cgraph_2015_1_24_mfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cgraph_2015_1_24_mfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cgraph_2015_1_24_mfcDlg::OnBnClickedtest()
{
	graph_v.clear();
	m_node_count = pow(2, 20);
	m_edge_count = 1 * m_node_count;
	m_nonspatial_ratio = 0.4;
	Generate_Random_Vector(graph_v, m_node_count, m_edge_count, m_nonspatial_ratio);
	//Generate_Random_DAG(graph_v, m_node_count, m_edge_count, m_nonspatial_ratio);
	VectorToDisk(graph_v, "RMBR/Random/graph_20.txt");

	string path = "RMBR/Random/";
	string root = "data/";
	root += path + "spatial_transitive_closure.txt";
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);
	printf("%d %d\n", m_node_count, m_range);
	fclose(stdout);

	for (int i = 0; i < m_node_count; i++)
	{
		vector<int> transitive_closure_line;
		transitive_closure_line = GetTransitiveClosureLine(i, graph_v, m_entity);
		SpatialTransitiveClosureLine_To_Disk(transitive_closure_line, i, path + "spatial_transitive_closure.txt", m_node_count, m_nonspatial_ratio);
	}

	//vector<set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic_In_Set(graph_v, m_entity);
	//TransitiveClosureDynamic_To_Disk(transitive_closure_dynamic, 1000, "RMBR/Random/transitive_closure.txt");
	//vector<vector<int>> transitive_closure = GetTransitiveClosure(graph_v, m_entity);
	//TransitiveClosure_To_Disk(transitive_closure, 1000, "RMBR/Random/transitive_closure_20.txt");
	
	/*
	graph_v.clear();
	Generate_Vector_From_Edge(graph_v, pow(2, 14), pow(2, 14), "RMBR/14/graph_edge_14.txt", 0.4);
	VectorToDisk(graph_v, "RMBR/14/graph.txt");
	vector<set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic_In_Set(graph_v, m_entity);
	TransitiveClosureDynamic_To_Disk(transitive_closure_dynamic, 1000, "RMBR/14/transitive_closure.txt");
	*/
	/*
	graph_v.clear();
	Generate_Random_Vector(graph_v, pow(2,14), pow(2,14), 0.4);
	//Generate_Vector_Noback(graph_v, pow(2,14),pow(2,14) , 0.25, 0.25, 0.25, 0.4);
	VectorToDisk(graph_v, "RMBR/14_my/graph_14.txt");
	vector<set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic_In_Set(graph_v, m_entity);
	TransitiveClosureDynamic_To_Disk(transitive_closure_dynamic, 1000, "RMBR/14_my/transitive_closure.txt");*/
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}



void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	ofstream filetime("data/size/time.txt",ios::app);
	ofstream fileaverage("data/size/average_time.txt",ios::app);
	int start, time;

	int start_pow = 20;
	//for (int start_pow = 16; start_pow <= 16; start_pow++)
	//{
		stringstream stream;
		string str;
		stream << start_pow;
		stream >> str;
		string filename_entity = "size/entity" + str + ".txt";
		string filename_graph = "size/graph" + str + ".txt";

		int node_count = pow(2, start_pow);
		int range = 500;

		//vector<vector<int>> graph;
		//graph = ReadVectorFromDisk(filename_graph);
		//vector<int> graph[8388608 / 8];
		//ReadArrayVectorFromDisk(graph,filename_graph);

		int sum_time = 0;
		int sum_paths_count = 0;
		vector<int> * x;
		int * y;
		int z;
		TRnd Rnd = ::time(0);
		for (int i = 0; i < 10000000; i++)
		{
			int startnode_id = Rnd.GetUniDev()*node_count*0.5;

			int start = clock();
			x = &graph_a[startnode_id];
			y = &(graph_a[startnode_id][0]);
			z = *y;
			time = clock() - start;
			sum_time += time;
		}
		fileaverage << node_count << " "<< sum_time <<" " << x <<" "<<y<<" "<<z<<endl;
	//}
	filetime.close();
 	fileaverage.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//graph = ReadVectorFromDisk("size/graph16.txt");
	//graph_v = ReadVectorFromDisk("rect_size/graph9.txt");
	CString filter;
	filter = "文本文档(*.txt)|*.txt|PDF文档(*.pdf)|*.pdf||";
	CFileDialog dlg(true);//TRUE, NULL, NULL, OFN_HIDEREADONLY, filter
	dlg.DoModal();
	//if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		MessageBox(str);
	} 
	//CDialogTest dd;
	//dd.DoModal();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	vector<int> edge_type;
	edge_type.push_back(0);
	edge_type.push_back(2);
	edge_type.push_back(4);

	vector<bool> constraint_step;
	constraint_step.resize(4);
	constraint_step[0] = false;
	constraint_step[1] = false;
	constraint_step[2] = true;
	constraint_step[3] = false;

	vector<MyRect> constraint_rect;
	constraint_rect.resize(3);
	MyRect rect;
	rect.left_bottom.x = 0;
	rect.left_bottom.y = 0;
	rect.right_top.x = 300;
	rect.right_top.y = 300;
	constraint_rect[2] = rect;

	int startnode_id = rnd.GetUniDev()*graph_v.size()*0.5;
	vector<int> Paths;
	Paths.reserve(256 * 256 * 256 * 256 * 2 * 7);

	SetStartTime(clock());
	//FindQualifiedPaths(Paths, graph_v, startnode_id, 3, edge_type, constraint_step, constraint_rect, entity_vector);
	int returntime = clock() - GetStartTime();
	
	ofstream file("data/size/time.txt", ios::app);
	file << returntime << "  " << GetPathsCount() << endl;
	file.close();

	{
		vector<int> tem = Paths;
		Paths.swap(tem);
	}

	//filetime << startnode_id << " " << Paths.size() << " " << time << endl;
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here

}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedHoptest()
{
	// TODO: Add your control notification handler code here
	ofstream filetime, fileaveragetime;
	filetime.open("data/rect_size/time.txt");
	fileaveragetime.open("data/rect_size/average_time.txt");
	vector<int> edge_type;
	edge_type.push_back(0);
	edge_type.push_back(2);
	//edge_type.push_back(4);

	vector<bool> constraint_step;
	constraint_step.resize(3);
	constraint_step[0] = false;
	constraint_step[1] = false;
	constraint_step[2] = true;
	//constraint_step[3] = false;

	int recsize = 0;

	for (recsize = 50;recsize<=300;recsize+=25)
	{
		filetime << recsize << endl;

		vector<MyRect> constraint_rect;
		constraint_rect.resize(3);
		MyRect rect;
		rect.left_bottom.x = 0;
		rect.left_bottom.y = 0;
		rect.right_top.x = recsize;
		rect.right_top.y = recsize;
		constraint_rect[2] = rect;

		int sum_time = 0,sum_pathscount = 0;
		for (int j = 0; j < 30; j++)
		{
			int startnode_id = rnd.GetUniDev()*graph_v.size()*0.5;
			vector<int> Paths;
			int start, time;
			start = clock();
			//FindQualifiedPaths(Paths, graph_v, startnode_id, 3, edge_type, constraint_step, constraint_rect, entity_vector);
			time = clock() - start;
			filetime << startnode_id << " " << GetPathsCount() << " " << time << endl;
			sum_time += time;
			sum_pathscount += GetPathsCount();
		}
		filetime << endl;
		fileaveragetime << recsize << " " << sum_time << " " << sum_pathscount << endl;
	}
	filetime.close();
	fileaveragetime.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedSizetest()
{
	// TODO: Add your control notification handler code here
	ofstream filetime, fileaveragetime;
	filetime.open("data/rect_size/time.txt");
	fileaveragetime.open("data/rect_size/average_time.txt");
	vector<int> edge_type;
	edge_type.push_back(0);
	edge_type.push_back(2);
	edge_type.push_back(4);

	vector<bool> constraint_step;
	constraint_step.resize(4);
	constraint_step[0] = false;
	constraint_step[1] = false;
	constraint_step[2] = true;
	constraint_step[3] = false;

	int recsize = 0;

	for (recsize = 50; recsize <= 300; recsize += 25)
	{
		filetime << recsize << endl;

		vector<MyRect> constraint_rect;
		constraint_rect.resize(4);
		MyRect rect;
		rect.left_bottom.x = 0;
		rect.left_bottom.y = 0;
		rect.right_top.x = recsize;
		rect.right_top.y = recsize;
		constraint_rect[2] = rect;

		int sum_time = 0, sum_pathscount = 0;
		for (int j = 0; j < 30; j++)
		{
			int startnode_id = rnd.GetUniDev()*graph_v.size()*0.5;
			vector<int> Paths;
			int start, time;
			start = clock();
			//FindQualifiedPaths(Paths, graph_v, startnode_id, 3, edge_type, constraint_step, constraint_rect, entity_vector);
			time = clock() - start;
			filetime << startnode_id << " " << GetPathsCount() << " " << time << endl;
			sum_time += time;
			sum_pathscount += GetPathsCount();
		}
		filetime << endl;
		fileaveragetime << recsize << " " << sum_time << " " << sum_pathscount << endl;
	}
	filetime.close();
	fileaveragetime.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtongenerategraph()
{
	//Generate graphs with different ratio, mutiple by 2 from 32 to 256
	/*int node_count = pow(2, 20);

	double nonspatial_entity_ratio = 0.5;

	double a = 0.25, b = 0.25, c = 0.25;

	int start, finish, time;
	string filename;
	for (int ratio = 16; ratio <= 512; ratio *=2)
	{
		vector<vector<int>> graph_vector;
		graph_vector.resize(node_count);
		for (int i = 0; i < node_count; i++)
		{
			graph_vector[i].reserve(ratio * 2 * 2 * 2);
		}
		int real_ratio = ratio * 2;
		INT64 edge_count = node_count*ratio;
		Generate_Vector(graph_vector, node_count, edge_count, a, b, c, 0.5);
		stringstream stream;
		string str;
		stream << real_ratio;
		stream >> str;

		filename = "ratio/graph" + str + ".txt";
		VectorToDisk(graph_vector, filename);
	}
	*/

	//Generate 0.4-0.6 graph and entity
	/*
	vector<Entity> entity;
	GenerateEntity(pow(2, 23), entity, 10000, 0.2);
	EntityToDIsk(entity, 10000, "comparison-0.4-0.6/entity.txt");

	
	int node_count = pow(2, 23);
	INT64 edge_count = 128 * node_count;
	vector<vector<int>> graph;
	double a = 0.25, b = 0.25, c = 0.25;
	Generate_Vector(graph, node_count, edge_count, a, b, c, 0.4);
	VectorToDisk(graph, "comparison-0.4-0.6/graph.txt");
	*/
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonratiotest()
{
	ofstream filetime("data/ratio/time.txt", ios::app);
	ofstream fileaveragetime("data/ratio/average_time.txt", ios::app);

	int node_count = pow(2, 20);
	int range = 500;
	vector<Entity> entity;
	ReadEntityFromDisk(node_count, entity, range, "ratio/entity20.txt");
	//ReadEntityFromDisk(node_count, entity, range, "entity.txt");

	vector<int> edge_type;
	edge_type.push_back(0);
	edge_type.push_back(2);
	edge_type.push_back(4);

	vector<bool> spatial_step;
	spatial_step.resize(4);
	spatial_step[0] = false;
	spatial_step[1] = false;
	spatial_step[2] = true;
	spatial_step[3] = false;

	vector<MyRect> constraint_rect;
	constraint_rect.resize(4);
	MyRect rect;
	rect.left_bottom.x = 0;
	rect.left_bottom.y = 0;
	rect.right_top.x = 200;
	rect.right_top.y = 200;
	constraint_rect[2] = rect;

	for (int ratio = 1024; ratio <= 1024; ratio *= 2)
	{
		vector<vector<int>> graph;

		stringstream stream;
		string str;
		stream << ratio;
		stream >> str;

		string filename = "ratio/graph" + str + ".txt";

		graph = ReadVectorFromDisk(filename);
		//graph = ReadVectorFromDisk("graph.txt");

		TRnd rand = time(0);

		filetime << ratio << endl;

		int sum_time = 0, sum_pathscount = 0;
		for (int i = 0; i < 30; i++)
		{
			int startnodeid = rand.GetUniDev()*node_count*0.5;

			vector<int> paths;
			int start = clock();
			FindQualifiedPaths(paths, graph, startnodeid, 3, edge_type, spatial_step, constraint_rect, entity);
			int time = clock() - start;
			filetime << startnodeid << " " << GetPathsCount() << " " << time << endl;
			sum_time += time;
			sum_pathscount += GetPathsCount();
		}
		filetime << endl;
		fileaveragetime << ratio << " " << sum_time / 30 << " " << sum_pathscount / 30 << endl;
	}
	filetime.close();
	fileaveragetime.close();

}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
}


void Cgraph_2015_1_24_mfcDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	/*
	string filename;
	int i = m_combo.GetCurSel();
	if (i == 0)
		filename = "comparison/entity1.txt";
	if (i == 1)
		filename = "comparison/entity2.txt";
	if (i == 2)
		filename = "comparison/entity3.txt";
	m_entity.clear();
	ReadEntityFromDisk(node_count, m_entity, range, filename);
	tree.RemoveAll();
	for (int i = node_count / 2; i < node_count; i++)
	{
		Entity c_entity = m_entity[i];
		double min[2];
		double max[2];

		min[0] = c_entity.location.x;
		min[1] = c_entity.location.y;
		max[0] = c_entity.location.x;
		max[1] = c_entity.location.y;

		tree.Insert(min, max, i);
	}*/
}


void Cgraph_2015_1_24_mfcDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonspatialfirst()
{
	// TODO: Add your control notification handler code here

	ofstream filetime("data/comparison-0.4-0.6/time_spatial_first.txt", ios::app);
	ofstream fileaveragetime("data/comparison-0.4-0.6/average_time_spatial_first.txt", ios::app);

	filetime << "qualified_node_count " << "path_count " << "time_RTree" << "time_graph" << "sumtime" << endl;
	fileaveragetime << "spatial_ratio " << "rect_size " << "qualified_node_count " << "path_count " << "average_time_RTree" << "average_time_graph" << "average_time_sum" << endl;

	filetime << m_starttype << endl;
	fileaveragetime << "type=" << m_starttype << endl;

	vector<int> edge_type;
	edge_type.push_back(4);
	edge_type.push_back(0);

	double spatial_ratio = m_startspatialrange;
	int size;

	int end_type = m_starttype;
	vector<int> paths;

	for (; fabs(spatial_ratio - m_endspatialrangeratio) > 0.00000001; spatial_ratio += m_step)
	{
		size = m_range * sqrt(spatial_ratio);
		Rect search_rect = Rect(0, 0, size, size);
		filetime << spatial_ratio << " " << size << endl;
 
		int sum_time_all = 0, sum_time_RTree = 0, sum_time_graph = 0;
		int qualified_node_count;
		int pathcount;
		for (int i = 0; i < m_duplicate_times; i++)
		{
			ResetHitID(m_node_count*(1-m_nonspatial_ratio)*spatial_ratio * 2);
			pathcount = 0;
			int start = clock();
			tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
			vector<int> hit_id = GetHitID();
			int time_RTree = clock() - start;
			start = clock();
			for (int j = 0; j < hit_id.size(); j++)
			{
				FindQualifiedPaths(paths, graph_v, hit_id[j], end_type, 2, edge_type, m_entity);
				pathcount += GetPathsCount();
			}
			int time_graph = clock() - start;
			filetime << hit_id.size() << " " << pathcount << " " << time_RTree << " " << time_graph << " " << time_RTree + time_graph << endl;
			sum_time_RTree += time_RTree;
			sum_time_graph += time_graph;
			sum_time_all = sum_time_all + time_RTree + time_graph;
			qualified_node_count = hit_id.size();
		}
		filetime << endl;
		fileaveragetime << spatial_ratio << " " << size << " " << qualified_node_count << " " << pathcount << " " << sum_time_RTree / m_duplicate_times << " " << sum_time_graph / m_duplicate_times << " " << sum_time_all / m_duplicate_times << endl;
	}

	filetime << endl;
	fileaveragetime << endl;

	filetime.close();
	fileaveragetime.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonspatialfirsttest()
{
	// TODO: Add your control notification handler code here
	ofstream filetime("data/comparison-0.2-0.8/time_spatial_first.txt", ios::app);
	ofstream fileaveragetime("data/comparison-0.2-0.8/average_time_spatial_first.txt", ios::app);

	filetime << "qualified_node_count " << "path_count " << "time" << endl;
	fileaveragetime << "spatial_ratio " << "rect_size " << "qualified_node_count " << "path_count " << "average_time" << endl;

	filetime << m_starttype << endl;
	fileaveragetime << m_starttype << endl;

	vector<int> edge_type;
	edge_type.push_back(4);
	edge_type.push_back(0);

	double spatial_ratio = m_startspatialrange;
	int size;

	int end_type = m_starttype;
	vector<int> paths;

	for (; fabs(spatial_ratio - m_endspatialrangeratio) > 0.00000001; spatial_ratio += m_step)
	{
		size = m_range * sqrt(spatial_ratio);
		Rect search_rect = Rect(0, 0, size, size);
		filetime << spatial_ratio << " " << size << endl;

		int sum_time = 0;
		int qualified_node_count;
		int pathcount;
		//for (int i = 0; i < 20; i++)
		{
			ResetHitID(m_node_count*(1 - m_nonspatial_ratio)*spatial_ratio * 2);
			pathcount = 0;
			int start = clock();
			tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
			vector<int> hit_id = GetHitID();
			for (int j = 0; j < hit_id.size(); j++)
			{
				FindQualifiedPaths(paths, graph_v, hit_id[j], end_type, 2, edge_type, m_entity);
				pathcount += GetPathsCount();
			}
			int time = clock() - start;
			filetime << hit_id.size() << " " << pathcount << " " << time << endl;
			sum_time += time;
			qualified_node_count = hit_id.size();
		}
		filetime << endl;
		//fileaveragetime << spatial_ratio << " " << size << " " << qualified_node_count << " " << sum_time / 20 << endl;
	}

	filetime << endl;
	fileaveragetime << endl;

	filetime.close();
	fileaveragetime.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonsocialfirsttest()
{
	// TODO: Add your control notification handler code here
	ofstream filetime("data/comparison-0.2-0.8/time_social_first.txt", ios::app);
	ofstream fileaveragetime("data/comparison-0.2-0.8/average_time_social_first.txt", ios::app);

	filetime << "qualified_node_count " << "path_count " << "time" << endl;
	fileaveragetime << "spatial_ratio " << "rect_size " << "qualified_node_count " << "path_count " << "average_time" << endl;

	filetime << m_starttype << endl;
	fileaveragetime << m_starttype << endl;

	vector<int> edge_type;
	edge_type.push_back(0);
	edge_type.push_back(2);
	//edge_type.push_back(4);

	vector<bool> spatial_step;
	spatial_step.resize(4);
	spatial_step[0] = false;
	spatial_step[1] = false;
	spatial_step[2] = true;
	//spatial_step[3] = false;

	double spatial_ratio = m_startspatialrange;
	int size;

	int start_type = m_starttype;

	for (; fabs(spatial_ratio - m_endspatialrangeratio) > 0.00000001; spatial_ratio += m_step)
	{
		size = m_range * sqrt(spatial_ratio);

		filetime << spatial_ratio << " " << size << endl;

		vector<MyRect> constraint_rect;
		constraint_rect.resize(4);
		MyRect rect;
		rect.left_bottom.x = 0;
		rect.left_bottom.y = 0;
		rect.right_top.x = size;
		rect.right_top.y = size;
		constraint_rect[2] = rect;

		int sum_time = 0;
		int qualified_node_count;
		int pathcount;
//		for (int i = 0; i < 20; i++)
		{
			qualified_node_count = 0;
			pathcount = 0;
			int start = clock();
			for (int j = 0; j < node_count; j++)
			{
				if (m_entity[j].type == start_type)
				{
					qualified_node_count++;
					vector<int> paths;
					FindQualifiedPaths(paths, graph_v, j, 2, edge_type, spatial_step, constraint_rect, m_entity);
					pathcount += GetPathsCount();
					continue;
				}
			}
			int time = clock() - start;
			filetime << qualified_node_count << " " << pathcount << " " << time << endl;
			sum_time += time;
		}
		filetime << endl;
		//fileaveragetime << spatial_ratio << " " << size << " " << qualified_node_count << " " << pathcount << " " << sum_time / 20 << endl;
	}
	filetime << endl;
	fileaveragetime << endl;
	filetime.close();
	fileaveragetime.close();
}


void Cgraph_2015_1_24_mfcDlg::OnEnChangeEditstartspatialrangeratio()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void Cgraph_2015_1_24_mfcDlg::OnEnChangeEditspatialrangeratiostep()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void Cgraph_2015_1_24_mfcDlg::OnEnChangeEditendspatialrangeratio()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtoninitialize()
{
	// TODO: Add your control notification handler code here
	ReadEntityFromDisk(m_node_count, m_entity, m_range, "comparison-0.4-0.6/entity.txt");
	for (int i = m_node_count *m_nonspatial_ratio; i < m_node_count; i++)
	{
		Entity c_entity = m_entity[i];
		double min[2];
		double max[2];

		min[0] = c_entity.location.x;
		min[1] = c_entity.location.y;
		max[0] = c_entity.location.x;
		max[1] = c_entity.location.y;

		tree.Insert(min, max, i);
	}

	graph_v = ReadVectorFromDisk("comparison-0.4-0.6/graph.txt");
}


void Cgraph_2015_1_24_mfcDlg::OnEnChangeEditDuplicatetimes()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtoncomparison()
{
	// TODO: Add your control notification handler code here
	ofstream filetime("data/comparison-0.4-0.6/time_social_first.txt", ios::app);
	ofstream fileaveragetime("data/comparison-0.4-0.6/average_time_social_first.txt", ios::app);

	filetime << "qualified_node_count " << "path_count " << "time" << endl;
	fileaveragetime << "spatial_ratio " << "rect_size " << "qualified_node_count " << "path_count " << "average_time" << endl;

	filetime << "type=" << m_starttype << endl;
	fileaveragetime << "type=" << m_starttype << endl;

	vector<int> edge_type;
	edge_type.push_back(0);
	edge_type.push_back(2);
	//edge_type.push_back(4);

	vector<bool> spatial_step;
	spatial_step.resize(4);
	spatial_step[0] = false;
	spatial_step[1] = false;
	spatial_step[2] = true;
	//spatial_step[3] = false;

	double spatial_ratio = m_startspatialrange;
	int size;

	int start_type = m_starttype;

	for (; fabs(spatial_ratio - m_endspatialrangeratio) > 0.00000001; spatial_ratio += m_step)
	{
		size = m_range * sqrt(spatial_ratio);

		filetime << spatial_ratio << " " << size << endl;

		vector<MyRect> constraint_rect;
		constraint_rect.resize(4);
		MyRect rect;
		rect.left_bottom.x = 0;
		rect.left_bottom.y = 0;
		rect.right_top.x = size;
		rect.right_top.y = size;
		constraint_rect[2] = rect;

		int sum_time = 0;
		int qualified_node_count;
		int pathcount;
		for (int i = 0; i < m_duplicate_times; i++)
		{
			qualified_node_count = 0;
			pathcount = 0;
			int start = clock();
			for (int j = 0; j < node_count; j++)
			{
				if (m_entity[j].type == start_type)
				{
					qualified_node_count++;
					vector<int> paths;
					FindQualifiedPaths(paths, graph_v, j, 2, edge_type, spatial_step, constraint_rect, m_entity);
					pathcount += GetPathsCount();
					continue;
				}
			}
			int time = clock() - start;
			filetime << qualified_node_count << " " << pathcount << " " << time << endl;
			sum_time += time;
		}
		filetime << endl;
		fileaveragetime << spatial_ratio << " " << size << " " << qualified_node_count << " " << pathcount << " " << sum_time / m_duplicate_times << endl;
	}
	filetime << endl;
	fileaveragetime << endl;
	filetime.close();
	fileaveragetime.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRmbr()
{
	m_range = 1000;
	int node_count = pow(2, 21);
	INT64 edge_count = 1 * node_count;

	string path = "RMBR/2097152_21_1/";

	//vector<Entity> entity;
	GenerateEntity(node_count, m_entity, m_range, 0.4);
	EntityToDIsk(m_entity, m_range, path + "entity.txt");

	
	//vector<vector<int>> graph;
	double a = 0.25, b = 0.25, c = 0.25;
	Generate_Vector_Noback(graph_v, node_count, edge_count, a, b, c, 0.4);
	VectorToDisk(graph_v, path + "graph.txt");
	
	edge_count = GetEdgeCount();

	//ReadEntityFromDisk(m_node_count, m_entity, range, "RMBR/entity.txt");

	//graph_v = ReadVectorFromDisk("RMBR/graph.txt");

	GenerateRMBR(m_entity, graph_v);

	RMBR_To_Disk(m_entity, m_range, path + "RMBR.txt");

	vector<set<int>> transitive_closure = GetTransitiveClosureDynamic_In_Set(graph_v, m_entity);
	SpatialTransitiveClosureDynamic_To_Disk(transitive_closure, m_range, path + "spatial_transitive_closure.txt", m_entity);

	//vector<vector<int>> transitive_closure = GetTransitiveClosure(graph_v, m_entity);
	/*
	string root = "data/";
	root += path + "spatial_transitive_closure.txt";
	char *ch = (char *)root.data();
	freopen(ch, "w", stdout);
	printf("%d %d\n", node_count, m_range);
	fclose(stdout);

	for (int i = 0; i < node_count; i++)
	{
		vector<int> transitive_closure_line;
		transitive_closure_line = GetTransitiveClosureLine(i, graph_v, m_entity);
		SpatialTransitiveClosureLine_To_Disk(transitive_closure_line, i, path + "spatial_transitive_closure.txt", m_entity);
	}
	*/
	//SpatialTransitiveClosure_To_Disk(transitive_closure, m_range, "RMBR/65536_16_1/spatial_transitive_closure.txt", m_entity);
	

	//ReadEntityFromDisk(m_node_count, m_entity, range, "RMBR/entity.txt");

	/*tree.RemoveAll();

	for (int i = 0; i < m_node_count; i++)
	{
		double min[2];
		double max[2];

		min[0] = m_entity[i].location.x;
		min[1] = m_entity[i].location.y;
		max[0] = m_entity[i].location.x;
		max[1] = m_entity[i].location.y;

		tree.Insert(min, max, i);
	}*/

	//ReadRmbr(m_entity, range, "RMBR/RMBR.txt");

	//graph_v = ReadVectorFromDisk("RMBR/graph.txt");

	//vector<vector<int>> transitive_closure = GetTransitiveClosure(graph_v, m_entity);
	//vector<MyRect> RMBR = GetRMBRFromTransitiveClosure(transitive_closure, m_entity);
	//TransitiveClosure_To_Disk(transitive_closure, range, "RMBR/transitive_closure.txt");
	//TransitiveClosureRMBR_To_Disk(RMBR, range, "RMBR/Transitive_closure_RMBR.txt");

	/*
	MyRect query_rect;
	query_rect.left_bottom.x = 0;
	query_rect.left_bottom.y = 0;
	query_rect.right_top.x = 50;
	query_rect.right_top.y = 50;

	for (int start_id = 0; start_id < node_count; start_id++)
	{
		Initialize_Visited_vertex();

		bool result1 = ReachabilityQuery(m_entity, graph_v, start_id, query_rect);

		bool result2 = RBSearch_RTree_TransitiveClosure(start_id, query_rect, transitive_closure);

		if (result1 == result2)
			continue;

		MessageBox(_T("false"), _T("Error"), MB_OK);

	}

	
	Initialize_Visited_vertex();
	bool result1 = ReachabilityQuery(m_entity, graph_v, m_start_vertex, query_rect);

	bool result2 = RBSearch_RTree_TransitiveClosure(m_start_vertex, query_rect, transitive_closure);
	
	if (result1 == result2)
		MessageBox(_T("true"), _T("Error"), MB_OK);
	else
		MessageBox(_T("false"), _T("Error"), MB_OK);

*/
	
	/*for (int i = 0; i < m_entity.size(); i++)
	{
		if ((m_entity[i].RMBR.left_bottom.x - RMBR[i].left_bottom.x)>0.000001 || (m_entity[i].RMBR.left_bottom.y - RMBR[i].left_bottom.y) > 0.000001
			|| (m_entity[i].RMBR.right_top.x - RMBR[i].right_top.x) > 0.000001 || (m_entity[i].RMBR.right_top.y - RMBR[i].right_top.y) > 0.000001)
			int a = false;
	}*/
}


void Cgraph_2015_1_24_mfcDlg::OnEnChangeEditstartvertex()
{
	UpdateData(true);
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedRmbrInitizlize()
{
	m_entity.clear();
	graph_v.clear();
	ReadEntityFromDisk(m_node_count, m_entity, range, "RMBR/entity.txt");
	ReadRmbr(m_entity, range, "RMBR/RMBR.txt");
	graph_v = ReadVectorFromDisk("RMBR/graph.txt");
}




void Cgraph_2015_1_24_mfcDlg::OnBnClickedReachquery()
{
	MyRect rect;
	rect.left_bottom.x = 0;
	rect.left_bottom.y = 0;
	rect.right_top.x = 70;
	rect.right_top.y = 70;

	ofstream file_result("data/RMBR/query_result.txt");
	for (int i = 0; i < m_node_count; i++)
	{
		Initialize_Visited_vertex();
		bool result = ReachabilityQuery(m_entity, graph_v, i, rect);
		if (result)
			file_result << "true\n";
		else
			file_result << "false\n";
	}
	file_result.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGenerationTest()
{
	m_range = 1000;
	m_nonspatial_ratio = 0.4;
	int node_count = pow(2, 15);
	INT64 edge_count = 1 * node_count;
	string path = "RMBR/test/";

	vector<Entity> p_entity;

	GenerateEntity(node_count, p_entity, m_range, m_nonspatial_ratio);
	//EntityToDIsk(p_entity, m_range, path + "entity.txt");

	vector<vector<int>> p_graph_v, p_graph_inedge_v;
	double a = 0.25, b = 0.25, c = 0.25;
	//graph_v.clear();
	//graph_inedge_v.clear();

	Generate_Vector_Noback_In_Edge(p_graph_v, p_graph_inedge_v, node_count, edge_count, a, b, c, m_nonspatial_ratio);
	int count = GetEdgeCount();
	//VectorToDisk(p_graph_v, path + "graph.txt");
	int start = clock();
	vector<vector<int>> transitive_closure = GetTransitiveClosure(p_graph_v, p_entity);
	int time1 = clock() - start;

	//TransitiveClosure_To_Disk(transitive_closure, m_range, path + "transitive_closure.txt");

	start = clock();
	vector<set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic_In_Set(p_graph_v, p_entity);
	int time2 = clock() - start;

	start = clock();
	vector<hash_set<int>> hs_transitive_closure_dynamic = GetTransitiveClosureDynamic(p_graph_v, p_entity);
	int time3 = clock() - start;

	//TransitiveClosureDynamic_To_Disk(transitive_closure_dynamic, m_range, path + "transitive_closure_dynamic.txt");

	/*for (int i = 0; i < transitive_closure.size(); i++)
	{
		if (transitive_closure[i].size() != transitive_closure_dynamic[i].size())
		{
			CString str;
			str.Format(TEXT("%d"),i);
			MessageBox(str);
		}
		set<int>::iterator end = transitive_closure_dynamic[i].end();
		for (int j = 0; j < transitive_closure[i].size(); j++)
		{
			if (transitive_closure_dynamic[i].find(transitive_closure[i][j]) == end)
			{
				CString str;
				str.Format(TEXT("%d     "), TEXT("%d"), i, j);
				MessageBox(str);
			}
		}
	}*/
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRead()
{
	string path = "RMBR/test/";

	ReadEntityFromDisk(m_node_count, m_entity, m_range, path + "entity.txt");
	graph_v = ReadVectorFromDisk(path + "graph.txt");
	vector<hash_set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic(graph_v, m_entity);
	TransitiveClosureDynamic_To_Disk(transitive_closure_dynamic, m_range, path + "transitive_closure_dynamic.txt");
}
