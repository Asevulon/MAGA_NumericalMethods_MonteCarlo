#include "pch.h"
#include "MultiExp.h"
#define pow2(x) ((x) * (x))
#define pow3(x) ((x) * (x) * (x))

void MultiExp::Interpolate()
{
	vector<double> f;
	for (auto& m : models)f.push_back(m.GetEsr());
	if (f.empty())
	{
		return;
	}

	vector<double> m;
	m.resize(f.size());
	int actsize = m.size();

	double h = (Tmax - Tmin) / (NStep - 1);

	m[0] = (4. * f[1] - f[2] - 3. * f[0]) / 2. / h;
	m[actsize - 1] = (3 * f[actsize - 1] + f[actsize - 3] - 4. * f[actsize - 2]) / 2. / h;
	for (int i = 1; i < actsize - 1; i++)
	{
		m[i] = (f[i + 1] - f[i - 1]) / 2. / h;
	}


	EsrInterpolated.resize(InterpolateN);
	EsrDerivative.resize(InterpolateN);
	TInterpolated.resize(InterpolateN);
	double ih = (Tmax - Tmin) / (InterpolateN - 1);
	double x = 0;
	double xi = 0;
	double xi1 = 0;
	int id = 0;
	double temp = 0;
	for (int i = 0; i < InterpolateN - 1; i++)
	{
		x = Tmin + i * ih;
		id = (x - Tmin) / h;
		xi = id * h + Tmin;
		xi1 = (id + 1) * h + Tmin;
		temp = pow2(xi1 - x) * (2 * (x - xi) + h) * f[id] / pow3(h);
		temp += pow2(x - xi) * (2 * (xi1 - x) + h) * f[id + 1] / pow3(h);
		temp += pow2(xi1 - x) * (x - xi) * m[id] / pow2(h);
		temp += pow2(x - xi) * (x - xi1) * m[id + 1] / pow2(h);
		EsrInterpolated[i] = temp;
		TInterpolated[i] = x / Tc;
	}
	EsrInterpolated[InterpolateN - 1] = f[actsize - 1];
	TInterpolated[InterpolateN - 1] = Tmax / Tc;

	EsrDerivative[0] = (4. * EsrInterpolated[1] - EsrInterpolated[2] - 3. * EsrInterpolated[0]) / 2. / ih;
	EsrDerivative[InterpolateN - 1] = (3. * EsrInterpolated[InterpolateN - 1] + EsrInterpolated[InterpolateN - 3] - 4. * EsrInterpolated[InterpolateN - 2]) / 2. / ih;
	for (int i = 1; i < InterpolateN - 1; i++)
	{
		EsrDerivative[i] = (EsrInterpolated[i + 1] - EsrInterpolated[i - 1]) / 2. / ih;
	}

	reinterpolate = false;
}

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

void MultiExp::SetInetrpolateN(int val)
{
	InterpolateN = val;
	reinterpolate = true;
}

void MultiExp::SetEsrStart(int val)
{
	EsrStart = val;
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
		models[i].SetEsrStart(EsrStart);
		models[i].main();
	}
	reinterpolate = true;
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

vector<double> MultiExp::GetEsrInterpolated()
{
	if (reinterpolate)Interpolate();
	return EsrInterpolated;
}

vector<double> MultiExp::GetT()
{
	vector<double>res;
	double step = (Tmax - Tmin) / (NStep - 1);
	for (int i = 0; i < NStep; i++)
	{
		res.push_back((i * step + Tmin) / Tc);
	}
	return res;
}

vector<double> MultiExp::GetTInterpolated()
{
	return TInterpolated;
}

vector<double> MultiExp::GetEsrDerivative()
{
	return EsrDerivative;
}

void MultiExp::Stop()
{
	for (auto& m : models)m.Stop();
}
