﻿// EsrDlg.cpp: файл реализации
//

#include "pch.h"
#include "MAGA_NumericalMethods_MonteCarlo.h"
#include "afxdialogex.h"
#include "EsrDlg.h"
#define MODELS_DONE WM_USER + 1

// Диалоговое окно EsrDlg

IMPLEMENT_DYNAMIC(EsrDlg, CDialogEx)

EsrDlg::EsrDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ESR_DIALOG, pParent)
	, Tmin(0.6)
	, Tmax(3.5)
	, N(50)
	, NInterpolate(2000)
	, EsrStart(500)
{

}

EsrDlg::~EsrDlg()
{
}

void EsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Tmin);
	DDX_Text(pDX, IDC_EDIT2, Tmax);
	DDX_Control(pDX, IDC_PROGRESS1, prog);
	DDX_Text(pDX, IDC_EDIT3, N);
	DDX_Control(pDX, IDC_Esr, EsrDrw);
	DDX_Control(pDX, IDOK, OkButton);
	DDX_Text(pDX, IDC_EDIT4, NInterpolate);
	DDX_Control(pDX, IDC_Esr2, DerDrw);
	DDX_Text(pDX, IDC_EDIT6, EsrStart);
}


BEGIN_MESSAGE_MAP(EsrDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &EsrDlg::OnBnClickedOk)
	ON_MESSAGE(MODELS_DONE, &EsrDlg::OnModelsDone)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений EsrDlg


void EsrDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	if (pdlg.DoModal() != IDOK)return;
	me.Stop();

	me.SetN(pdlg.N);
	me.SetX(pdlg.x);
	me.SetEsm(pdlg.Esm);
	me.SetStepLimit(pdlg.StepLimit);
	me.SetNStep(N);
	me.SetTmin(Tmin);
	me.SetTmax(Tmax);
	me.SetInetrpolateN(NInterpolate);
	me.SetEsrStart(EsrStart);
	prog.SetPos(0);

	me.main();

	timerid = SetTimer(1234, 50, NULL);
	thread thr([&]() {
		me.Wait();
		PostMessageW(MODELS_DONE);
		KillTimer(timerid);
		});
	thr.detach();
}


BOOL EsrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	prog.SetRange(0, 100);
	prog.SetPos(0);

	EsrDrw.SetPadding(22, 5, 10, 10);
	EsrDrw.SetTitle(L"<E>");

	DerDrw.SetPadding(22, 5, 10, 10);
	DerDrw.SetTitle(L"Производная <E>");
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


afx_msg LRESULT EsrDlg::OnModelsDone(WPARAM wParam, LPARAM lParam)
{
	EsrDrw.SetData(me.GetEsrInterpolated());
	EsrDrw.SetKeys(me.GetTInterpolated());
	EsrDrw.Invalidate();
	DerDrw.SetData(me.GetEsrDerivative());
	DerDrw.SetKeys(me.GetTInterpolated());
	DerDrw.Invalidate();
	prog.SetPos(me.GetProgress());
	MessageBoxW(L"Моделирование завершено");
	return 0;
}


void EsrDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	
	prog.SetPos(me.GetProgress());
	CDialogEx::OnTimer(nIDEvent);
}
