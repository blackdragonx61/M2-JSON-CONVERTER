#pragma once

#include "../../Main/dumpinterface.hpp"

class CConvertBLEND : public DumpInterface
{
public:
	CConvertBLEND(const char* FileName, int32_t Indent = 4);
	~CConvertBLEND();

	bool BuildJson() override;
};