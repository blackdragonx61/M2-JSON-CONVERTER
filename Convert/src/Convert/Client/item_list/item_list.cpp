#include "item_list.hpp"

CConvertITEMLIST::CConvertITEMLIST(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertITEMLIST::~CConvertITEMLIST() = default;

bool CConvertITEMLIST::BuildJson() /*override*/
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

		if (!(TokenVector.size() == 3 || TokenVector.size() == 4))
		{
			printf("[%s] <%s> - StrangeLine (line:%d)\n", typeid(*this).name(), sFileName.c_str(), i);
			continue;
		}

		try
		{
			json jSub{
				{ "vnum", std::stoul(TokenVector[0]) },
				{ "type", TokenVector[1] },
				{ "icon", TokenVector[2] }
			};

			if (TokenVector.size() == 4)
				jSub["model"] = TokenVector[3];

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