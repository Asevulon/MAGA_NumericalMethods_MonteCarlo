#pragma once
#include<vector>
using namespace std;

class Model
{
private:
	int N = 0;
	double x = 0;
	vector<vector<vector<int>>>data;
protected:
	void GenerateStartDistribution();
	double rand(double left, double right);
	int RandId();
	void GenerateBorders();
public:
	void SetN(int val);
	void SetX(double val);
	vector<vector<int>>GetXOY();
	vector<vector<int>>GetXOZ();
	vector<vector<int>>GetYOZ();
	void main();
};

