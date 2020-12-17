#include <iostream>
#include <chrono>
#include <cmath>
#include <omp.h>
#include <typeinfo>
#include <random>

int NX = 30000, NY = 20000;

double scalarProduct(double **x, double **y)
{
	double result = 0;
//#pragma omp parallel for shared(NX, NY, x, y) reduction(+: result) default(none)
	for (int j = 0; j < NY; ++j)
	{
		for (int i = 0; i < NX; ++i)
		{
			result += x[j][i] * y[j][i];
		}
	}
	return result;
}

void applyOperatorNoB(double **result, double **x, bool **mask)
{
#pragma omp parallel for shared(x, result, mask)
	for (int j = 1; j < NY - 1; ++j)
	{
		for (int i = 1; i < NX - 1; ++i)
		{
			result[j][i] = 0;

			if (!mask[j][i])
			{
				for (int k = -1; k <= 1; ++k)
					for (int l = -1; l <= 1; ++l)
					{
						if (abs(k) xor abs(l))
						{
							int newJ = j + k, newI = i + l;

							if (newJ < 0)
							{
								result[j][i] += (0 - x[j][i]) * 2;
							}
							else if (newI >= NX - 1 || newI <= 0 || newJ >= NY - 1 || newJ <= 0)
							{
								result[j][i] += 0;
							}
							else if (mask[newJ][newI])
							{
								result[j][i] += (0 - x[j][i]) * 2;
							}
							else
							{
								result[j][i] += x[newJ][newI] - x[j][i];
							}
						}

					}
			}
		}
	}
}

int main()
{
	double **x;
	x = new double *[NY];
	for (int i = 0; i < NY; ++i)
		x[i] = new double[NX];

	double **result;
	result = new double *[NY];
	for (int i = 0; i < NY; ++i)
		result[i] = new double[NX];

	bool **mask;
	mask = new bool *[NY];
	for (int i = 0; i < NY; ++i)
		mask[i] = new bool[NX];

	for (int j = 0; j < NY; ++j)
	{
		for (int i = 0; i < NX; ++i)
		{
			x[j][i] = j - i;
			mask[j][i] = floor(2 * sin(j + i));
		}
	}

	long double var;

	std::cout << "start" << std::endl;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (int i = 0; i < 20; ++i)
		applyOperatorNoB(result, x, mask);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << scalarProduct(result, result) << std::endl;

	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
			  << "[ms]" << std::endl;


	for (int j = 0; j < NY; ++j)
		delete[] x[j];
	delete[] x;

	for (int j = 0; j < NY; ++j)
		delete[] result[j];
	delete[] result;

	for (int j = 0; j < NY; ++j)
		delete[] mask[j];
	delete[] mask;
	return 0;
}
