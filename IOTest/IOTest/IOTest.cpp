// IOTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

#define BUFF_SIZE 65536

double time()
{
	return std::chrono::time_point_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now()).time_since_epoch().count() / 1000000000.0;
}

int main()
{
	double start = time();

	std::ios::sync_with_stdio(false);

	std::vector<short> data;

	//Read

	//int tot;
	//std::cin >> tot;
	//data.resize(tot + 1);
	data.resize(1000000001, 0);

	char* buff = new char[BUFF_SIZE];
	int cur = 0;

	int cnt;
	std::cin.read(buff, BUFF_SIZE);
	while (cnt = std::cin.gcount())
	{
		for (int i = 0; i < cnt; ++i)
		{
			if (buff[i] >= '0' && buff[i] <= '9')
				data[cur] = data[cur] * 10 + (buff[i] & 0xf);
			else if(buff[i] == '\n')
				++cur;
		}
		std::cin.read(buff, BUFF_SIZE);
	};
	data.pop_back();

	delete[] buff;

	//Sort
	//std::sort(data.begin(), data.end());

	//Write
	for (short s : data);
		//std::cout << s << "\n";
	//std::cout.flush();

	std::cout << time() - start << "s" << std::endl;

	return 0;
}

