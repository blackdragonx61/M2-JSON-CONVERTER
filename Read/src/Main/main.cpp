/*
* Author: blackdragonx61 / Mali
* 21 Feb 2022
*/

#include <Windows.h>
#include <iostream>

/*CLIENT*/
#include "../Read/Client/atlasinfo/atlasinfo.hpp"

static void Read(ReadInterface&& c)
{
	c.PrintJson();
}

int main()
{
	SetConsoleTitle("M2JSONCONVERTER by blackdragonx61 / Mali");

	/*CLIENT*/
	Read(CReadATLASINFO{ "atlasinfo.json" });
	
	
	printf("\n");
	printf("Finished.");

	std::cin.get();
	return 0;
}