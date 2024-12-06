#pragma once
#include<vector>
#include<thread>
#include<mutex>
using namespace std;

class Model2D
{
private:
	int N = 0;
	double x = 0;
	double Esm = 0;
	double T = 0;
	double E = 0;
	int StepLimit = 0;
	vector<vector<int>>data;

	int MKSH = 0;
	int NMKSH = 0;
	int StepCounter = 0;
	const double kb = 1.380649e-23;

	bool Continue = false;

	mutex smutex;
	mutex dmutex;
	mutex wmutex;
	mutex amutex;
	condition_variable acv;
	bool MKSHDone = false;
protected:
	void GenerateStartDistribution();
	inline double rand(double left, double right);
	// id -> [1, N]
	inline int RandId();
	void GenerateBorders();
	void CalcStartEnergy();
	void MonteCarloStep();
	inline vector<vector<int>> GetNeighbours(int i, int j);
	inline void AvoidBorder(int& i, int& j, int& k);
	inline void AvoidBorder(vector<int>&ids);
	inline double CalcDE(int& i, int& j, int& in, int& jn);
	inline void Swap(int& i, int& j, int& in, int& jn);
	void MonteCarlo();
	void MonteCarloAnimated();
	void WaitAnimation();
	void Test();
public:
	Model2D();
	Model2D(const Model2D& r);
	void SetN(int val);
	void SetX(double val);
	void SetEsm(double val);
	void SetT(double val);
	void SetStepLimit(int val);
	vector<vector<int>>GetData();
	void main();
	void Animated();
	void Stop();
	bool InProc();
	void Wait();
	int GetStepCounter();
	double GetEsr();
	double GetT();
	void NotifyAnimation();
};

