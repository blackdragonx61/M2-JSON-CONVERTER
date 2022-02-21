#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertITEMLIST : public DumpInterface
{
public:
	CConvertITEMLIST(const char* FileName, int32_t Indent = 4);
	~CConvertITEMLIST();

	bool BuildJson() override;
};