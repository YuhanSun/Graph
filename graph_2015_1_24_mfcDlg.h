
// graph_2015_1_24_mfcDlg.h : header file
//

#pragma once
#include "afxwin.h"


// Cgraph_2015_1_24_mfcDlg dialog
class Cgraph_2015_1_24_mfcDlg : public CDialogEx
{
// Construction
public:
	Cgraph_2015_1_24_mfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GRAPH_2015_1_24_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedtest();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedHoptest();
	afx_msg void OnBnClickedSizetest();
	afx_msg void OnBnClickedButtongenerategraph();
	afx_msg void OnBnClickedButtonratiotest();
	afx_msg void OnBnClickedButton9();
//	afx_msg void OnBnClickedButtoncomparison();
	int m_starttype;
	CString m_entity_name;
	CComboBox m_combo;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonspatialfirst();
	afx_msg void OnBnClickedButtonspatialfirsttest();
	afx_msg void OnBnClickedButtonsocialfirsttest();
	afx_msg void OnEnChangeEditstartspatialrangeratio();
	double m_step;
	double m_startspatialrange;
	afx_msg void OnEnChangeEditspatialrangeratiostep();
	double m_endspatialrangeratio;
	afx_msg void OnEnChangeEditendspatialrangeratio();
	afx_msg void OnBnClickedButtoninitialize();
	int m_duplicate_times;
	afx_msg void OnEnChangeEditDuplicatetimes();
	afx_msg void OnBnClickedButtoncomparison();
	afx_msg void OnBnClickedButtonRmbr();
	int m_start_vertex;
	afx_msg void OnEnChangeEditstartvertex();
	afx_msg void OnBnClickedRmbrInitizlize();
	afx_msg void OnBnClickedReachquery();
//	CString m_status;
//	int m_status;
	CEdit m_status;
//	int m_status_value;
	int m_status_value;
	afx_msg void OnBnClickedButtonGenerationTest();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonDagGenerate();
	afx_msg void OnBnClickedButtonEntity();
	afx_msg void OnBnClickedButtonRandomgraphGenerate();
	afx_msg void OnBnClickedButtonRealdataGenerate();
	afx_msg void OnBnClickedButtonGenEntity();
	afx_msg void OnBnClickedButtonSpatialTran();
	afx_msg void OnBnClickedButtonTransform();
	afx_msg void OnBnClickedButtonGraphTransform();
	afx_msg void OnBnClickedButtonReserseTransform();
	afx_msg void OnBnClickedButtongridGenerate();
	afx_msg void OnBnClickedButtonGeoreachGraphconvert();
};
