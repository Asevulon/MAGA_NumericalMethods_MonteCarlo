#include "pch.h"
#include "Model.h"

void Model::GenerateStartDistribution()
{
	srand(time(NULL));
	
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

void Model::SetN(int val)
{
	N = val + 2;
	if (N % 2)N++;
}

void Model::SetX(double val)
{
	x = val;
}

vector<vector<int>> Model::GetXOY()
{
	vector<vector<int>>res(N,vector<int>(N,1));
	int id = N / 2;
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
	GenerateStartDistribution();
}
