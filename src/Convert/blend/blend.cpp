#include "blend.hpp"

CConvertBLEND::CConvertBLEND(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertBLEND::~CConvertBLEND() = default;

constexpr int MAX_BLEND_ITEM_VALUE = 5;

bool CConvertBLEND::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	FILE* fp{ fopen(sFileName.c_str(), "rt") };

	if (fp == nullptr)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}

	char	one_line[256]{};
	const char* delim = " \t\r\n";
	char* v;

	json jSub{};

	while (fgets(one_line, 256, fp))
	{
		if (one_line[0] == '#')
			continue;

		const char* token_string = strtok(one_line, delim);

		if (nullptr == token_string)
			continue;

		try
		{
			if (!_strcmpi(token_string, "section"))
			{
				jSub.clear();
			}
			else if (!_strcmpi(token_string, "item_vnum"))
			{
				v = strtok(nullptr, delim);

				if (nullptr == v)
				{
					printf("[%s] <%s>: problem at <item_vnum>\n", typeid(this).name(), sFileName.c_str());
					fclose(fp);
					return false;
				}

				jSub["item_vnum"] = std::stoul(v);
			}
			else if (!_strcmpi(token_string, "apply_type"))
			{
				v = strtok(NULL, delim);

				if (nullptr == v)
				{
					printf("[%s] <%s>: problem at <apply_type>\n", typeid(this).name(), sFileName.c_str());
					fclose(fp);
					return false;
				}

				jSub["apply_type"] = v;
			}
			else if (!_strcmpi(token_string, "apply_value"))
			{
				for (int i = 0; i < MAX_BLEND_ITEM_VALUE; ++i)
				{
					v = strtok(nullptr, delim);

					if (nullptr == v)
					{
						printf("[%s] <%s>: problem at <apply_value>\n", typeid(this).name(), sFileName.c_str());
						fclose(fp);
						return false;
					}

					jSub["apply_value"].push_back(std::stoi(v));
				}
			}
			else if (!_strcmpi(token_string, "apply_duration"))
			{
				for (int i = 0; i < MAX_BLEND_ITEM_VALUE; ++i)
				{
					v = strtok(nullptr, delim);

					if (nullptr == v)
					{
						printf("[%s] <%s>: problem at <apply_duration>\n", typeid(this).name(), sFileName.c_str());
						fclose(fp);
						return false;
					}

					jSub["apply_duration"].push_back(std::stoi(v));
				}
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