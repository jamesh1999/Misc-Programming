#ifndef __CONTEXT_INFO_INCLUDED__
#define __CONTEXT_INFO_INCLUDED__

namespace Core
{

	struct ContextInfo
	{
		int major_version, minor_version;
		bool core;

		ContextInfo() : major_version(3), minor_version(3), core(true) {};

		ContextInfo(int major_version, int minor_version, bool core) : major_version(major_version), minor_version(minor_version), core(core) {};

		ContextInfo(const ContextInfo& context_info)
		{
			major_version = context_info.major_version;
			minor_version = context_info.minor_version;
			core = context_info.core;
		}

		void operator=(const ContextInfo& context_info)
		{
			major_version = context_info.major_version;
			minor_version = context_info.minor_version;
			core = context_info.core;
		}
	};

}

#endif