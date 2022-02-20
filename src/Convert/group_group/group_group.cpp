#include "group_group.hpp"

CConvertGROUPGROUP::CConvertGROUPGROUP(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertGROUPGROUP::~CConvertGROUPGROUP() = default;

bool CConvertGROUPGROUP::BuildJson() /*override*/
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

			for (int k = 1; k < 256; ++k)
			{
				char buf[4];
				snprintf(buf, sizeof(buf), "%d", k);

				if (loader.GetTokenVector(buf, &pTok))
				{
					const int prob = (pTok->size() > 1) ? std::stoi(pTok->at(1)) : 1;
					jSub["vnum_list"].push_back({ std::stoul(pTok->at(0)), prob });
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