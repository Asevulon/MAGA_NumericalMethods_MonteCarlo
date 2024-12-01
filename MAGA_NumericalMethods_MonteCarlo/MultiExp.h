#pragma once
#include"Model.h"
class MultiExp
{
private:
	int NStep = 0;
	double Tmin = 0;
	double Tmax = 0;

	int N = 0;
	double x = 0;
	double Esm = 0;
	int StepLimit = 0;
	vector<Model>models;
protected:

public:
	void SetN(int val);
	void SetX(double val);
	void SetEsm(double val);
	void SetStepLimit(int val);
	void SetTmin(double val);
	void SetTmax(double val);
	void SetNStep(int val);

	void main();
	void Wait();
	double GetProgress();
	vector<double>GetEsr();
	vector<double>GetT();
	void Stop();
};

