#include "item_names.hpp"

CConvertITEMNAMES::CConvertITEMNAMES(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertITEMNAMES::~CConvertITEMNAMES() = default;

bool CConvertITEMNAMES::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	std::ifstream ifs(sFileName);

	if (ifs.is_open() == false)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return false;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		const auto pos = line.find("\t");
		if (pos == std::string::npos)
			continue;

		json jSub;
		try
		{
			jSub["vnum"] = std::stoull(line.substr(0, pos));
			jSub["name"] = line.substr(pos + 1);
		}
		catch (const std::exception&)
		{
			continue;
		}

		m_JsonData += std::move(jSub);
	}

	ifs.close();
	return true;
}