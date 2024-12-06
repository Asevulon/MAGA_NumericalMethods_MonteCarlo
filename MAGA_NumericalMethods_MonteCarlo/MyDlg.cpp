
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MAGA_NumericalMethods_MonteCarlo.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define REDRAW WM_USER + 1

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAGA_NUMERICALMETHODS_MONTECARLO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XOY, XOY);
	DDX_Control(pDX, IDC_XOZ, XOZ);
	DDX_Control(pDX, IDC_YOZ, YOZ);
	DDX_Control(pDX, IDC_PROGRESS1, Progress);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PARAMS, &MyDlg::OnBnClickedButtonParams)
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_MESSAGE(REDRAW, &MyDlg::OnRedraw)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	XOZ.SetTitle(L"XOZ");
	XOY.SetTitle(L"XOY");
	YOZ.SetTitle(L"YOZ");

	XOZ.SetPadding(10, 5, 10, 10);
	XOY.SetPadding(10, 5, 10, 10);
	YOZ.SetPadding(10, 5, 10, 10);

	Progress.SetRange(0, 100);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedButtonParams()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (pdlg.DoModal() != IDOK)return;
	init = true;
	model.SetN(pdlg.N);
	model.SetX(pdlg.x);
	model.SetEsm(pdlg.Esm);
	model.SetT(pdlg.T);
	model.SetStepLimit(pdlg.StepLimit);
}


void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!init)OnBnClickedButtonParams();
	Progress.SetPos(0);
	model.Stop();
	//model.Animated();
	model.main();
	timerid = SetTimer(123, 500, NULL);

	thread thr([&]() {
		model.Wait(); 
		KillTimer(timerid); 
		PostMessageW(REDRAW);
		});
	thr.detach();
}


void MyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//XOY.SetData(model.GetXOY());
	//XOZ.SetData(model.GetXOZ());
	//YOZ.SetData(model.GetYOZ());
	XOY.SetData(model.GetData());
	model.NotifyAnimation();
	XOY.Invalidate();
	Progress.SetPos(float(model.GetStepCounter()) / float(pdlg.StepLimit) * 100.);
	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT MyDlg::OnRedraw(WPARAM wParam, LPARAM lParam)
{
	//XOY.SetData(model.GetXOY());
	//XOZ.SetData(model.GetXOZ());
	//YOZ.SetData(model.GetYOZ());
	XOY.SetData(model.GetData());
	XOY.Invalidate();
	model.NotifyAnimation();
	Progress.SetPos(float(model.GetStepCounter()) / float(pdlg.StepLimit) * 100.);
	return 0;
}


void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	EsrDlg dlg;
	dlg.DoModal();
}
