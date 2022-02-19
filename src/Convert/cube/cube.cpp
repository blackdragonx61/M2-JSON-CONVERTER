#include "cube.hpp"

CConvertCUBE::CConvertCUBE(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertCUBE::~CConvertCUBE() = default;

#pragma warning(disable:26800)

bool CConvertCUBE::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	FILE* fp{ fopen(sFileName.c_str(), "rt") };

	if (fp == nullptr)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}
	
	char	one_line[256];
	int		value1, value2;
	const char* delim = " \t\r\n";
	char* v, * token_string;

	json jSub{};

	while (fgets(one_line, 256, fp))
	{
		value1 = value2 = 0;

		if (one_line[0] == '#')
			continue;

		token_string = strtok(one_line, delim);

		if (nullptr == token_string)
			continue;

		try
		{
			if ((v = strtok(NULL, delim)))
				value1 = std::stoi(v);

			if ((v = strtok(NULL, delim)))
				value2 = std::stoi(v);

			if (!_strcmpi(token_string, "section"))
			{
				jSub.clear();
			}
			else if (!_strcmpi(token_string, "npc"))
			{
				jSub["npc"].push_back(value1);
			}
			else if (!_strcmpi(token_string, "item"))
			{
				jSub["item"].push_back({ value1, value2 });
			}
			else if (!_strcmpi(token_string, "reward"))
			{
				jSub["reward"].push_back({ value1, value2 });
			}
			else if (!_strcmpi(token_string, "percent"))
			{
				jSub["percent"] = value1;
			}
			else if (!_strcmpi(token_string, "gold"))
			{
				jSub["gold"] = value1;
			}
			else if (!_strcmpi(token_string, "end"))
			{
				m_JsonData += std::move(jSub);
			}
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s>: %s\n", typeid(this).name(), sFileName.c_str(), ex.what());
			fclose(fp);
			return false;
		}
	}

	fclose(fp);
	return true;
}