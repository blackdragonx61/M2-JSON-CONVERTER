#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertNPCLIST : public DumpInterface
{
public:
	CConvertNPCLIST(const char* FileName, int32_t Indent = 4);
	~CConvertNPCLIST();

	bool BuildJson() override;
};