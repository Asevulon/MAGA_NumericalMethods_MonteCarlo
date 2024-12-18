#pragma once
#include<vector>
#include<thread>
#include<mutex>
using namespace std;

class Model
{
private:
	int N = 0;
	double x = 0;
	double Esm = 0;
	double T = 0;
	double E = 0;
	int StepLimit = 0;
	vector<vector<vector<int>>>data;

	double Esr = 0;
	int EsrStart = 0;
	int MKSH = 0;
	int NMKSH = 0;
	int StepCounter = 0;
	const double kb = 1.380649e-23;

	bool Continue = false;

	mutex smutex;
	mutex dmutex;
	mutex wmutex;
	mutex emutex;

	bool MKSHDone = false;

	vector<double>Edata;
protected:
	void GenerateStartDistribution();
	inline double rand(double left, double right);
	// id -> [1, N]
	inline int RandId();
	void GenerateBorders();
	void CalcStartEnergy();
	void MonteCarloStep();
	inline vector<vector<int>> GetNeighbours(int i, int j, int k);
	inline void AvoidBorder(int& i, int& j, int& k);
	inline void AvoidBorder(vector<int>&ids);
	inline double CalcLocalEnergy(int& i, int& j, int& k);
	inline double CalcDE(int& i, int& j, int& k, int& in, int& jn, int& kn);
	inline void Swap(int& i, int& j, int& k, int& in, int& jn, int& kn);
	void MonteCarlo();
public:
	Model();
	Model(const Model& r);
	void SetN(int val);
	void SetX(double val);
	void SetEsm(double val);
	void SetT(double val);
	void SetStepLimit(int val);
	void SetEsrStart(int val);
	vector<vector<int>>GetXOY();
	vector<vector<int>>GetXOZ();
	vector<vector<int>>GetYOZ();
	void main();
	void Stop();
	bool InProc();
	void Wait();
	int GetStepCounter();
	double GetEsr();
	double GetT();
	vector<double>GetVectorE();
};

