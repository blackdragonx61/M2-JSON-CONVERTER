/*
* Author: blackdragonx61 / Mali
* 21 Feb 2022
*/

#include <iostream>

/*CLIENT*/
#include "../Read/Client/atlasinfo/atlasinfo.hpp"
#include "../Read/Client/msm/msm.hpp"

static void Read(ReadInterface&& c)
{
	c.PrintJson();
}

int main()
{
	/*CLIENT*/
	Read(CReadATLASINFO{ "atlasinfo.json" });
	Read(CReadMSM{ "assassin_m.json" });
	
	printf("\n");
	printf("Finished.");

	std::cin.get();
	return 0;
}