#include "pch.h"
#include "Model2D.h"

void Model2D::GenerateStartDistribution()
{
	unique_lock<mutex>lk(dmutex);
	srand(time(NULL) ^ GetCurrentThreadId());
	
	int target = (x < 0.5) ? -1 : 1;

	data = vector<vector<int>>(N, vector<int>(N, target));

	int B = (N - 2) * (N - 2) / 2;

	int counter = 0;
	int i = 0;
	int j = 0;
	target = -target;
	while (counter < B)
	{
		i = RandId();
		j = RandId();
		if (data[i][j] == target)continue;
		data[i][j] = target;
		counter++;
	}

	GenerateBorders();
}

double Model2D::rand(double left, double right)
{
	return left + (right - left) * double(std::rand()) / double(RAND_MAX);
}

int Model2D::RandId()
{
	return rand(1, N - 1 - 1e-10);
}

void Model2D::GenerateBorders()
{
	for (int i = 1; i < N - 1; i++)
	{
			data[i][0] = data[i][N - 2];
			data[i][N - 1] = data[i][1];
	}

	for (int j = 1; j < N - 1; j++)
	{
			data[0][j] = data[N - 2][j];
			data[N - 1][j] = data[1][j];
	}
}

void Model2D::CalcStartEnergy()
{
	unique_lock<mutex>lk(dmutex);
	int n = N - 1;
	E = 0;
	for (int i = 1; i < n; i++)
	{
		for (int j = 1; j < n; j++)
		{
				E += data[i][j] * data[i - 1][j];
				E += data[i][j] * data[i + 1][j];

				E += data[i][j] * data[i][j - 1];
				E += data[i][j] * data[i][j + 1];

		}
	}
	E *= -Esm;
}

void Model2D::MonteCarloStep()
{
	unique_lock<mutex>lk(dmutex);
	StepCounter++;
	if (StepCounter == NMKSH)
	{
		StepCounter = 0;
		MKSH++;
	}
	int i = RandId();
	int j = RandId();

	vector<vector<int>> neighbours = GetNeighbours(i, j);
	if (neighbours.empty())
	{
		StepCounter--;
		return;
	}

	int id = rand(0, neighbours.size() - 1e-10);
	
	AvoidBorder(neighbours[id]);

	int in = neighbours[id][0];
	int jn = neighbours[id][1];

	double DE = CalcDE(i, j, in, jn);

	if (DE < 0)
	{
		Swap(i, j, in, jn);
		E += DE;
		return;
	}

	double R = rand(0, 1);
	double cap = exp(-DE / kb / T);
	if (R < cap)
	{
		Swap(i, j, in, jn);
		E += DE;
		return;
	}
}

vector<vector<int>> Model2D::GetNeighbours(int i, int j)
{
	vector<vector<int>> res;
	int target = data[i][j];
	vector<int>ids(2, 0);
	if (data[i + 1][j] != target)
	{
		ids[0] = i + 1;
		ids[1] = j;
		res.push_back(ids);
	}

	if (data[i - 1][j] != target)
	{
		ids[0] = i - 1;
		ids[1] = j;
		res.push_back(ids);
	}

	if (data[i][j + 1] != target)
	{
		ids[0] = i;
		ids[1] = j + 1;
		res.push_back(ids);
	}

	if (data[i][j - 1] != target)
	{
		ids[0] = i;
		ids[1] = j - 1;
		res.push_back(ids);
	}

	return res;
}

inline void Model2D::AvoidBorder(int& i, int& j, int& k)
{
	if (i == 0) i = N - 2;
	else if (i == N - 1)i = 1;

	if (j == 0) j = N - 2;
	else if (j == N - 1)j = 1;

	if (k == 0) k = N - 2;
	else if (k == N - 1)k = 1;
}

inline void Model2D::AvoidBorder(vector<int>& ids)
{
	if(ids[0] == 0) ids[0] = N - 2;
	else if (ids[0] == N - 1)ids[0] = 1;

	if (ids[1] == 0) ids[1] = N - 2;
	else if (ids[1] == N - 1)ids[1] = 1;
}

inline double Model2D::CalcDE(int& i, int& j, int& in, int& jn)
{
	double res = 0;

	int n[5] = { data[i][j],data[i + 1][j],data[i - 1][j],data[i][j + 1],data[i][j - 1] };
	int nn[5] = { data[in][jn],data[in + 1][jn],data[in - 1][jn],data[in][jn + 1],data[in][jn - 1] };

	res -= n[0] * n[1];
	res -= n[0] * n[2];
	res -= n[0] * n[3];
	res -= n[0] * n[4];

	res -= nn[0] * nn[1];
	res -= nn[0] * nn[2];
	res -= nn[0] * nn[3];
	res -= nn[0] * nn[4];

	n[0] = -n[0];
	nn[0] = -nn[0];

	res += n[0] * n[1];
	res += n[0] * n[2];
	res += n[0] * n[3];
	res += n[0] * n[4];

	res += nn[0] * nn[1];
	res += nn[0] * nn[2];
	res += nn[0] * nn[3];
	res += nn[0] * nn[4];

	res *= -Esm;
	return res;
}

inline void Model2D::Swap(int& i, int& j, int& in, int& jn)
{
	data[i][j] = -data[i][j];
	data[in][jn] = -data[in][jn];

	if (i == 1)data[N - 1][j] = data[i][j];
	if (i == N - 2)data[1][j] = data[i][j];

	if (j == 1)data[i][N - 1] = data[i][j];
	if (j == N - 2)data[i][1] = data[i][j];

	if (in == 1)data[N - 1][jn] = data[in][jn];
	if (in == N - 2)data[1][jn] = data[in][jn];

	if (jn == 1)data[in][N - 1] = data[in][jn];
	if (jn == N - 2)data[in][1] = data[in][jn];
}

void Model2D::MonteCarlo()
{
	unique_lock<mutex>lk(smutex);
	unique_lock<mutex>lk1(wmutex);

	GenerateStartDistribution();
	//CalcStartEnergy();
	StepCounter = 0;
	MKSH = 0;
	NMKSH = N * N;
	Continue = true;

	while ((MKSH < StepLimit))
	{
		MonteCarloStep();
	}
	CalcStartEnergy();
	Continue = false;
}

void Model2D::MonteCarloAnimated()
{
	unique_lock<mutex>lk(smutex);
	unique_lock<mutex>lk1(wmutex);

	GenerateStartDistribution();
	//CalcStartEnergy();
	StepCounter = 0;
	MKSH = 0;
	NMKSH = N * N;
	Continue = true;

	while ((MKSH < StepLimit))
	{
		MonteCarloStep();
		WaitAnimation();
	}
	CalcStartEnergy();
	Continue = false;
}

void Model2D::WaitAnimation()
{
	unique_lock<mutex>lk(amutex);
	acv.wait(lk);
}

Model2D::Model2D()
{
}

Model2D::Model2D(const Model2D& r)
{

}

void Model2D::SetN(int val)
{
	N = val + 2;
	if (N % 2)N++;
}

void Model2D::SetX(double val)
{
	x = val;
}

void Model2D::SetEsm(double val)
{
	Esm = val;
}

void Model2D::SetT(double val)
{
	T = val * Esm / 2. / kb;
}

void Model2D::SetStepLimit(int val)
{
	StepLimit = val;
}

vector<vector<int>> Model2D::GetData()
{
	unique_lock<mutex>lk(dmutex);
	return data;
}

void Model2D::main()
{
	thread thr([&]() {MonteCarlo(); });
	thr.detach();
}

void Model2D::Animated()
{
	thread thr([&]() {MonteCarloAnimated(); });
	thr.detach();
}

void Model2D::Stop()
{
	Continue = false;
	unique_lock<mutex>lk(smutex);
}

bool Model2D::InProc()
{
	return Continue;
}

void Model2D::Wait()
{
	if (!Continue)return;
	unique_lock<mutex>lk(wmutex);
}

int Model2D::GetStepCounter()
{
	return MKSH;
}

double Model2D::GetEsr()
{
	return E / (N - 2) / (N - 2) / (N - 2);
}

double Model2D::GetT()
{
	return T;
}

void Model2D::NotifyAnimation()
{
	acv.notify_all();
}
