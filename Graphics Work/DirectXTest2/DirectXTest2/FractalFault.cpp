#include "FractalFault.h"
#include <cstdlib>
#include <cmath>
//#include "GraphicsController.h"

void FractalFault(float* out, int w, int h, int n)
{
	for (int i = 0; i < w * h; ++i)
		out[i] = 0.0f;

	for(int i = 0; i < n; ++i)
	{
		float grad = (rand() - RAND_MAX / 2) / 10000.0f;
		int offset = rand() % h;
		bool greater = rand() % 2 == 0;
		float val = 1.0f / (120 + i);

		for(int y = 0; y < h; ++y)
			for(int x = 0; x < w; ++x)
			{
				if (greater)
				{
					if (y > grad * x + offset)
						out[y * w + x] += val;
					else
						out[y * w + x] -= val;
				}
				else
				{
					if (y <= grad * x + offset)
						out[y * w + x] += val;
					else
						out[y * w + x] -= val;
				}
			}
	}
}