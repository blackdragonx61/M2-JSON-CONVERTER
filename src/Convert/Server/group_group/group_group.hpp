#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertGROUPGROUP : public DumpInterface
{
public:
	CConvertGROUPGROUP(const char* FileName, int32_t Indent = 4);
	~CConvertGROUPGROUP();

	bool BuildJson() override;
};