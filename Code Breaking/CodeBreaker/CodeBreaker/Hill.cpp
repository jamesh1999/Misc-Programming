#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Hill.h"
#include "Globals.h"
#include "Eigen\Dense"

using namespace Eigen;

const unsigned size = 3;

std::string decryptHill(const std::vector<unsigned>& input, unsigned n, Matrix<int, size, size>* mat)
{
	std::string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	std::string text;
	text.resize(input.size(), 'A');

	unsigned x = input.size() / n;
	for (unsigned i = 0; i < x; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int acc = 0;
			for (int k = 0; k < n; ++k)
			{
				acc += input[i * n + k] * (*mat)(j, k);
			}
			text[i * n + j] = alphabet[acc % 25];
		}
	}

	return text;
}

unsigned coeff(int det)
{
	int x = 1;
	det %= 25;
	if (det < 0)
		det += 25;
	for (; (x * det) % 25 != 1 && x < 25; ++x);
	return x;
}

void hill(std::string text)
{
	std::vector<unsigned> vals;
	vals.resize(text.size());
	std::string crib = "";
	std::string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

	for (unsigned i = 0; i < text.size(); ++i)
		vals[i] = alphabet.find_first_of(text[i]);

	double best = -DBL_MAX;

	int max = text.size() - (text.size() % size) - size * size;
			for (int q = 0; q <= max; ++q)
			{
				int offset = -q % size;
				if (offset < 0)
					offset += size;
				Matrix<int, size, size> s1;
				Matrix<int, size, size> s2;

				for (int i = 0; i < size; ++i)
				{
					for (int j = 0; j < size; ++j)
					{
						s1(j, i) = vals[i * size + j + q + offset];
						s2(j, i) = alphabet.find_first_of(crib[i * size + j + offset]);
					}
				}

				int det = s1.determinant();
				unsigned c = coeff(det);

				if (c == 25)
					continue;

				Matrix<double, size, size> dbl = s1.cast<double>();
				Matrix<double, size, size> dbl_inv = dbl.inverse() * det * c;
				Matrix<int, size, size> s1_inv;

				for (int i = 0; i < size; ++i)
					for (int j = 0; j < size; ++j)
						s1_inv(i, j) = std::llround(dbl_inv(i, j));

				Matrix<int, size, size> key = s2 * s1_inv;

				for (int i = 0; i < size; ++i)
					for (int j = 0; j < size; ++j)
					{
						key(i, j) = key(i, j) % 25;
						if (key(i, j) < 0)
							key(i, j) += 25;
					}

				std::string res = decryptHill(vals, size, &key);
				double score = evaluate(res);

				if (score > best)
				{
					best = score;
					system("cls");
					std::cout << score << std::endl;
					std::cout << res << std::endl;
				}
			}
}