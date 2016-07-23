#ifndef __WINDOW_INFO_INCLUDED__
#define __WINDOW_INFO_INCLUDED__

#include <string>

namespace Core
{
	struct WindowInfo
	{
		std::string name;
		int width, height;
		int position_x, position_y;
		bool is_reshapable;

		WindowInfo()
		{
			name = "OpenGL";
			width = 800; height = 600;
			position_x = 300;
			position_y = 300;
			is_reshapable = true;
		}

		WindowInfo(std::string name, int start_x, int start_y, int w, int h, bool reshapable)
			: name(name), position_x(start_x), position_y(start_y), width(w), height(h), is_reshapable(reshapable) {};

		WindowInfo(const WindowInfo& window_info)
		{
			name = window_info.name;
			position_x = window_info.position_x;
			position_y = window_info.position_y;

			width = window_info.width;
			height = window_info.height;
			is_reshapable = window_info.is_reshapable;
		}

		void operator=(const WindowInfo& window_info)
		{
			name = window_info.name;
			position_x = window_info.position_x;
			position_y = window_info.position_y;

			width = window_info.width;
			height = window_info.height;
			is_reshapable = window_info.is_reshapable;
		}

	};
}

#endif