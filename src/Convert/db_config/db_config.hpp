#pragma once

#include "../../Main/dump_interface.hpp"

class CConvertDBCONFIG : public DumpInterface
{
public:
	CConvertDBCONFIG(const char* FileName, int32_t Indent = 4);
	~CConvertDBCONFIG();

	bool BuildJson() override;

private:
	void NextLine(FILE* fp);
	bool GetWord(FILE* fp, char* tar);
	bool GetLine(FILE* fp, char* dest);

	std::unordered_map<std::string, std::string>	m_valueMap;
};