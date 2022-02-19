#pragma once

#include "../../Main/dumpinterface.hpp"

class CConvertMSM : public DumpInterface
{
public:
	CConvertMSM(const char* FileName, int32_t Indent = 4);
	~CConvertMSM();

	bool BuildJson() override;
};