#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertITEMDESC : public DumpInterface
{
public:
	CConvertITEMDESC(const char* FileName, int32_t Indent = 4);
	~CConvertITEMDESC();

	bool BuildJson() override;
};