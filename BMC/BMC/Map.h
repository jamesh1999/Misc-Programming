#ifndef __MAP_INCLUDED__
#define __MAP_INCLUDED__

#include <string>
#include <vector>

namespace ConfigData
{
	extern const double MAP_RESOLUTION;

	class Map
	{
	private:
		std::vector<bool> m_rasterized;
		int m_width = 0;
		int m_height = 0;

	public:
		explicit Map(std::string);
		bool CheckOverlap(double, double);
	};

	typedef Map ObstructionMap;
	typedef Map Layout;
}

#endif