#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertRACEHEIGHT : public DumpInterface
{
public:
	CConvertRACEHEIGHT(const char* FileName, int32_t Indent = 4);
	~CConvertRACEHEIGHT();

	bool BuildJson() override;
};