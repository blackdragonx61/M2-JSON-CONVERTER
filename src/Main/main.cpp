/*
* Author: blackdragonx61 / Mali
* 27 Oct 2021
*/

#include <Windows.h>

#include "../Convert/atlasinfo/atlasinfo.hpp"
#include "../Convert/blend/blend.hpp"
#include "../Convert/cfg/cfg.hpp"
#include "../Convert/cube/cube.hpp"
#include "../Convert/db_config/db_config.hpp"
#include "../Convert/game_config/game_config.hpp"
#include "../Convert/msm/msm.hpp"
#include "../Convert/group/group.hpp"
#include "../Convert/group_group/group_group.hpp"

static void Convert(DumpInterface&& c)
{
	if (c.BuildJson())
		c.Dump();
}

int main()
{
	SetConsoleTitle("M2JSONCONVERTER by blackdragonx61 / Mali");

	DumpInterface::CreateOutDir();

	Convert(CConvertATLASINFO{ "atlasinfo.txt" });
	Convert(CConvertBLEND{ "blend.txt" });
	Convert(CConvertCFG{ "metin2.cfg" });
	Convert(CConvertCUBE{ "cube.txt" });
	Convert(CConvertDBCONFIG{ "conf.txt" });
	Convert(CConvertGAMECONFIG{ "CONFIG" });
	Convert(CConvertMSM{ "assassin_m.msm" });
	Convert(CConvertMSM{ "assassin_w.msm" });
	Convert(CConvertMSM{ "shaman_m.msm" });
	Convert(CConvertMSM{ "shaman_w.msm" });
	Convert(CConvertMSM{ "sura_m.msm" });
	Convert(CConvertMSM{ "sura_w.msm" });
	Convert(CConvertMSM{ "warrior_m.msm" });
	Convert(CConvertMSM{ "warrior_w.msm" });
	Convert(CConvertMSM{ "wolfman_m.msm" });
	Convert(CConvertGROUP{ "group.txt" });
	Convert(CConvertGROUPGROUP{ "group_group.txt" });

	printf("\n");
	printf("Finished.");

	std::cin.get();
	return 0;
}