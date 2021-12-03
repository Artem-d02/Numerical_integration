// Numerical_integration.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <array>
#include <functional>
#include <cmath>
#include <exception>
#include <fstream>

const size_t NUMBER_OF_POINT = 9;

//-------------------------------- point representation -----------------------------

struct Point final
{
	double x;
	double y;
};

std::array<Point, NUMBER_OF_POINT> initialize_table() noexcept;

template <size_t S>
double trapezoid_method(const std::array<Point, S>&, const size_t = 1);

template <size_t S>
double richardson_extrapolation(std::function<double(const std::array<Point, S>&, const size_t)>&, const std::array<Point, S>&);

template <size_t S>
double simpson_method(const std::array<Point, S>&);

template <size_t S>
bool check_uniformity(const std::array<Point, S>&);

//-------------------------------- main ---------------------------------------------

int main()
{
	std::fstream fout;
	fout.open("Output.txt", std::fstream::out);

	auto table = initialize_table();		//	initial data
	auto integral_mt = trapezoid_method<table.size()>(table);		//	trapezoid method
	std::cout << "Integral by the trapezoid method: " << integral_mt << std::endl;
	fout << "Integral by the trapezoid method: " << integral_mt << std::endl;
	
	std::function<double(const std::array<Point, table.size()>&, const size_t)> wrapper = std::bind(trapezoid_method<table.size()>, std::placeholders::_1, std::placeholders::_2);
	auto integral_mr = richardson_extrapolation<table.size()>(wrapper, table);		//	Richardson extrapolation
	std::cout << "Richardson extrapolation: " << integral_mr << std::endl;
	fout << "Richardson extrapolation: " << integral_mr << std::endl;

	auto integral_ms = simpson_method<table.size()>(table);		// Simpson's method
	std::cout << "Integral by the Simpson's method: " << integral_ms << std::endl;
	fout << "Integral by the Simpson's method: " << integral_ms << std::endl;

	fout.close();
}

//-------------------------------- initialization of the function from the table ----

std::array<Point, NUMBER_OF_POINT> initialize_table() noexcept
{
	std::array<Point, NUMBER_OF_POINT> table;
	double start_x = 0.0;
	for (auto& point : table)
	{
		point.x = start_x;
		start_x += 0.25;
	}
	table[0].y = 1;
	table[1].y = 0.979915;
	table[2].y = 0.927295;
	table[3].y = 0.858001;
	table[4].y = 0.785398;
	table[5].y = 0.716844;
	table[6].y = 0.716844;
	table[7].y = 0.716844;
	table[8].y = 0.716844;
	return table;
}

//-------------------------------- trapezoid method ---------------------------------

template <size_t S>
double trapezoid_method(const std::array<Point, S>& table, const size_t step)
{
	Point pre_point = table[0];
	double integral = 0.0;
	for (int i = 0; i < table.size(); i += step)
	{
		integral += (table[i].x - pre_point.x) * (table[i].y + pre_point.y) / 2;
		pre_point = table[i];
	}
	return integral;
}

//-------------------------------- Richardson's extrapolation -----------------------

template <size_t S>
double richardson_extrapolation(std::function<double(const std::array<Point, S>&, const size_t)>& integrate, const std::array<Point, S>& table)
{
	auto integral_init_approx = integrate(table, 1);
	return integral_init_approx + (integral_init_approx - integrate(table, 2)) / (2 * 2 - 1);
}

//-------------------------------- Simpson's method ---------------------------------

template <size_t S>
double simpson_method(const std::array<Point, S>& table)
{
	if (!check_uniformity(table))
		throw std::exception("Error: table is not uniformity, Simpson's method not be able to use");
	auto h = table[1].x - table[0].x;
	auto sum = table.front().y + table.back().y;
	for (int i = 1; i < table.size() - 1; i++)
	{
		if (i % 2 == 0)
			sum += 2 * table[i].y;
		else
			sum += 4 * table[i].y;
	}
	return h * sum / 3;
}

//-------------------------------- Checking table for uniformity (for Simpson's) ----

template <size_t S>
bool check_uniformity(const std::array<Point, S>& table)
{
	auto pre = table[0];
	auto h = table[1].x - table[0].x;
	for (const auto& elem : table)
	{
		if (elem.x - pre.x - h > 1e-10)
			return false;
		pre = elem;
	}
	return true;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
