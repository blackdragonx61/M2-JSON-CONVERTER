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
	
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document d;
	d.ParseStream(isw);

	ifs.close();

	if (d.HasParseError()) {
		printf("[%s] <%s>: %s\n",
			typeid(*this).name(), sFileName.c_str(), GetParseError_En(d.GetParseError()));
		return;
	}

	for (const auto& field : d.GetArray())
	{
		auto p = std::make_unique<SAtlasInfo>();
		
		p->name = field["name"].GetString();
		p->x = field["x"].GetInt();
		p->y = field["y"].GetInt();
		p->width = field["width"].GetInt();
		p->height = field["height"].GetInt();

		vecTest.emplace_back(std::move(p));
	}

	for (const auto& elem : vecTest)
	{
		printf("name: %s, x:%d, y:%d width:%d, height:%d\n",
			elem->name.c_str(), elem->x, elem->y, elem->width, elem->height);
	}
}