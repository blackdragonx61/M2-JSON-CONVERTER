#pragma once

#include "../../../Main/read_interface.hpp"

class CReadMSM : public ReadInterface
{
public:
	CReadMSM(const char* FileName);
	~CReadMSM();

	void PrintJson() override;
};