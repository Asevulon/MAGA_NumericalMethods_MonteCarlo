#include "pch.h"
#include "MultiExp.h"

void MultiExp::SetN(int val)
{
	N = val;
}

void MultiExp::SetX(double val)
{
	x = val;
}

void MultiExp::SetEsm(double val)
{
	Esm = val;
}

void MultiExp::SetStepLimit(int val)
{
	StepLimit = val;
}

void MultiExp::SetTmin(double val)
{
	Tmin = val;
}

void MultiExp::SetTmax(double val)
{
	Tmax = val;
}

void MultiExp::SetNStep(int val)
{
	NStep = val;
}

void MultiExp::main()
{
	models = vector<Model>(NStep, Model());
	double step = (Tmax - Tmin) / (NStep - 1);
	for (int i = 0; i < NStep; i++)
	{
		models[i].SetN(N);
		models[i].SetX(x);
		models[i].SetEsm(Esm);
		models[i].SetT(i * step + Tmin);
		models[i].SetStepLimit(StepLimit);
		models[i].main();
	}
}

void MultiExp::Wait()
{
	for (auto& m : models)m.Wait();
}

double MultiExp::GetProgress()
{
	float summ = 0;
	for (auto& m : models)summ += m.GetStepCounter();
	summ /= NStep * StepLimit;
	summ *= 100;
	return summ;
}

vector<double> MultiExp::GetEsr()
{
	vector<double>res;
	for (auto& m : models)res.push_back(m.GetEsr());
	return res;
}

vector<double> MultiExp::GetT()
{
	vector<double>res;
	double step = (Tmax - Tmin) / (NStep - 1);
	for (int i = 0; i < NStep; i++)
	{
		res.push_back(i * step + Tmin);
	}
	return res;
}

void MultiExp::Stop()
{
	for (auto& m : models)m.Stop();
}
