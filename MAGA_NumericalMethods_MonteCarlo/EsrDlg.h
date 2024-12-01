#pragma once
#include "afxdialogex.h"
#include"ParamDlg.h"
#include"MultiExp.h"
#include"Drawer.h"
// Диалоговое окно EsrDlg

class EsrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EsrDlg)

public:
	EsrDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~EsrDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ESR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	double Tmin;
	double Tmax;
	CProgressCtrl prog;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	ParamDlg pdlg;
	int N;
protected:
	afx_msg LRESULT OnModelsDone(WPARAM wParam, LPARAM lParam);
	UINT_PTR timerid;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	MultiExp me;
	Drawer EsrDrw;
	CButton OkButton;
};
