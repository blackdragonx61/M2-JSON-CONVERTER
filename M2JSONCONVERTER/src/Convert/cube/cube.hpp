﻿#pragma once

#include "../../Main/dumpinterface.hpp"

class CConvertCUBE : public DumpInterface
{
public:
	CConvertCUBE(const char* FileName, int32_t Indent = 4);
	~CConvertCUBE();

	bool BuildJson() override;
};
