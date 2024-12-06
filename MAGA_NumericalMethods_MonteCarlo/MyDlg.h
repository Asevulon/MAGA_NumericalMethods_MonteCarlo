
// MyDlg.h: файл заголовка
//

#pragma once
#include"ParamDlg.h"
#include"ModelDrawer.h"
#include"Model.h"
#include"Model2D.h"
#include"EsrDlg.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAGA_NUMERICALMETHODS_MONTECARLO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonParams();
	ParamDlg pdlg;
	bool init = false;
	ModelDrawer XOY;
	ModelDrawer XOZ;
	ModelDrawer YOZ;
	afx_msg void OnBnClickedOk();
	Model2D model;
	UINT_PTR timerid;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl Progress;
protected:
	afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
};
