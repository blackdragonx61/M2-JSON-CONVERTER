#pragma once

#include "../../../Main/read_interface.hpp"

class CReadITEMDESC : public ReadInterface
{
public:
	CReadITEMDESC(const char* FileName);
	~CReadITEMDESC();

	void PrintJson() override;
};