#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertGAMECONFIG : public DumpInterface
{
public:
	CConvertGAMECONFIG(const char* FileName, int32_t Indent = 4);
	~CConvertGAMECONFIG();

	bool BuildJson() override;
};