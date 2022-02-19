/*
* Author: blackdragonx61 / Mali
* 27 Oct 2021
*/

#include <Windows.h>

#include "../Convert/cfg/cfg.hpp"
#include "../Convert/cube/cube.hpp"
#include "../Convert/db_config/db_config.hpp"
#include "../Convert/msm/msm.hpp"

static void Convert(DumpInterface* c)
{
	if (c->BuildJson())
		c->Dump();

	delete c;
}

int main()
{
	SetConsoleTitle("M2JSONCONVERTER by blackdragonx61 / Mali");

	DumpInterface::CreateOutDir();

	Convert(new CConvertCFG{ "metin2.cfg" });
	Convert(new CConvertCUBE{ "cube.txt" });
	Convert(new CConvertDBCONFIG{ "conf.txt" });
	Convert(new CConvertMSM{ "assassin_m.msm" });
	Convert(new CConvertMSM{ "assassin_w.msm" });
	Convert(new CConvertMSM{ "shaman_m.msm" });
	Convert(new CConvertMSM{ "shaman_w.msm" });
	Convert(new CConvertMSM{ "sura_m.msm" });
	Convert(new CConvertMSM{ "sura_w.msm" });
	Convert(new CConvertMSM{ "warrior_m.msm" });
	Convert(new CConvertMSM{ "warrior_w.msm" });
	Convert(new CConvertMSM{ "wolfman_m.msm" });

	printf("\n");
	printf("Finished.");

	std::cin.get();
	return 0;
}