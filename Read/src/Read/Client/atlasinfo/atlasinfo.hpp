#pragma once

#include "../../../Main/read_interface.hpp"

class CReadATLASINFO : public ReadInterface
{
public:
	CReadATLASINFO(const char* FileName);
	~CReadATLASINFO();

	void PrintJson() override;
};