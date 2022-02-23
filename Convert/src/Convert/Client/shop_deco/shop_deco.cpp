#include "shop_deco.hpp"

CConvertSHOPDECORATION::CConvertSHOPDECORATION(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertSHOPDECORATION::~CConvertSHOPDECORATION() = default;

bool CConvertSHOPDECORATION::BuildJson() /*override*/
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

		const size_t sizeTokenVec = TokenVector.size();
		
		try
		{
			if (sizeTokenVec == 3)
			{
				m_JsonData += {
					{ "type", std::stoi(TokenVector[0]) },
					{ "name", TokenVector[1] },
					{ "vnum", std::stoull(TokenVector[2]) }
				};
			}
			else if (sizeTokenVec == 4)
			{
				//DWORD dwColor;
				//sscanf(TokenVector[3].data(), "%x", &dwColor);
				
				m_JsonData += {
					{ "type", std::stoi(TokenVector[0]) },
					{ "name", TokenVector[1] },
					{ "path", TokenVector[2] },
					//{ "color", dwColor },
					{ "color", TokenVector[3] },
				};
			}
			else
			{
				printf("[%s] <%s> - StrangeLine (line:%d)\n", typeid(*this).name(), sFileName.c_str(), i);
				continue;
			}
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