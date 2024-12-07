#pragma once
#include "afxdialogex.h"


// Диалоговое окно ParamDlg

class ParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParamDlg)

public:
	ParamDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ParamDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int N;
	double x;
	double Esm;
	double T;
	int StepLimit;
	int EsrStart;
};
