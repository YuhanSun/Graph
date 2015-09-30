﻿
// graph_2015_1_24_mfcDlg.cpp : implementation file
//
#include "stdafx.h"
#include "graph_2015_1_24_mfc.h"
#include "graph_2015_1_24_mfcDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_DATA_SIZE = 10000000000;

//vector<vector<int>> graph = ReadVectorFromDisk("size/graph20.txt");
//vector<vector<int>> graph_v = ReadVectorFromDisk("size/graph19.txt");
vector<vector<int>> m_graph_v;
vector<vector<int>> m_graph_inedge_v;
vector<Entity> m_entity;
int m_range;
int m_node_count;
int m_edge_count;
double m_nonspatial_ratio;
string m_path;


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

ON_BN_CLICKED(IDC_ReachQuery, &Cgraph_2015_1_24_mfcDlg::OnBnClickedReachquery)
ON_BN_CLICKED(IDC_BUTTON_Generation_Test, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGenerationTest)
ON_BN_CLICKED(IDC_BUTTON_Read, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRead)
ON_BN_CLICKED(IDC_BUTTON_DAG_Generate, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonDagGenerate)
ON_BN_CLICKED(IDC_BUTTON_Entity, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonEntity)
ON_BN_CLICKED(IDC_BUTTON_RandomGraph_Generate, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRandomgraphGenerate)
ON_BN_CLICKED(IDC_BUTTON_RealData_Generate, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRealdataGenerate)
ON_BN_CLICKED(IDC_BUTTON_Gen_Entity, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGenEntity)
ON_BN_CLICKED(IDC_BUTTON_Spatial_Tran, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonSpatialTran)
ON_BN_CLICKED(IDC_BUTTON_Transform, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonTransform)
ON_BN_CLICKED(IDC_BUTTON_Graph_Transform, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGraphTransform)
ON_BN_CLICKED(IDC_BUTTON_Reserse_Transform, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonReserseTransform)
ON_BN_CLICKED(IDC_BUTTONGrid_Generate, &Cgraph_2015_1_24_mfcDlg::OnBnClickedButtongridGenerate)
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
	vector<Entity> entity1,entity2;
	ReadEntityInSCCFromDisk(m_node_count, entity1, m_range, "Real_Data/citeseer/Random_spatial_distributed/20/entity.txt");
	ReadEntityInSCCFromDisk(m_node_count, entity2, m_range, "Real_Data/citeseer/Random_spatial_distributed/20/entity.txt");

	vector<set<int>> graph;
	ReadArbitaryGraphFromDisk(graph, m_node_count, "Real_Data/citeseer/graph.txt");
	int start = clock();
	GenerateRMBR(entity1, graph);
	int time = clock() - start;
	start = clock();
	GenerateRMBRByQueue(entity2, graph);
	time = clock() - start;

	EntityInSCC_To_Disk(entity1, m_range, "Real_Data/citeseer/entity1.txt");
	EntityInSCC_To_Disk(entity1, m_range, "Real_Data/citeseer/entity2.txt");

	for (int i = 0; i < entity1.size(); i++)
	{
		if (abs(entity1[i].RMBR.left_bottom.x - entity2[i].RMBR.left_bottom.x) > 0.00000001)
		{
			CString str;
			str.Format(TEXT("%d left_bottom.x"), i);
			MessageBox(str);
		}
		if (abs(entity1[i].RMBR.left_bottom.y - entity2[i].RMBR.left_bottom.y) > 0.00000001)
		{
			CString str;
			str.Format(TEXT("%d left_bottom.y"), i);
			MessageBox(str);
		}
		if (abs(entity1[i].RMBR.right_top.x - entity2[i].RMBR.right_top.x) > 0.00000001)
		{
			CString str;
			str.Format(TEXT("%d right_top.x"), i);
			MessageBox(str);
		}
		if (abs(entity1[i].RMBR.right_top.y - entity2[i].RMBR.right_top.y) > 0.00000001)
		{
			CString str;
			str.Format(TEXT("%d right_top.y"), i);
			MessageBox(str);
		}
	}
}

 
void Cgraph_2015_1_24_mfcDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}



void Cgraph_2015_1_24_mfcDlg::OnBnClickedButton1()
{
	m_range = 1000;
	vector<set<int>> SCC;
	m_node_count = pow(2, 18);
	m_path = "RMBR/18_16/";
	m_edge_count = 16 * m_node_count;


	//GenerateArbitaryGraph(graph_entity, m_node_count, m_edge_count,0.25, 0.25, 0.25);
	//ArbitaryGraphToDisk(graph_entity, path + "graph_entity.txt");

	//graph_entity.resize(m_node_count);
	//ReadGraphFromEdge(graph_entity, path + "edgelist.txt");

	//ReadArbitaryGraphFromDisk(graph_entity, m_node_count, m_path + "graph_entity.txt");

	ReadSCC(SCC, m_path + "SCC.txt");

	//ReadEntityInSCCFromDisk(m_node_count, m_entity, m_range, path + "entity.txt");

	GenerateEntityInSCC(m_node_count, m_entity, m_range, 0.4);
	ConnectSCCEntity(m_entity, SCC);

	GenerateRMBR(m_entity, m_graph_v);

	EntityInSCCSeperate_To_Disk(m_entity, m_range, m_path);

	vector<set<int>> graph_SCC;
	graph_SCC.resize(SCC.size());

	GenerateSCCGraph(m_entity, m_graph_v, graph_SCC);

	ArbitaryGraphToDisk(graph_SCC, m_path + "graph_SCC.txt");


	vector<set<int>> transitive_closure = GetTransitiveClosureDynamic_In_Set(graph_SCC);
	TransitiveClosureDynamic_To_Disk(transitive_closure, 1000, m_path + "transitive_closure.txt");
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

	int startnode_id = rnd.GetUniDev()*m_graph_v.size()*0.5;
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
			int startnode_id = rnd.GetUniDev()*m_graph_v.size()*0.5;
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
			int startnode_id = rnd.GetUniDev()*m_graph_v.size()*0.5;
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
				FindQualifiedPaths(paths, m_graph_v, hit_id[j], end_type, 2, edge_type, m_entity);
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
				FindQualifiedPaths(paths, m_graph_v, hit_id[j], end_type, 2, edge_type, m_entity);
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
					FindQualifiedPaths(paths, m_graph_v, j, 2, edge_type, spatial_step, constraint_rect, m_entity);
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

	m_graph_v = ReadVectorFromDisk("comparison-0.4-0.6/graph.txt");
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
					FindQualifiedPaths(paths, m_graph_v, j, 2, edge_type, spatial_step, constraint_rect, m_entity);
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
	EntityToDisk(m_entity, m_range, path + "entity.txt");

	
	//vector<vector<int>> graph;
	double a = 0.25, b = 0.25, c = 0.25;
	Generate_Vector_Noback(m_graph_v, node_count, edge_count, a, b, c, 0.4);
	VectorToDisk(m_graph_v, path + "graph.txt");
	
	edge_count = GetEdgeCount();

	//ReadEntityFromDisk(m_node_count, m_entity, range, "RMBR/entity.txt");

	//graph_v = ReadVectorFromDisk("RMBR/graph.txt");

	GenerateRMBR(m_entity, m_graph_v);

	RMBR_To_Disk(m_entity, m_range, path + "RMBR.txt");

	vector<set<int>> transitive_closure = GetTransitiveClosureDynamic_In_Set(m_graph_v);
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
	m_graph_v.clear();
	ReadEntityFromDisk(m_node_count, m_entity, range, "RMBR/entity.txt");
	ReadRmbr(m_entity, range, "RMBR/RMBR.txt");
	m_graph_v = ReadVectorFromDisk("RMBR/graph.txt");
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
		bool result = ReachabilityQuery(m_entity, m_graph_v, i, rect);
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
	vector<vector<int>> transitive_closure = GetTransitiveClosure(p_graph_v);
	int time1 = clock() - start;

	//TransitiveClosure_To_Disk(transitive_closure, m_range, path + "transitive_closure.txt");

	start = clock();
	vector<set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic_In_Set(p_graph_v);
	int time2 = clock() - start;

	start = clock();
	vector<hash_set<int>> hs_transitive_closure_dynamic = GetTransitiveClosureDynamic(p_graph_v);
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
	m_graph_v = ReadVectorFromDisk(path + "graph.txt");
	vector<hash_set<int>> transitive_closure_dynamic = GetTransitiveClosureDynamic(m_graph_v);
	TransitiveClosureDynamic_To_Disk(transitive_closure_dynamic, m_range, path + "transitive_closure_dynamic.txt");
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonDagGenerate()
{
	/*
	for (int k = 5; k < 9; k++)
	{
		m_node_count = pow(2, 18);
		int ratio = k;
		m_edge_count = ratio * m_node_count;
		vector<set<int>> dag;
		m_path = "RMBR/DAG/18_"+getstring(ratio)+"/";

		GenerateDAG(dag, m_node_count, m_edge_count);
		ArbitaryGraphToDisk(dag, m_path + "/dag.txt");

		//ReadArbitaryGraphFromDisk(dag, m_node_count, m_path + "dag.txt");
		ofstream ofile(m_path + "transitive_closure.txt", ios::out);
		ofile.close();
		for (int i = 0; i < m_node_count; i++)
		{
			vector<int> current_tran = GetTransitiveClosureLineArbitary(i, dag);
			TransitiveClosureLine_To_Disk(current_tran, i, m_path + "/transitive_closure.txt", m_node_count);
		}
		GenerateEntityInSCC(m_node_count, m_entity, 1000, 0.4);
		GenerateRMBR(m_entity, dag);
		EntityInSCCSeperate_To_Disk(m_entity, 1000, m_path);
	}
	*/
	
	string m_path = "Real_Data/Patents";
	m_nonspatial_ratio = 0.8;
	//hash_map<int, int> id_index;
	vector<vector<int>> real_graph;
	
	
	//ReadID(id_index, m_path + "/entity_id.txt");
	//ReadArbitaryGraphFromDisk(real_graph, id_index, m_path + "/edgelist.txt", 5);
	ReadGraphFromEdge(real_graph, m_node_count, m_path + "/edgelist.txt", 5);

	
	//m_node_count = real_graph.size();
	m_range = 1000;

	//ArbitaryGraphEdgeToDisk(real_graph, m_path + "/transform/edgelist.txt");

	GenerateEntityInSCC(m_node_count, m_entity, m_range, m_nonspatial_ratio);
	GenerateRMBR(m_entity, real_graph);
	EntityInSCCSeperate_To_Disk(m_entity, m_range, m_path + "/Random_spatial_distributed/80");

	/*for (int i = 0; i < m_node_count; i++)
	{
		vector<int> trans = GetTransitiveClosureLine(i, real_graph);
		TransitiveClosureLine_To_Disk(trans, i, m_path + "/transform/transitive_closure.txt", m_node_count);
	}*/



}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonEntity()
{
	// TODO: Add your control notification handler code here
	m_node_count = pow(2, 18);
	int ratio = 1;
	m_edge_count = ratio * m_node_count;
	m_range = 1000;
	m_nonspatial_ratio = 0.4;
	vector<set<int>> dag;
	m_path = "RMBR/DAG/18_4/";

	ReadArbitaryGraphFromDisk(dag, m_node_count, m_path + "dag.txt");

	GenerateEntityInSCC(m_node_count, m_entity, m_range, m_nonspatial_ratio);

	GenerateRMBR(m_entity, dag);

	EntityInSCCSeperate_To_Disk(m_entity, m_range, "RMBR/DAG/18_4");
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRandomgraphGenerate()
{
	vector<set<int>> graph_entity, SCC, graph_SCC;
	for (int k = 6;k<11 ; k++)
	{
		m_range = 1000;
		m_node_count = pow(2, 18);
		int ratio = k;
		m_edge_count = ratio * m_node_count;
		
		m_path = "RMBR/18_" + getstring(ratio);
		
		
		//GenerateArbitaryGraph(graph_entity, m_node_count, m_edge_count);
		//ArbitaryGraphToDisk(graph_entity, m_path + "/graph_entity.txt");

		/*
		for (int i = 0; i < m_node_count; i++)
		{
			graph_entity[i].clear();
		}
		graph_entity.clear();*/
		
		ReadArbitaryGraphFromDisk(graph_entity, m_node_count, m_path + "/graph_entity.txt");

		ReadSCC(SCC, m_path + "/SCC.txt");

		GenerateEntityInSCC(m_node_count, m_entity, m_range, 0.4);
		ConnectSCCEntity(m_entity, SCC);

		GenerateRMBR(m_entity, graph_entity);

		EntityInSCCSeperate_To_Disk(m_entity, m_range, m_path);

		graph_SCC.resize(SCC.size());

		GenerateSCCGraph(m_entity, graph_entity, graph_SCC);

		ArbitaryGraphToDisk(graph_SCC, m_path + "/graph_SCC.txt");
		
	

		//ReadArbitaryGraphFromDisk(graph_SCC, m_node_count, m_path + "/graph_SCC.txt");

		for (int i = 0; i < m_node_count; i++)
		{
			vector<int> transitive_closure = GetTransitiveClosureLineArbitary(i, graph_SCC);
			TransitiveClosureLine_To_Disk(transitive_closure, i, m_path + "/transitive_closure.txt", m_node_count);
		}

		for (int i = 0; i < m_node_count; i++)
		{
			graph_entity[i].clear();
		}
		graph_entity.clear();

		for (int i = 0; i < SCC.size(); i++)
		{
			SCC[i].clear();
			graph_SCC[i].clear();
		}
		SCC.clear();
		graph_SCC.clear();
		m_entity.clear();
	}
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonRealdataGenerate()
{
	//string m_path = "Real_Data/WikiTalk";
	vector<string> filename;
	//filename.push_back("citeseer");
	//filename.push_back("citeseerx");
	//filename.push_back("go_uniprot");
	//filename.push_back("uniprotenc_22m");
	//filename.push_back("uniprotenc_100m");
	//filename.push_back("uniprotenc_150m");
	filename.push_back("Patents");

	TRnd Rnd = time(0);
	int testingcount = 1000;

	for (int i = 0; i < filename.size(); i++)
	{
		string m_path = "Real_Data/" + filename[i];
		vector<vector<int>> real_graph;

		ReadArbitaryGraphFromDisk(real_graph, m_node_count, m_path + "/graph.txt");
		int start = clock();
		double total_count = 0;
		//for (int j = 0; j < testingcount; j++)
		vector<int> trans;
		int id = 225232;
		
		//int id = Rnd.GetUniDev()*m_node_count;
		trans = GetTransitiveClosureLineArbitary(id, real_graph);
		//total_count += trans.size();
		//TransitiveClosureLine_To_Disk(trans, i, m_path + "/transitive_closure.txt", m_node_count);
		
		int time = clock() - start;
		//ofstream file("data/Real_Data/"+filename[i]+"/average_transitive_size.txt", ios::app);
		ofstream file("data/Real_Data/" + filename[i] + "/reachable_vertices.csv");
		//file << " building transitive closure time: " << time;
		//file << testingcount << "\t" << total_count / testingcount;
		file << trans.size() << endl;
		for (int j = 0; j < trans.size(); j++)
			file << trans[j] << "\t";
		file << endl;
		
		file.close();
	}
	

	

	//ReadArbitaryGraphFromDisk(real_graph, m_node_count, m_path + "/SCC_Graph.txt");
	//ReadGraphFromEdge(real_graph, m_node_count, m_path + "/edgelist.txt",5);
	/*
	//ReadID(id_index, m_path + "/entity_id.txt");
	//ReadArbitaryGraphFromDisk(real_graph, id_index, m_path + "/edgelist.txt", 5);
	ReadGraphFromEdge(real_graph, m_node_count, m_path + "/WikiTalk.txt", 3);
	//ArbitaryGraphToDisk(real_graph, m_path + "/graph_entity.txt");

	vector<set<int>> SCC;
	ReadSCC(SCC, m_path + "/SCC.txt");

	//m_node_count = real_graph.size();
	m_range = 1000;

	//ArbitaryGraphEdgeToDisk(real_graph, m_path + "/transform/edgelist.txt");

	GenerateEntityInSCC(m_node_count, m_entity, m_range, m_nonspatial_ratio);
	ConnectSCCEntity(m_entity, SCC);

	vector<set<int>> SCC_Graph;
	SCC_Graph.resize(SCC.size());
	GenerateSCCGraph(m_entity, real_graph, SCC_Graph);
	ArbitaryGraphToDisk(SCC_Graph, m_path + "/SCC_Graph.txt");*/


	
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGenEntity()
{
	vector<pair<string, int>> graphs;
	graphs.push_back(make_pair("citeseer", 693947));
	graphs.push_back(make_pair("citeseerx", 6540401));
	graphs.push_back(make_pair("go_uniprot", 6967956));
	graphs.push_back(make_pair("Patents", 3774768));
	graphs.push_back(make_pair("uniprotenc_22m", 1595444));
	graphs.push_back(make_pair("uniprotenc_100m", 16087295));
	graphs.push_back(make_pair("uniprotenc_150m", 25037600));
	//graphs.push_back(make_pair("uniprotenc_100m_new", 16087295));

	m_range = 1000;
	vector<set<int>> graph;

	ofstream ofile("data/Real_Data/RMBR_construct_time_new.txt", ios::app);

	//for (int j = 0; j < graphs.size(); j++)
	int j = 3;
	{
		graph.clear();
		m_path = "Real_Data/" + graphs[j].first;
		m_node_count = graphs[j].second;

		ofile << graphs[j].first << endl;
		ofile << "ratio\t" << "RMBR_size\n";
		ReadArbitaryGraphFromDisk(graph, m_node_count, m_path + "/graph.txt");
		for (int times = 0; times < 10; times++)
		{
			for (int i = 20; i < 100; i += 20)
			{
				m_nonspatial_ratio = i / 100.0;
				m_entity.clear();
				//GenerateEntityInSCC(m_node_count, m_entity, m_range, m_nonspatial_ratio);
				//GenerateRMBR(m_entity, graph);
				ReadEntityInSCCFromDisk(m_node_count, m_entity, m_range, m_path + "/Random_spatial_distributed/" + getstring(i) + "/entity.txt");
				/*long size = 0;
				for (int k = 0; k < m_entity.size(); k++)
				{
					if (abs(m_entity[k].RMBR.left_bottom.x + 1) < 0.000000001)
						continue;
					double x = m_entity[k].RMBR.left_bottom.x;
					size += sizeof(x) * 4;
				}
				ofile << i << "\t" << size << endl;*/
				long start = clock();
				GenerateRMBR(m_entity, graph);
				long time = clock() - start;
				ofile << i << "\t" << time << endl;
				//EntityInSCC_To_Disk(m_entity, m_range, m_path + "/Random_spatial_distributed/" + getstring(i) + "/entity.txt");
			}
			ofile << endl;
		}
		
	}
	ofile.close();
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonSpatialTran()
{
	string m_path = "Real_Data/Patents";
	m_node_count = 3774768;

	vector<vector<int>> tran;
	ReadTransitiveClosureFromDisk(tran, m_node_count, m_path + "/transitive_closure.txt");

	for (int i = 20; i < 100; i += 20)
	{
		string entity_path = m_path + "/Random_spatial_distributed/" + getstring(i);
		string spatial_tran_path = "data/Real_Data/Patents/Random_spatial_distributed/" + getstring(i) + "/spatial_transitive_closure.txt";
		m_entity.clear();
		ReadEntityInSCCSeperateFromDisk(m_node_count, m_entity, m_range, entity_path);
		
		
		char *ch = (char*)spatial_tran_path.data();
		freopen(ch, "w", stdout);
		for (int j = 0; j < m_node_count; j++)
		{
			vector<int> line;
			line.reserve(tran[j].size());
			for (int k = 0; k < tran[j].size(); k++)
			{
				int end_id = tran[j][k];
				if (m_entity[end_id].IsSpatial)
					line.push_back(end_id);
			}
			printf("%d %d ", j, line.size());
			for (int k = 0; k < line.size(); k++)
				printf("%d ", line[k]);
			printf("\n");
		}
		fclose(stdout);
	}
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonTransform()
{
	vector<string> filename;
	//filename.push_back("test.txt");
	//filename.push_back("citeseer");
	//filename.push_back("citeseerx");
	//filename.push_back("go_uniprot");
	//filename.push_back("uniprotenc_22m");
	//filename.push_back("uniprotenc_100m");
	//filename.push_back("uniprotenc_150m");

	string infilepath = "D:/grail-master/grail-master/Downloads";

	for (int i = 0; i < filename.size(); i++)
	{
		string path = infilepath + "/" + filename[i] + ".txt";
		char* ch = (char*)path.data();
		freopen(ch, "r", stdin);
		string line;
		SSIZE_T read;
		if(!getline(cin, line))
			MessageBox(_T("format mismatch"), _T("Error"), MB_OK);
		scanf("%d\n", &m_node_count);

		vector<set<int>> graph;
		graph.resize(m_node_count);

		while (getline(cin, line))
		{
			int index = line.find(':');
			string start_str = line.substr(0, index);
			int start = atoi((char*)start_str.data());
			line = line.substr(index + 2, line.length());

			if (line.length() == 1)
				continue;
			else
			{
				while (true)
				{
					index = line.find(' ');
					if (index == -1)
						break;
					else
					{
						string end_str = line.substr(0, index);
						int end = atoi((char*)end_str.data());
						graph[start].insert(end);
						line = line.substr(index + 1, line.length());
					}
				}
			}
		}
		fclose(stdin);

		string outfilename = "Real_Data/" + filename[i] + "/graph.txt";
		ArbitaryGraphToDisk(graph, outfilename);

		for (int j = 0; j < m_node_count; j++)
		{
			graph[j].clear();
		}
		graph.clear();
	}
}


void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonGraphTransform()
{
	string path = "Real_Data/Patents";
	
	//ArbitaryGraphToDisk(graph, path + "/graph.txt");
	vector<vector<int>> graph;
	ReadArbitaryGraphFromDisk(graph, m_node_count, path + "/graph.txt");

	/*for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			if (i == graph[i][j])
			{
				CString str;
				str.Format(_T("%d"), i);
				MessageBox(str);
			}
		}
	}*/

	int edge_count = 0;
	for (int i = 0; i < graph.size(); i++)
	{
		edge_count += graph[i].size();
	}

	string w_path = "data/" + path + "/graph_PrunedLabel.txt";
	char *ch = (char*)w_path.data();
	freopen(ch, "w", stdout);
	printf("%d %d\n", graph.size(), edge_count);
	for (int i = 0; i < graph.size(); i++)
	{
		if (graph[i].size() != 0)
		{
			printf("%d", graph[i][0] + 1);
			for (int j = 1; j < graph[i].size(); j++)
			{
				printf(" %d", graph[i][j] + 1);
			}
		}
		printf("\n");
	}
	fclose(stdout);
	/*for (int i = 0; i < graph.size(); i++)
	{
		if (graph[i].size() != 0)
		{
			set<int>::iterator iter = graph[i].begin();
			printf("%d", *iter);
			set<int>::iterator end = graph[i].end();
			iter++;
			while (iter != end)
			{
				printf(" %d", *iter);
				iter++;
			}
		}
		printf("\n");
	}
	fclose(stdout);*/
}




void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtonReserseTransform()
{
	ifstream infile("D:/PrunedLabeling-master/PrunedLabeling-master/sample.graph");
	string line;
	int V, E;
	stringstream ss;
	getline(infile, line);
	ss << line;
	ss >> V >> E;

	vector<vector<int> > G(V);

	for (int i = 0; i < V; i++) {
		stringstream ss;
		int n;
		getline(infile, line);
		ss << line;

		while (ss >> n) {
			n--;
			G[i].push_back(n);
		}
	}

	infile.close();

	/*for (int i = 0; i < G.size(); i++)
	{
		vector<int> tranline = GetTransitiveClosureLineArbitary(i, G);
		TransitiveClosureLine_To_Disk(tranline, i, "Real_Data/Patents/tran_sample.txt", 1000);
	}*/
	ArbitaryGraphToDisk(G, "Real_Data/Patents/sample/samplegraph.txt");
}

void GenerateGridIndex()
{
	vector<string> filename;
	//filename.push_back("Patents");
	//filename.push_back("citeseerx");
	//filename.push_back("go_uniprot");
	filename.push_back("uniprotenc_22m");
	//filename.push_back("uniprotenc_100m");
	//filename.push_back("uniprotenc_150m");

	for (int fileindex = 0; fileindex < filename.size(); fileindex++)
	{
		string datasource = filename[fileindex];

		ReadArbitaryGraphFromDisk(m_graph_v, m_node_count, "Real_Data/" + datasource + "/graph.txt");

		//get in-edge graph
		vector<vector<int>> in_edge_graph;
		in_edge_graph.resize(m_node_count);
		for (int i = 0; i < m_node_count; i++)
		{
			for (int j = 0; j < m_graph_v[i].size(); j++)
			{
				int neighbor = m_graph_v[i][j];
				in_edge_graph[neighbor].push_back(i);
			}
		}

		Location l1;
		l1.x = 0;
		l1.y = 0;
		Location l2;
		l2.x = 1000;
		l2.y = 1000;
		int pieces = 128;
		int grid_count = pieces*pieces;
		//vector<set<int>> index1;
		vector<vector<bool>> index1;
		index1.resize(m_node_count);
		//vector<vector<bool>> update;
		//vector<set<int>> update;
		//vector<vector<int>> update;
		//update.resize(m_node_count);
		for (int i = 0; i < m_node_count; i++)
		{
			//update[i].resize(grid_count);
			index1[i].resize(grid_count);
		}

		//for (int ratio = 20; ratio <= 80; ratio += 20)
		int ratio = 80;
		{
			for (int i = 0; i < m_node_count; i++)
			{
				for (int j = 0; j < grid_count; j++)
				{
					index1[i][j] = false;
					//update[i][j] = false;
				}
			}
			ReadEntityInSCCFromDisk(m_node_count, m_entity, m_range, "Real_Data/" + datasource + "/Random_spatial_distributed/" + getstring(ratio) + "/entity.txt");
			int start = clock();
			//GenerateGridPointIndex(l1, l2, pieces, pieces, m_graph_v, in_edge_graph, m_entity, index1, update);
			GenerateGridPointIndex(l1, l2, pieces, pieces, m_graph_v, m_entity, index1);
			//GenerateGridPointIndexNotInQueue(l1, l2, pieces, pieces, m_graph_v, m_entity, index1);
			int time = clock() - start;
			ofstream ofile("data/Real_Data/" + datasource + "/GeoReachGrid_" + getstring(pieces) + "/construction_time.txt", ios::app);
			ofile << ratio << "\tWithout Update" << '\t' << time << endl;
			//ofile << ratio << "\tWith Update vector bool" << '\t' << time << endl;
			//ofile << ratio << "\tWith Update vector" << '\t' << time << endl;
			ofile.close();

			INT64 count = 0;
			//for (int i = 0; i < index1.size(); i++)
			//count += index1[i].size();
			for (int i = 0; i < index1.size(); i++)
			{
				for (int j = 0; j < index1[i].size(); j++)
				{
					if (index1[i][j])
					count += 1;
				}
			}
			for (int i = 0; i < index1.size(); i++)
			{
				//count += index1[i].size();
			}
			ofile.open("data/Real_Data/" + datasource + "/GeoReachGrid_" + getstring(pieces) + "/index_size.txt", ios::app);
			ofile << ratio << "\t" << count * 4 << endl;
			ofile.close();
			//ArbitaryGraphToDisk(index1, "Real_Data/" + datasource + "/GeoReachGrid_5/GeoReachGrid_" + getstring(ratio) + ".txt");
			GridPointIndexToDisk(index1, "Real_Data/" + datasource + "/GeoReachGrid_" + getstring(pieces) + "/GeoReachGrid_" + getstring(ratio) + ".txt");

		}
		vector<vector<int>> my_graph;
		m_graph_v.swap(my_graph);
	}



	/*{
	vector<set<int>> index2;
	GenerateGridPointIndexNotInQueue(l1, l2, 5, 5, m_graph_v, m_entity, index2);
	ArbitaryGraphToDisk(index2, "Real_Data/Patents/index_40_noqueue.txt");

	}*/

	/*for (int i = 0; i < index1.size(); i++)
	{
	if (index1[i].size() != index2[i].size())
	{
	CString str;
	str.Format(TEXT("%d size error"), i);
	MessageBox(str);
	}
	set<int>::iterator end = index2[i].end();
	for (set<int>::iterator iter = index2[i].begin(); iter != end; iter++)
	{
	if (index1[i].find(*iter) == index1[i].end())
	{
	CString str;
	str.Format(TEXT("%d not contain"), i);
	MessageBox(str);
	}
	}

	}*/
}

void SortTest()
{
	string datasource = "citeseer";

	ReadArbitaryGraphFromDisk(m_graph_v, m_node_count, "Real_Data/" + datasource + "/graph.txt");
	/*
	vector<vector<int>> graph;
	graph.resize(6);
	graph[4].push_back(5);
	graph[0].push_back(2);
	graph[5].push_back(0);
	graph[4].push_back(0);
	graph[4].push_back(1);
	graph[2].push_back(3);
	graph[3].push_back(1);
	*/
	queue<int> queue;

	TopologicalSort(m_graph_v, queue);

	int k = queue.front();
}

void GenerateGridSequence()
{
	vector<string> filename;
	//filename.push_back("Patents");
	//filename.push_back("citeseerx");
	//filename.push_back("go_uniprot");
	filename.push_back("uniprotenc_22m");
	//filename.push_back("uniprotenc_100m");
	//filename.push_back("uniprotenc_150m");

	for (int fileindex = 0; fileindex < filename.size(); fileindex++)
	{
		string datasource = filename[fileindex];

		ReadArbitaryGraphFromDisk(m_graph_v, m_node_count, "Real_Data/" + datasource + "/graph.txt");

		//get in-edge graph
		vector<vector<int>> in_edge_graph;
		in_edge_graph.resize(m_node_count);
		for (int i = 0; i < m_node_count; i++)
		{
			for (int j = 0; j < m_graph_v[i].size(); j++)
			{
				int neighbor = m_graph_v[i][j];
				in_edge_graph[neighbor].push_back(i);
			}
		}

		Location l1;
		l1.x = 0;
		l1.y = 0;
		Location l2;
		l2.x = 1000;
		l2.y = 1000;
		int pieces = 128;
		int grid_count = pieces*pieces;
		//vector<set<int>> index1;
		vector<vector<bool>> index1;
		index1.resize(m_node_count);
		//vector<vector<bool>> update;
		//vector<set<int>> update;
		//vector<vector<int>> update;
		//update.resize(m_node_count);
		for (int i = 0; i < m_node_count; i++)
		{
			//update[i].resize(grid_count);
			index1[i].resize(grid_count);
		}

		//for (int ratio = 20; ratio <= 80; ratio += 20)
		int ratio = 40;
		{
			for (int i = 0; i < m_node_count; i++)
			{
				for (int j = 0; j < grid_count; j++)
				{
					index1[i][j] = false;
					//update[i][j] = false;
				}
			}
			queue<int> queue;
			ReadEntityInSCCFromDisk(m_node_count, m_entity, m_range, "Real_Data/" + datasource + "/Random_spatial_distributed/" + getstring(ratio) + "/entity.txt");
			int start = clock();
			TopologicalSort(m_graph_v, queue);
			GenerateGridIndexSequence(l1, l2, pieces, pieces, in_edge_graph, m_entity, index1, queue);
			int time = clock() - start;
			ofstream ofile("data/Real_Data/" + datasource + "/GeoReachGrid_" + getstring(pieces) + "/construction_time.txt", ios::app);
			ofile << ratio << "\tWith sort" << '\t' << time << endl;
			//ofile << ratio << "\tWith Update vector bool" << '\t' << time << endl;
			//ofile << ratio << "\tWith Update vector" << '\t' << time << endl;
			ofile.close();

			INT64 count = 0;
			//for (int i = 0; i < index1.size(); i++)
			//count += index1[i].size();
			for (int i = 0; i < index1.size(); i++)
			{
				for (int j = 0; j < index1[i].size(); j++)
				{
					if (index1[i][j])
					count += 1;
				}
			}
			for (int i = 0; i < index1.size(); i++)
			{
				//count += index1[i].size();
			}
			ofile.open("data/Real_Data/" + datasource + "/GeoReachGrid_" + getstring(pieces) + "/index_size.txt", ios::app);
			ofile << ratio << "\t" << count * 4 << endl;
			ofile.close();
			//ArbitaryGraphToDisk(index1, "Real_Data/" + datasource + "/GeoReachGrid_5/GeoReachGrid_" + getstring(ratio) + ".txt");
			GridPointIndexToDisk(index1, "Real_Data/" + datasource + "/GeoReachGrid_" + getstring(pieces) + "/GeoReachGrid_" + getstring(ratio) + "_sort.txt");

		}
		vector<vector<int>> my_graph;
		m_graph_v.swap(my_graph);
	}
}

boolean compare(vector<vector<bool>> &index1, vector <vector<bool>> &index2)
{
	if (index1.size() != index2.size())
		return false;
	if (index1[0].size() != index2[0].size())
		return false;
	for (int i = 0; i < index1.size(); i++)
		for (int j = 0; j < index1[i].size(); j++)
			if (index1[i][j] != index2[i][j])
				return false;
	return true;
}

void Cgraph_2015_1_24_mfcDlg::OnBnClickedButtongridGenerate()
{
	//SortTest();
	GenerateGridSequence();
	//GenerateGridIndex();
	vector<vector<bool>> index1, index2;
	ReadGridPointIndexFromDisk(128 * 128, index1, "Real_Data/uniprotenc_22m/GeoReachGrid_128/GeoReachGrid_20.txt");
	ReadGridPointIndexFromDisk(128 * 128, index2, "Real_Data/uniprotenc_22m/GeoReachGrid_128/GeoReachGrid_20_sort.txt");
	bool result = compare(index1, index2);
	if (result)
	{
		CString str;
		str.Format(TEXT("true"));
		MessageBox(str);
	}
	else
	{
		CString str;
		str.Format(TEXT("false"));
		MessageBox(str);
	}
}
