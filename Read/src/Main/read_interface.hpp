#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <format>

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
		printf("-----------------------READ END:<%s>-----------------------\n\n", m_FileName.c_str());
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