#include "race_height.hpp"

CConvertRACEHEIGHT::CConvertRACEHEIGHT(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertRACEHEIGHT::~CConvertRACEHEIGHT() = default;

bool CConvertRACEHEIGHT::BuildJson() /*override*/
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

		if (TokenVector.size() != 2)
		{
			printf("[%s] <%s> - StrangeLine (line:%d)\n", typeid(*this).name(), sFileName.c_str(), i);
			continue;
		}

		try
		{
			m_JsonData += {
				{ "vnum", std::stoul(TokenVector[0]) },
				{ "height", std::stof(TokenVector[1]) }
			};
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