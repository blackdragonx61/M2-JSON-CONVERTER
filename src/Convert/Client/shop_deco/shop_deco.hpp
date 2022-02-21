#pragma once

#include "../../../Main/dump_interface.hpp"

class CConvertSHOPDECORATION : public DumpInterface
{
public:
	CConvertSHOPDECORATION(const char* FileName, int32_t Indent = 4);
	~CConvertSHOPDECORATION();

	bool BuildJson() override;
};