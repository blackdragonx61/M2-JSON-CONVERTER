#include "group.hpp"

CConvertGROUP::CConvertGROUP(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertGROUP::~CConvertGROUP() = default;

bool CConvertGROUP::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	CTextFileLoader loader;
	if (!loader.Load(sFileName.c_str()))
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}

	std::string stName;

	for (DWORD i = 0; i < loader.GetChildNodeCount(); ++i)
	{
		json jSub;
		
		loader.SetChildNode(i);
		loader.GetCurrentNodeName(&stName);

		try
		{
			// jSub["group_name"] = stName;

			int iVnum;

			if (!loader.GetTokenInteger("vnum", &iVnum))
			{
				printf("[%s] Syntax error <%s> : no vnum, (node:%s)\n", 
					typeid(this).name(), sFileName.c_str(), stName.c_str());
				loader.SetParentNode();
				continue;
			}

			jSub["vnum"] = iVnum;

			TTokenVector* pTok;

			if (!loader.GetTokenVector("leader", &pTok))
			{
				printf("[%s] Syntax error <%s> : no leader, (node:%s)\n", 
					typeid(this).name(), sFileName.c_str(), stName.c_str());
				loader.SetParentNode();
				continue;
			}

			if (pTok->size() < 2)
			{
				printf("[%s] Syntax error <%s> : no leader vnum, (node:%s)\n", 
					typeid(this).name(), sFileName.c_str(), stName.c_str());
				loader.SetParentNode();
				continue;
			}

			// jSub["leader"] = { pTok->at(0), std::stoul(pTok->at(1)) };
			jSub["leader"] = std::stoul(pTok->at(1));

			for (int k = 1; k < 256; ++k)
			{
				char buf[4];
				snprintf(buf, sizeof(buf), "%d", k);

				if (loader.GetTokenVector(buf, &pTok))
				{
					// jSub["vnum_list"].push_back({ pTok->at(0), std::stoul(pTok->at(1)) });
					jSub["vnum_list"].push_back(std::stoul(pTok->at(1)));
					continue;
				}

				break;
			}

			loader.SetParentNode();

			m_JsonData += std::move(jSub);
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s>: %s (node:%s)\n",
				typeid(this).name(), sFileName.c_str(), ex.what(), stName.c_str());
			return false;
		}
	}

	return true;
}