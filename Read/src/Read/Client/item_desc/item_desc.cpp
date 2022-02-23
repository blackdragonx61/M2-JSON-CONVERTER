#include "item_desc.hpp"

CReadITEMDESC::CReadITEMDESC(const char* FileName)
	: ReadInterface{ FileName }
{
}

CReadITEMDESC::~CReadITEMDESC() = default;

void CReadITEMDESC::PrintJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	std::ifstream ifs(sFileName);
	if (!ifs.is_open())
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return;
	}

	struct SItemDesc
	{
		std::uint64_t Vnum;
		std::string Name;
		std::string Description;
		std::string Summary;
	};

	std::vector<SItemDesc> vecTest;
	
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

		SItemDesc data{};
		data.Vnum = vnum.GetUint64();

		if (field.HasMember("name"))
		{
			const auto& name = field["name"];
			if (!name.IsString())
			{
				printf("[%s] <%s>: <name> is not string.\n",
					typeid(*this).name(), sFileName.c_str());
				return;
			}

			data.Name = name.GetString();
		}

		if (field.HasMember("description"))
		{
			const auto& description = field["description"];
			if (!description.IsString())
			{
				printf("[%s] <%s>: <description> is not string.\n",
					typeid(*this).name(), sFileName.c_str());
				return;
			}

			data.Description = description.GetString();
		}

		if (field.HasMember("summary"))
		{
			const auto& summary = field["summary"];
			if (!summary.IsString())
			{
				printf("[%s] <%s>: <summary> is not string.\n",
					typeid(*this).name(), sFileName.c_str());
				return;
			}

			data.Summary = summary.GetString();
		}

		vecTest.emplace_back(std::move(data));
	}

	for (const auto& elem : vecTest)
		printf("vnum:%llu\tname:%s\tdesc:%s\tsum:%s\n", 
			elem.Vnum, elem.Name.c_str(), elem.Description.c_str(), elem.Summary.c_str());
}