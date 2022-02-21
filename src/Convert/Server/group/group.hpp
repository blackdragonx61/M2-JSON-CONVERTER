#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertGROUP : public DumpInterface
{
public:
	CConvertGROUP(const char* FileName, int32_t Indent = 4);
	~CConvertGROUP();

	bool BuildJson() override;
};