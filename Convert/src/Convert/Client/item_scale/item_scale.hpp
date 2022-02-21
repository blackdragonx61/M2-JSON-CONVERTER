#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertITEMSCALE : public DumpInterface
{
public:
	CConvertITEMSCALE(const char* FileName, int32_t Indent = 4);
	~CConvertITEMSCALE();

	bool BuildJson() override;
};