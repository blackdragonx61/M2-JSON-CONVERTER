#include "atlasinfo.hpp"

CConvertATLASINFO::CConvertATLASINFO(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertATLASINFO::~CConvertATLASINFO() = default;

bool CConvertATLASINFO::BuildJson() /*override*/
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

	char szMapName[256];
	int x, y;
	int width, height;
	const DWORD uLineCount = memoryTextFileLoader.GetLineCount();

	for (DWORD uLineIndex = 0; uLineIndex < uLineCount; ++uLineIndex)
	{
		const std::string& c_rstLine = memoryTextFileLoader.GetLineString(uLineIndex);
		if (c_rstLine.empty())
			continue;

		sscanf(c_rstLine.c_str(), "%s %d %d %d %d",
			szMapName, &x, &y, &width, &height);

		if ('\0' == szMapName[0])
			continue;

		m_JsonData += {
			{ "name", szMapName }, 
			{ "x", x },
			{ "y", y },
			{ "width", width },
			{ "height", height }
		};
	}

	return true;
}