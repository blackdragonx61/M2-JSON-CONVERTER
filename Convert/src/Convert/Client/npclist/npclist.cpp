#include "npclist.hpp"

CConvertNPCLIST::CConvertNPCLIST(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertNPCLIST::~CConvertNPCLIST() = default;

enum ENpcListCol
{
	NPCLIST_COL_VNUM,
	NPCLIST_COL_NAME,
	NPCLIST_COL_SRC_NAME,

	NPCLIST_COL_MAX,
};

bool CConvertNPCLIST::BuildJson() /*override*/
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

		while (TokenVector.size() < NPCLIST_COL_MAX)
			TokenVector.push_back("");

		try
		{
			json jSub{
				{ "vnum", std::stoull(TokenVector[NPCLIST_COL_VNUM]) },
				{ "name", TokenVector[NPCLIST_COL_NAME] },
			};

			if (TokenVector[NPCLIST_COL_SRC_NAME].empty() == false)
				jSub["src_name"] = TokenVector[NPCLIST_COL_SRC_NAME];

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