#ifndef __CONFIG_INCLUDED__
#define __CONFIG_INCLUDED__

#include <string>
#include <vector>
#include <utility>
#include <iostream>

#include "TrackType.h"
#include "TrainType.h"

class Config
{
private:
	std::string m_configData;

	typedef std::pair<std::string, std::string> KeyValuePair;

	//Helper functions for parsing the config
	std::string GetCategory(const std::string&);
	static std::vector<std::pair<int, std::string>> SplitInstanced(const std::string&);
	static std::string StripWhiteSpace(const std::string&);
	static std::vector<KeyValuePair> ToKVPs(const std::string&);
	static bool GetBool(const std::vector<KeyValuePair>&, std::string);
	static std::string GetString(const std::vector<KeyValuePair>&, std::string);
	static int GetInt(const std::vector<KeyValuePair>&, std::string);
	static double GetDouble(const std::vector<KeyValuePair>&, std::string);

public:
	static Config* instance;

	//Configuration data
	//Instanced data
	std::vector<ConfigData::TrackType> tracks;
	std::vector<ConfigData::TrainType> trains;

	//Global settings
	double maxGForceX;
	double maxGForceY;
	double maxGForceZ;
	
	Config();

	static void LoadFromFile(std::string);

	//Delete move/copy constructors for singletons
	Config(const Config&) = delete;
	Config(const Config&&) = delete;
	Config& operator=(const Config&) = delete;
	Config& operator=(const Config&&) = delete;
};

#endif