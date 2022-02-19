#pragma once

#include "../../Main/dumpinterface.hpp"

class CConvertCFG : public DumpInterface
{
public:
	CConvertCFG(const char* FileName, int32_t Indent = 4);
	~CConvertCFG();

	bool BuildJson() override;
};