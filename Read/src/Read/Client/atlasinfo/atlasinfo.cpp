#include "atlasinfo.hpp"

CReadATLASINFO::CReadATLASINFO(const char* FileName)
	: ReadInterface{ FileName }
{
}

CReadATLASINFO::~CReadATLASINFO() = default;

void CReadATLASINFO::PrintJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	std::ifstream ifs(sFileName);
	if (!ifs.is_open())
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return;
	}

	struct SAtlasInfo
	{
		std::string name;
		int x, y;
		int width, height;
	};

	std::vector<std::unique_ptr<SAtlasInfo>> vecTest;
	
	try
	{
		json jRead = json::parse(ifs);
		for (const auto& item : jRead.items())
		{
			auto p = std::make_unique<SAtlasInfo>();
			p->name = item.value()["name"].get<decltype(SAtlasInfo::name)>();
			p->x = item.value()["x"].get<decltype(SAtlasInfo::x)>();
			p->y = item.value()["y"].get<decltype(SAtlasInfo::y)>();
			p->width = item.value()["width"].get<decltype(SAtlasInfo::width)>();
			p->height = item.value()["height"].get<decltype(SAtlasInfo::height)>();

			vecTest.emplace_back(std::move(p));
		}

		for (const auto& elem : vecTest)
		{
			printf("name: %s, x:%d, y:%d width:%d, height:%d\n", 
				elem->name.c_str(), elem->x, elem->y, elem->width, elem->height);
		}
	}
	catch (const std::exception& ex)
	{
		printf("[%s] <%s>: %s\n",
			typeid(*this).name(), sFileName.c_str(), ex.what());
		ifs.close();
		return;
	}

	ifs.close();
}