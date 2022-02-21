#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertMSM : public DumpInterface
{
public:
	CConvertMSM(const char* FileName, int32_t Indent = 4);
	~CConvertMSM();

	bool BuildJson() override;
};