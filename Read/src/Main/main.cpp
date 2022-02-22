/*
* Author: blackdragonx61 / Mali
* 21 Feb 2022
*/

#include <iostream>

/*CLIENT*/
#include "../Read/Client/atlasinfo/atlasinfo.hpp"

static void Read(ReadInterface&& c)
{
	c.PrintJson();
}

int main()
{
	/*CLIENT*/
	Read(CReadATLASINFO{ "atlasinfo.json" });
	
	
	printf("\n");
	printf("Finished.");

	std::cin.get();
	return 0;
}