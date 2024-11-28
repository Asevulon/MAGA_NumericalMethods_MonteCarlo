#pragma once


// ModelDrawer
#include<gdiplus.h>
#include<vector>
#include<algorithm>
using namespace Gdiplus;
using namespace std;

class ModelDrawer : public CStatic
{
	DECLARE_DYNAMIC(ModelDrawer)
private:
	PointF radius;
	vector<vector<int>>data;
	double lPadding;
	double rPadding;
	double tPadding;
	double bPadding;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;


	ULONG_PTR token;
	
	CString title;

	int GridLinesAmount = 11;
	double MarksScaler = 1;
public:
	ModelDrawer();
	virtual ~ModelDrawer();

protected:
	double CalcStringLen(HDC hDC, CString string);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetData(vector<vector<int>>&source);
	void SetRange(PointF& lb, PointF& rt);
	//Отступ задается в процентах (диапазон значений от 0 до 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetMarksLen(int len);
	void SetTitle(CString str);
	void SetGridLinesAmount(int val);
	void SetMarksScaler(double val);
};


