#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertATLASINFO : public DumpInterface
{
public:
	CConvertATLASINFO(const char* FileName, int32_t Indent = 4);
	~CConvertATLASINFO();

	bool BuildJson() override;
};