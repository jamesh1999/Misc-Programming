#include "stdafx.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "Config.h"


Config* Config::instance = nullptr;

Config::Config()
{
	instance = this;
}
//Gets a [category] from the loaded file with a given tag
std::string Config::GetCategory(const std::string& tag)
{
	bool inHeader = false;
	int readStart = -1;
	std::string headerBuff = "";
	unsigned i = 0;
	for (; i < m_configData.size(); ++i)
	{
		char c = m_configData[i];
		if (c == '[')
		{
			inHeader = true;

			//If read finished return substr
			if (readStart != -1)
				break;
		}
		else if (c == ']')
		{
			inHeader = false;

			//Check if desired category
			if (headerBuff == tag)
				readStart = i + 1;
			headerBuff = "";
		}
		else if (inHeader)
			headerBuff += c;
	}

	if(readStart != -1)
		return m_configData.substr(readStart, i - readStart);

	throw std::exception("That tag is not in the loaded config file");
}

//Splits an instanced category into its instances
std::vector<std::pair<int, std::string>> Config::SplitInstanced(const std::string& data)
{
	std::vector<std::pair<int, std::string>> instances;
	int id;
	std::string buff = "";
	for(unsigned i = 0; i < data.length(); ++i)
	{
		if (data[i] == '#')
		{
			//Store previous instance
			if (StripWhiteSpace(buff).size() > 0)
				instances.push_back(std::pair<int, std::string>(id, buff));
			buff = "";
			
			char* len;
			const char* start = data.c_str() + i + 1;
			id = std::strtol(start, &len, 10);
			i += len - start;
		}
		else
			buff += data[i];
	}

	//Store final instance
	if (buff.size() > 0)
		instances.push_back(std::pair<int, std::string>(id, std::string(buff)));

	std::sort(instances.begin(), instances.end());
	return instances;
}

//Removes whitespace characters from the beginning and end of a string
std::string Config::StripWhiteSpace(const std::string& data)
{
	int start = -1;
	int end = -1;

	for (unsigned i = 0; i < data.size(); ++i)
	{
		if (data[i] == ' ' 
			|| data[i] == '\r' 
			|| data[i] == '\n' 
			|| data[i] == '\t') continue;

		if (start == -1)
			start = i;
		
		end = i;
	}

	if (start == -1)
		return "";

	return data.substr(start, end - start + 1);
}

//Creates a vector of KeyValuePairs from a given category's contents
std::vector<Config::KeyValuePair> Config::ToKVPs(const std::string& data)
{
	std::string keyBuff;
	std::string valueBuff;
	bool inKey = true;
	std::vector<KeyValuePair> kvpList;

	for(char c : data)
		switch(c)
		{
		case '\n':
			{
				std::string key = StripWhiteSpace(keyBuff);
				std::string value = StripWhiteSpace(valueBuff);

				if (key.size() > 0 && value.size() > 0)
					kvpList.push_back(KeyValuePair(key, value));

				keyBuff = "";
				valueBuff = "";
				inKey = true;
			}
			break;

		case ':':
			inKey = false;
			break;

		default:
			if (inKey)
				keyBuff.push_back(c);
			else
				valueBuff.push_back(c);
		}

	//Add last pair
	std::string key = StripWhiteSpace(keyBuff);
	std::string value = StripWhiteSpace(valueBuff);

	if (key.size() > 0 && value.size() > 0)
		kvpList.push_back(KeyValuePair(key, value));

	return kvpList;
}

bool Config::GetBool(const std::vector<KeyValuePair>& data, std::string key)
{
	for(KeyValuePair kvp : data)
	{
		if (kvp.first == key)
			return kvp.second == "true";
	}

	throw std::exception("Key not found");
}

std::string Config::GetString(const std::vector<KeyValuePair>& data, std::string key)
{
	for (KeyValuePair kvp : data)
	{
		if (kvp.first == key)
			return kvp.second;
	}

	throw std::exception("Key not found");
}

int Config::GetInt(const std::vector<KeyValuePair>& data, std::string key)
{
	for (KeyValuePair kvp : data)
		if (kvp.first == key)
		{
			char* end;
			return strtol(kvp.second.c_str(), &end, 10);
		}

	throw std::exception("Key not found");
}

double Config::GetDouble(const std::vector<KeyValuePair>& data, std::string key)
{
	for (KeyValuePair kvp : data)
		if (kvp.first == key)
		{
			char* end;
			return strtod(kvp.second.c_str(), &end);
		}

	throw std::exception("Key not found");
}

void Config::LoadFromFile(std::string filename)
{
	//Load file contents
	std::ifstream in(filename);
	instance->m_configData.assign((std::istreambuf_iterator<char>(in)),
		(std::istreambuf_iterator<char>()));
	in.close();

	//Track types
	{
		auto tracks = SplitInstanced(instance->GetCategory("tracks"));
		ConfigData::TrackType buffer;
		for (auto track : tracks)
		{
			auto data = ToKVPs(track.second);
			std::cout << data.size() << std::endl;
			buffer.costPerLength = GetDouble(data, "cost");
			buffer.obstructionID = GetInt(data, "obstruction");
			buffer.repulsionForce = GetDouble(data, "repulsion");
			instance->tracks.push_back(buffer);
		}
	}

	//Train types
	{
		auto trains = SplitInstanced(instance->GetCategory("trains"));
		ConfigData::TrainType buffer;
		for (auto train : trains)
		{
			auto data = ToKVPs(train.second);
			buffer.accelerationForce = GetDouble(data, "acceleration");
			buffer.decellerationForce = GetDouble(data, "deceleration");
			buffer.cD = GetDouble(data, "drag");
			buffer.capacity = GetInt(data, "capacity");
			buffer.initialCost = GetDouble(data, "startcost");
			buffer.mass = GetDouble(data, "mass");
			buffer.runningCost = GetDouble(data, "runningcost");
			buffer.seatInclination = GetDouble(data, "inclination");
			buffer.variableSeatInclination = GetBool(data, "variableinclination");
			instance->trains.push_back(buffer);
		}
	}

	//Global settings
	auto global = ToKVPs(instance->GetCategory("global"));
	instance->maxGForceX = GetDouble(global, "maxgx");
	instance->maxGForceY = GetDouble(global, "maxgy");
	instance->maxGForceZ = GetDouble(global, "maxgz");
}
