#include "item_list.hpp"

CReadITEMLIST::CReadITEMLIST(const char* FileName)
	: ReadInterface{ FileName }
{
}

CReadITEMLIST::~CReadITEMLIST() = default;

void CReadITEMLIST::PrintJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	std::ifstream ifs(sFileName);
	if (!ifs.is_open())
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return;
	}

	struct SItemList
	{
		std::uint64_t Vnum;
		std::string Type;
		std::string Icon;
		std::string Model;
	};

	std::vector<SItemList> vecTest;
	
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
		if (!field.HasMember("vnum"))
		{
			printf("[%s] <%s>: Cannot find <vnum>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& vnum = field["vnum"];
		if (!vnum.IsNumber())
		{
			printf("[%s] <%s>: <vnum> is not number.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		SItemList data{};
		data.Vnum = vnum.GetUint64();

		if (field.HasMember("type"))
		{
			const auto& type = field["type"];
			if (!type.IsString())
			{
				printf("[%s] <%s>: <type> is not string.\n",
					typeid(*this).name(), sFileName.c_str());
				return;
			}

			data.Type = type.GetString();
		}

		if (field.HasMember("icon"))
		{
			const auto& icon = field["icon"];
			if (!icon.IsString())
			{
				printf("[%s] <%s>: <icon> is not string.\n",
					typeid(*this).name(), sFileName.c_str());
				return;
			}

			data.Icon = icon.GetString();
		}

		if (field.HasMember("model"))
		{
			const auto& model = field["model"];
			if (!model.IsString())
			{
				printf("[%s] <%s>: <model> is not string.\n",
					typeid(*this).name(), sFileName.c_str());
				return;
			}

			data.Model = model.GetString();
		}

		vecTest.emplace_back(std::move(data));
	}

	for (const auto& elem : vecTest)
		printf("vnum:%llu\ttype:%s\ticon:%s\tmodel:%s\n", 
			elem.Vnum, elem.Type.c_str(), elem.Icon.c_str(), elem.Model.c_str());
}