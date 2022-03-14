#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertITEMNAMES : public DumpInterface
{
public:
	CConvertITEMNAMES(const char* FileName, int32_t Indent = 4);
	~CConvertITEMNAMES();

	bool BuildJson() override;
};