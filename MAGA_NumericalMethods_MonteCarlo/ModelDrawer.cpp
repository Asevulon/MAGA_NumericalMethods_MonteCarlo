// ModelDrawer.cpp: файл реализации
//

#include "pch.h"
#include "ModelDrawer.h"


// ModelDrawer

IMPLEMENT_DYNAMIC(ModelDrawer, CStatic)

ModelDrawer::ModelDrawer()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
	radius.X = 0.4;
	radius.Y = 0.4;
}

ModelDrawer::~ModelDrawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(ModelDrawer, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений ModelDrawer




void ModelDrawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics ToWindow(lpDrawItemStruct->hDC);
	int width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	if (data.empty())
	{
		ToWindow.DrawImage(&bmp, 0, 0);
		return;
	}

	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / (GridLinesAmount - 1);
	float steplenX = (right - left) / (GridLinesAmount - 1);

	FontFamily FF(L"Arial");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}


	SolidBrush DataBrushA(Color(200, 100, 0));
	SolidBrush DataBrushB(Color(0, 100, 200));
	Brush* currentbrush = nullptr;
	double twidth = 0;
	double theight = 0;
	double xlen = right - left;
	double ylen = top - bot;
	for (int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < data.size(); j++)
		{
			p1 = PointF(i, j);
			p2 = PointF(i + 1, j + 1);
			matr.TransformPoints(&p1);
			matr.TransformPoints(&p2);
			twidth = p2.X - p1.X;
			theight = p1.Y - p2.Y;
			currentbrush = (data[i][j] == 1) ? &DataBrushA : &DataBrushB;
			gr.FillRectangle(currentbrush, p1.X, p2.Y, twidth, theight);
		}
	}

	ToWindow.DrawImage(&bmp, 0, 0);
}

void ModelDrawer::SetData(vector<vector<int>>& source)
{
	data = source;
	SetRange(PointF(0, 0), PointF(data.size() , data.size()));
	SetGridLinesAmount(data.size() + 2);
	Invalidate();
}


void ModelDrawer::SetRange(PointF& lb, PointF& rt)
{
	left = lb.X;
	bot = lb.Y;
	right = rt.X;
	top = rt.Y;
}

void ModelDrawer::SetPadding(double lP, double rP, double tP, double bP)
{
	lPadding = lP;
	rPadding = rP;
	tPadding = tP;
	bPadding = bP;
}
void ModelDrawer::SetMarksLen(int len)
{
	MarksLen = len;
}
void ModelDrawer::SetTitle(CString str)
{
	title = str;
}

void ModelDrawer::SetGridLinesAmount(int val)
{
	GridLinesAmount = val;
}

void ModelDrawer::SetMarksScaler(double val)
{
	MarksScaler = val;
}


double ModelDrawer::CalcStringLen(HDC hDC, CString str)
{
	double numlen = 0;
	int size = str.GetLength() + 1;
	for (int i = 0; i < size; i++)
	{
		ABCFLOAT abc;
		GetCharABCWidthsFloatW(hDC, str[i], str[i], &abc);
		numlen += abc.abcfA + abc.abcfB + abc.abcfC;
	}
	return numlen;
}