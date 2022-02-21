#include "item_mask.hpp"

CConvertITEMMASK::CConvertITEMMASK(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertITEMMASK::~CConvertITEMMASK() = default;

bool CConvertITEMMASK::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	FILE* fp{ fopen(sFileName.c_str(), "rt") };

	if (fp == nullptr)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}

	int ori_vnum, new_vnum;
	while (fscanf(fp, "%d %d", &ori_vnum, &new_vnum) != EOF)
	{
		m_JsonData += {
			{ "new_vnum", new_vnum },
			{ "ori_vnum", ori_vnum },
		};
	}

	fclose(fp);
	return true;
}