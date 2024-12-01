#include "pch.h"
#include "Model.h"

void Model::GenerateStartDistribution()
{
	unique_lock<mutex>lk(dmutex);
	srand(time(NULL) & GetCurrentThreadId());
	
	int target = (x < 0.5) ? -1 : 1;

	data = vector<vector<vector<int>>>(N, vector<vector<int>>(N, vector<int>(N, target)));

	int B = (N - 2) * (N - 2) * (N - 2) / 2;

	int counter = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	target = -target;
	while (counter < B)
	{
		i = RandId();
		j = RandId();
		k = RandId();
		if (data[i][j][k] == target)continue;
		data[i][j][k] = target;
		counter++;
	}

	GenerateBorders();
}

double Model::rand(double left, double right)
{
	return left + (right - left) * double(std::rand()) / double(RAND_MAX);
}

int Model::RandId()
{
	return rand(1, N - 1 - 1e-10);
}

void Model::GenerateBorders()
{
	for (int i = 1; i < N - 1; i++)
	{
		for (int j = 1; j < N - 1; j++)
		{
			data[i][j][0] = data[i][j][N - 2];
			data[i][j][N - 1] = data[i][j][1];
		}
	}

	for (int i = 1; i < N - 1; i++)
	{
		for (int k = 1; k < N - 1; k++)
		{
			data[i][0][k] = data[i][N - 2][k];
			data[i][N - 1][k] = data[i][1][k];
		}
	}

	for (int j = 1; j < N - 1; j++)
	{
		for (int k = 1; k < N - 1; k++)
		{
			data[0][j][k] = data[N - 2][j][k];
			data[N - 1][j][k] = data[1][j][k];
		}
	}
}

void Model::CalcStartEnergy()
{
	unique_lock<mutex>lk(dmutex);
	int n = N - 1;
	E = 0;
	for (int i = 1; i < n; i++)
	{
		for (int j = 1; j < n; j++)
		{
			for (int k = 1; k < n; k++)
			{
				E += data[i][j][k] * data[i - 1][j][k];
				E += data[i][j][k] * data[i + 1][j][k];

				E += data[i][j][k] * data[i][j - 1][k];
				E += data[i][j][k] * data[i][j + 1][k];

				E += data[i][j][k] * data[i][j][k - 1];
				E += data[i][j][k] * data[i][j][k + 1];
			}
		}
	}
	E *= Esm;
}

void Model::MonteCarloStep()
{
	unique_lock<mutex>lk(dmutex);
	StepCounter++;

	int i = RandId();
	int j = RandId();
	int k = RandId();

	vector<vector<int>> neighbours = GetNeighbours(i, j, k);
	if (neighbours.empty())
	{
		StepCounter--;
		return;
	}

	int id = rand(0, neighbours.size() - 1e-10);
	
	AvoidBorder(neighbours[id]);

	int in = neighbours[id][0];
	int jn = neighbours[id][1];
	int kn = neighbours[id][2];

	double DE = CalcDE(i, j, k, in, jn, kn);

	if (DE < 0)
	{
		Swap(i, j, k, in, jn, kn);
		E += DE;
		return;
	}

	double R = rand(0, 1);
	if (R < exp(-DE / kb / T))
	{
		Swap(i, j, k, in, jn, kn);
		E += DE;
		return;
	}
}

vector<vector<int>> Model::GetNeighbours(int i, int j, int k)
{
	vector<vector<int>> res;
	int target = data[i][j][k];
	vector<int>ids(3, 0);
	if (data[i + 1][j][k] != target)
	{
		ids[0] = i + 1;
		ids[1] = j;
		ids[2] = k;
		res.push_back(ids);
	}

	if (data[i - 1][j][k] != target)
	{
		ids[0] = i - 1;
		ids[1] = j;
		ids[2] = k;
		res.push_back(ids);
	}

	if (data[i][j + 1][k] != target)
	{
		ids[0] = i;
		ids[1] = j + 1;
		ids[2] = k;
		res.push_back(ids);
	}

	if (data[i][j - 1][k] != target)
	{
		ids[0] = i;
		ids[1] = j - 1;
		ids[2] = k;
		res.push_back(ids);
	}

	if (data[i][j][k + 1] != target)
	{
		ids[0] = i;
		ids[1] = j;
		ids[2] = k + 1;
		res.push_back(ids);
	}

	if (data[i][j][k - 1] != target)
	{
		ids[0] = i;
		ids[1] = j;
		ids[2] = k - 1;
		res.push_back(ids);
	}

	return res;
}

inline void Model::AvoidBorder(int& i, int& j, int& k)
{
	if (i == 0) i = N - 2;
	else if (i == N - 1)i = 1;

	if (j == 0) j = N - 2;
	else if (j == N - 1)j = 1;

	if (k == 0) k = N - 2;
	else if (k == N - 1)k = 1;
}

inline void Model::AvoidBorder(vector<int>& ids)
{
	if(ids[0] == 0) ids[0] = N - 2;
	else if (ids[0] == N - 1)ids[0] = 1;

	if (ids[1] == 0) ids[1] = N - 2;
	else if (ids[1] == N - 1)ids[1] = 1;

	if (ids[2] == 0) ids[2] = N - 2;
	else if (ids[2] == N - 1)ids[2] = 1;
}

inline double Model::CalcLocalEnergy(int& i, int& j, int& k)
{
	double res = 0;
	res += data[i][j][k] * data[i - 1][j][k];
	res += data[i][j][k] * data[i + 1][j][k];

	res += data[i][j][k] * data[i][j - 1][k];
	res += data[i][j][k] * data[i][j + 1][k];

	res += data[i][j][k] * data[i][j][k - 1];
	res += data[i][j][k] * data[i][j][k + 1];

	res *= Esm;
	return res;
}

inline double Model::CalcDE(int& i, int& j, int& k, int& in, int& jn, int& kn)
{
	double res = 0;

	int n[7] = { data[i][j][k],data[i + 1][j][k],data[i - 1][j][k],data[i][j + 1][k],data[i][j - 1][k],data[i][j][k + 1],data[i][j][k - 1] };
	int nn[7] = { data[in][jn][kn],data[in + 1][jn][kn],data[in - 1][jn][kn],data[in][jn + 1][kn],data[in][jn - 1][kn],data[in][jn][kn + 1],data[in][jn][kn - 1] };

	res += n[0] * n[1];
	res += n[0] * n[2];
	res += n[0] * n[3];
	res += n[0] * n[4];
	res += n[0] * n[5];
	res += n[0] * n[6];

	res += nn[0] * nn[1];
	res += nn[0] * nn[2];
	res += nn[0] * nn[3];
	res += nn[0] * nn[4];
	res += nn[0] * nn[5];
	res += nn[0] * nn[6];

	n[0] = -n[0];
	nn[0] = -nn[0];

	res -= n[0] * n[1];
	res -= n[0] * n[2];
	res -= n[0] * n[3];
	res -= n[0] * n[4];
	res -= n[0] * n[5];
	res -= n[0] * n[6];

	res -= nn[0] * nn[1];
	res -= nn[0] * nn[2];
	res -= nn[0] * nn[3];
	res -= nn[0] * nn[4];
	res -= nn[0] * nn[5];
	res -= nn[0] * nn[6];

	res *= Esm;
	return res;
}

inline void Model::Swap(int& i, int& j, int& k, int& in, int& jn, int& kn)
{
	data[i][j][k] = -data[i][j][k];
	data[in][jn][kn] = -data[in][jn][kn];

	if (i == 1)data[N - 1][j][k] = data[i][j][k];
	if (i == N - 2)data[1][j][k] = data[i][j][k];

	if (j == 1)data[i][N - 1][k] = data[i][j][k];
	if (j == N - 2)data[i][1][k] = data[i][j][k];

	if (k == 1)data[i][j][N - 1] = data[i][j][k];
	if (k == N - 2)data[i][j][1] = data[i][j][k];

	if (in == 1)data[N - 1][jn][kn] = data[in][jn][kn];
	if (in == N - 2)data[1][jn][kn] = data[in][jn][kn];

	if (jn == 1)data[in][N - 1][kn] = data[in][jn][kn];
	if (jn == N - 2)data[in][1][kn] = data[in][jn][kn];

	if (kn == 1)data[in][jn][N - 1] = data[in][jn][kn];
	if (kn == N - 2)data[in][jn][1] = data[in][jn][kn];
}

void Model::MonteCarlo()
{
	unique_lock<mutex>lk(smutex);
	unique_lock<mutex>lk1(wmutex);

	GenerateStartDistribution();
	CalcStartEnergy();
	StepCounter = 0;
	Continue = true;

	while ((StepCounter < StepLimit) && Continue)
	{
		MonteCarloStep();
	}
	CalcStartEnergy();
	Continue = false;
}

Model::Model()
{
}

Model::Model(const Model& r)
{

}

void Model::SetN(int val)
{
	N = val + 2;
	if (N % 2)N++;
}

void Model::SetX(double val)
{
	x = val;
}

void Model::SetEsm(double val)
{
	Esm = val;
}

void Model::SetT(double val)
{
	T = val * Esm / 2. / kb;
}

void Model::SetStepLimit(int val)
{
	StepLimit = val;
}

vector<vector<int>> Model::GetXOY()
{
	vector<vector<int>>res(N,vector<int>(N,1));
	int id = N / 2;

	unique_lock<mutex>lk(dmutex);

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			res[i][j] = data[i][j][id];
		}
	}
	return res;
}

vector<vector<int>> Model::GetXOZ()
{
	vector<vector<int>>res(N, vector<int>(N, 1));
	int id = N / 2;
	unique_lock<mutex>lk(dmutex);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			res[i][j] = data[i][id][j];
		}
	}
	return res;
}

vector<vector<int>> Model::GetYOZ()
{
	vector<vector<int>>res(N, vector<int>(N, 1));
	int id = N / 2;
	unique_lock<mutex>lk(dmutex);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			res[i][j] = data[id][i][j];
		}
	}
	return res;
}

void Model::main()
{
	

	thread thr([&]() {MonteCarlo(); });
	thr.detach();
}

void Model::Stop()
{
	Continue = false;
	unique_lock<mutex>lk(smutex);
}

bool Model::InProc()
{
	return Continue;
}

void Model::Wait()
{
	if (!Continue)return;
	unique_lock<mutex>lk(wmutex);
}

int Model::GetStepCounter()
{
	return StepCounter;
}

double Model::GetEsr()
{
	return E / (N - 2) / (N - 2) / (N - 2);
}

double Model::GetT()
{
	return T;
}
