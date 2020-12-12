#include <iostream>
#include <chrono>
#include <cmath>
#include <omp.h>
#include <typeinfo>
#include <random>

int NX = 20000, NY = 20000;

double scalarProduct(double **x, double **y)
{
	double result = 0;
#pragma omp parallel for shared(NX, NY, x, y) reduction(+: result) default(none)
	for (int j = 0; j < NY; ++j)
	{
		for (int i = 0; i < NX; ++i)
		{
			result += x[j][i] * y[j][i];
		}
	}
	return result;
}

int main()
{
	double **x;
	x = new double *[NY];
	for (int i = 0; i < NY; ++i)
		x[i] = new double [NX];

	double **y;
	y = new double *[NY];
	for (int i = 0; i < NY; ++i)
		y[i] = new double [NX];

	for (int j = 0; j < NY; ++j)
	{
		for (int i = 0; i < NX; ++i)
		{
			x[j][i] = j-i;
			y[j][i] = 2*j-i;
		}
	}

	long double var;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	var = scalarProduct(x, y);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << var << std::endl;

	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
			  << "[ms]" << std::endl;


	for (int j = 0; j < NY; ++j)
		delete[] x[j];
	delete[] x;

	for (int j = 0; j < NY; ++j)
		delete[] y[j];
	delete[] y;
	return 0;
}
