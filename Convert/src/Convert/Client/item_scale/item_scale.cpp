#include "item_scale.hpp"

CConvertITEMSCALE::CConvertITEMSCALE(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertITEMSCALE::~CConvertITEMSCALE() = default;

bool CConvertITEMSCALE::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	CTextFileLoader textFileLoader;
	if (!textFileLoader.Load(sFileName.c_str()))
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return false;
	}

	textFileLoader.SetTop();

	CMemoryTextFileLoader& memoryTextFileLoader = textFileLoader.GetMemoryTextFileLoader();

	TTokenVector TokenVector;
	for (DWORD i = 0; i < memoryTextFileLoader.GetLineCount(); ++i)
	{
		if (!memoryTextFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;

		if (!(TokenVector.size() == 6 || TokenVector.size() == 7))
		{
			printf("[%s] <%s> - StrangeLine (line:%d)\n", typeid(*this).name(), sFileName.c_str(), i);
			continue;
		}

		try
		{
			json jSub{
				{ "vnum", std::stoull(TokenVector[0]) },
				{ "job", TokenVector[1] },
				{ "sex", TokenVector[2] },
				{ "param1", std::stoi(TokenVector[3]) },
				{ "param2", std::stoi(TokenVector[4]) },
				{ "param3", std::stoi(TokenVector[5]) },
			};

			if (TokenVector.size() == 7)
				jSub["param4"] = std::stof(TokenVector[6]);

			m_JsonData += std::move(jSub);
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s>: %s (line:%d)\n",
				typeid(*this).name(), sFileName.c_str(), ex.what(), i);
			return false;
		}
	}

	return true;
}