﻿// ParamDlg.cpp: файл реализации
//

#include "pch.h"
#include "MAGA_NumericalMethods_MonteCarlo.h"
#include "afxdialogex.h"
#include "ParamDlg.h"


// Диалоговое окно ParamDlg

IMPLEMENT_DYNAMIC(ParamDlg, CDialogEx)

ParamDlg::ParamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAM_DIALOG, pParent)
	, N(10)
	, x(0.5)
{

}

ParamDlg::~ParamDlg()
{
}

void ParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, N);
	DDX_Text(pDX, IDC_EDIT2, x);
}


BEGIN_MESSAGE_MAP(ParamDlg, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений ParamDlg