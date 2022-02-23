#pragma once

#include "../../../Main/read_interface.hpp"

class CReadITEMLIST : public ReadInterface
{
public:
	CReadITEMLIST(const char* FileName);
	~CReadITEMLIST();

	void PrintJson() override;
};