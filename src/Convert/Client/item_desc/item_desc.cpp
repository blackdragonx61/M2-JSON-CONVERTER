#include "item_desc.hpp"

CConvertITEMDESC::CConvertITEMDESC(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertITEMDESC::~CConvertITEMDESC() = default;

enum EItemDescCol
{
	ITEMDESC_COL_VNUM,
	ITEMDESC_COL_NAME,
	ITEMDESC_COL_DESC,
	ITEMDESC_COL_SUMM,
	ITEMDESC_COL_NUM,
};

bool CConvertITEMDESC::BuildJson() /*override*/
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

	TTokenVector kTokenVector;
	for (DWORD i = 0; i < memoryTextFileLoader.GetLineCount(); ++i)
	{
		if (!memoryTextFileLoader.SplitLineByTab(i, &kTokenVector))
			continue;

		while (kTokenVector.size() < ITEMDESC_COL_NUM)
			kTokenVector.push_back("");

		try
		{
			m_JsonData += {
				{ "vnum", std::stoul(kTokenVector[ITEMDESC_COL_VNUM]) },
				{ "name", kTokenVector[ITEMDESC_COL_NAME] },
				{ "description", kTokenVector[ITEMDESC_COL_DESC] },
				{ "summary", kTokenVector[ITEMDESC_COL_SUMM] },
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