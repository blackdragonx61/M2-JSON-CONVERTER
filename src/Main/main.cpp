/*
* Author: blackdragonx61 / Mali
* 27 Oct 2021
*/

#include <Windows.h>

/*CLIENT*/
#include "../Convert/Client/atlasinfo/atlasinfo.hpp"
#include "../Convert/Client/cfg/cfg.hpp"
#include "../Convert/Client/msm/msm.hpp"
#include "../Convert/Client/item_desc/item_desc.hpp"
#include "../Convert/Client/item_list/item_list.hpp"
#include "../Convert/Client/item_scale/item_scale.hpp"
#include "../Convert/Client/race_height/race_height.hpp"

/*SERVER*/
#include "../Convert/Server/blend/blend.hpp"
#include "../Convert/Server/cube/cube.hpp"
#include "../Convert/Server/db_config/db_config.hpp"
#include "../Convert/Server/game_config/game_config.hpp"
#include "../Convert/Server/group/group.hpp"
#include "../Convert/Server/group_group/group_group.hpp"
#include "../Convert/Server/item_mask/item_mask.hpp"

static void Convert(DumpInterface&& c)
{
	if (c.BuildJson())
		c.Dump();
}

int main()
{
	SetConsoleTitle("M2JSONCONVERTER by blackdragonx61 / Mali");

	DumpInterface::CreateOutDir();

	/*CLIENT*/
	Convert(CConvertATLASINFO{ "atlasinfo.txt" });
	Convert(CConvertCFG{ "metin2.cfg" });
	Convert(CConvertMSM{ "assassin_m.msm" });
	Convert(CConvertMSM{ "assassin_w.msm" });
	Convert(CConvertMSM{ "shaman_m.msm" });
	Convert(CConvertMSM{ "shaman_w.msm" });
	Convert(CConvertMSM{ "sura_m.msm" });
	Convert(CConvertMSM{ "sura_w.msm" });
	Convert(CConvertMSM{ "warrior_m.msm" });
	Convert(CConvertMSM{ "warrior_w.msm" });
	Convert(CConvertMSM{ "wolfman_m.msm" });
	Convert(CConvertITEMDESC{ "itemdesc.txt" });
	Convert(CConvertITEMLIST{ "item_list.txt" });
	Convert(CConvertITEMSCALE{ "item_scale.txt" });
	Convert(CConvertRACEHEIGHT{ "race_height.txt" });

	/*SERVER*/
	Convert(CConvertBLEND{ "blend.txt" });
	Convert(CConvertCUBE{ "cube.txt" });
	Convert(CConvertDBCONFIG{ "conf.txt" });
	Convert(CConvertGAMECONFIG{ "CONFIG" });
	Convert(CConvertGROUP{ "group.txt" });
	Convert(CConvertGROUPGROUP{ "group_group.txt" });
	Convert(CConvertITEMMASK{ "ori_to_new_table.txt" });

	printf("\n");
	printf("Finished.");

	std::cin.get();
	return 0;
}