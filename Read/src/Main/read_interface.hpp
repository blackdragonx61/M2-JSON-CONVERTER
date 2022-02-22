#pragma once

#include <string>
#include <fstream>
#include <vector>

// https://github.com/nlohmann/json (3.10.5)
#include <nlohmann/json.hpp>
using json = nlohmann::ordered_json;

// https://github.com/Tencent/rapidjson/ (1.1.0)
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

class ReadInterface
{
public:
	ReadInterface(const char* FileName) :
		m_FileName{ FileName }
	{
		printf("-----------------------READ BEGIN:<%s>-----------------------\n", m_FileName.c_str());
	}

	virtual ~ReadInterface() 
	{
		printf("-----------------------READ END:<%s>-----------------------\n", m_FileName.c_str());
	};
	virtual void PrintJson() = 0;

protected:
	const std::string& GetFileName() const
	{
		return m_FileName;
	}

private:
	std::string m_FileName;
};