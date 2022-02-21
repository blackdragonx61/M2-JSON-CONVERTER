#pragma once

#include "../../Main/dump_interface.hpp"

class CConvertITEMMASK : public DumpInterface
{
public:
	CConvertITEMMASK(const char* FileName, int32_t Indent = 4);
	~CConvertITEMMASK();

	bool BuildJson() override;
};