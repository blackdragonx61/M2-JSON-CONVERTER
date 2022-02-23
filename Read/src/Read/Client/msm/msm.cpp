#include "msm.hpp"

CReadMSM::CReadMSM(const char* FileName)
	: ReadInterface{ FileName }
{
}

CReadMSM::~CReadMSM() = default;

typedef struct D3DXVECTOR3
{
	float x, y, z;
} D3DXVECTOR3, * LPD3DXVECTOR3;

void CReadMSM::PrintJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	std::ifstream ifs(sFileName);
	if (!ifs.is_open())
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document d;
	d.ParseStream(isw);

	ifs.close();

	if (d.HasParseError()) {
		printf("[%s] <%s>: %s\n",
			typeid(*this).name(), sFileName.c_str(), GetParseError_En(d.GetParseError()));
		return;
	}

	//
	std::string sBaseModelFileName;

	if (!d.HasMember("BaseModelFileName"))
	{
		printf("[%s] <%s>: Cannot find <BaseModelFileName>.\n",
			typeid(*this).name(), sFileName.c_str());
		return;
	}

	const auto& BaseModelFileName = d["BaseModelFileName"];
	if (!BaseModelFileName.IsString())
	{
		printf("[%s] <%s>: <BaseModelFileName> is not string.\n",
			typeid(*this).name(), sFileName.c_str());
		return;
	}

	printf("BaseModelFileName:%s\n", BaseModelFileName.GetString());

	// Hair Data
	{
		if (!d.HasMember("HairData"))
		{
			printf("[%s] <%s>: Cannot find <HairData>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& HairData = d["HairData"];
		if (!HairData.IsObject())
		{
			printf("[%s] <%s>: <HairData> is not object.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		struct SHAIRSUB
		{
			int HairIndex;
			std::string Model;
			std::string SourceSkin;
			std::string TargetSkin;
		};

		struct SHAIRDATA
		{
			std::string PathName;
			int HairDataCount;
			std::vector<SHAIRSUB> HairSub;
		};

		if (!HairData.HasMember("PathName"))
		{
			printf("[%s] <%s>: Cannot find <HairData><PathName>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& PathName = HairData["PathName"];
		if (!PathName.IsString())
		{
			printf("[%s] <%s>: <HairData><PathName> is not string.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		if (!HairData.HasMember("HairDataCount"))
		{
			printf("[%s] <%s>: Cannot find <HairData><HairDataCount>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& HairDataCount = HairData["HairDataCount"];
		if (!HairDataCount.IsNumber())
		{
			printf("[%s] <%s>: <HairData><HairDataCount> is not number.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		SHAIRDATA mainData{};
		mainData.PathName = PathName.GetString();
		mainData.HairDataCount = HairDataCount.GetInt();

		mainData.HairSub.reserve(mainData.HairDataCount);

		//Hair Sub Data
		{
			for (int i = 0; i < mainData.HairDataCount; i++)
			{
				const std::string __HairDataSubName = std::format("HairData{:02}", i);
				if (!HairData.HasMember(__HairDataSubName.c_str()))
				{
					printf("[%s] <%s>: Cannot find <HairData><%s>\n",
						typeid(*this).name(), sFileName.c_str(), __HairDataSubName.c_str());
					return;
				}

				const auto& HairSubData = HairData[__HairDataSubName.c_str()];
				if (!HairSubData.IsObject())
				{
					printf("[%s] <%s>: <HairData><%s> is not object.\n",
						typeid(*this).name(), sFileName.c_str(), __HairDataSubName.c_str());
					return;
				}

				SHAIRSUB subData{};

				if (HairSubData.HasMember("HairIndex"))
				{
					const auto& HairSubDataIndex = HairSubData["HairIndex"];
					if (!HairSubDataIndex.IsNumber())
					{
						printf("[%s] <%s>: <HairData><%s><HairIndex> is not number.\n",
							typeid(*this).name(), sFileName.c_str(), __HairDataSubName.c_str());
						return;
					}

					subData.HairIndex = HairSubDataIndex.GetInt();
				}

				if (HairSubData.HasMember("Model"))
				{
					const auto& HairSubDataModel = HairSubData["Model"];
					if (!HairSubDataModel.IsString())
					{
						printf("[%s] <%s>: <HairData><%s><Model> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __HairDataSubName.c_str());
						return;
					}

					subData.Model = HairSubDataModel.GetString();
				}

				if (HairSubData.HasMember("SourceSkin"))
				{
					const auto& HairSubDataSourceSkin = HairSubData["SourceSkin"];
					if (!HairSubDataSourceSkin.IsString())
					{
						printf("[%s] <%s>: <HairData><%s><SourceSkin> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __HairDataSubName.c_str());
						return;
					}

					subData.SourceSkin = HairSubDataSourceSkin.GetString();
				}

				if (HairSubData.HasMember("TargetSkin"))
				{
					const auto& HairSubDataTargetSkin = HairSubData["TargetSkin"];
					if (!HairSubDataTargetSkin.IsString())
					{
						printf("[%s] <%s>: <HairData><%s><TargetSkin> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __HairDataSubName.c_str());
						return;
					}

					subData.TargetSkin = HairSubDataTargetSkin.GetString();
				}

				mainData.HairSub.emplace_back(std::move(subData));
			}
		}

		printf("<HairData><PathName>:%s\n", mainData.PathName.c_str());
		printf("<HairData><HairDataCount>:%d\n", mainData.HairDataCount);

		for (size_t i = 0; i < mainData.HairSub.size(); i++)
		{
			const auto& sub = mainData.HairSub.at(i);
			const std::string __HairDataSubName = std::format("HairData{:02}", i);
			printf(std::format("<HairData><{}><HairIndex>:{}\n", __HairDataSubName.c_str(), sub.HairIndex).c_str());
			printf(std::format("<HairData><{}><Model>:{}\n", __HairDataSubName.c_str(), sub.Model).c_str());
			printf(std::format("<HairData><{}><SourceSkin>:{}\n", __HairDataSubName.c_str(), sub.SourceSkin).c_str());
			printf(std::format("<HairData><{}><TargetSkin>:{}\n", __HairDataSubName.c_str(), sub.TargetSkin).c_str());
		}
	}

	// Shape Data
	{
		if (!d.HasMember("ShapeData"))
		{
			printf("[%s] <%s>: Cannot find <ShapeData>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& ShapeData = d["ShapeData"];
		if (!ShapeData.IsObject())
		{
			printf("[%s] <%s>: <ShapeData> is not object.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		struct SSHAPESUB
		{
			int ShapeIndex;
			std::string Model;
			std::string SourceSkin;
			std::string TargetSkin;

			std::string SpecialPath;
			std::string SourceSkin2;
			std::string TargetSkin2;
		};

		struct SSHAPEDATA
		{
			std::string PathName;
			int ShapeDataCount;
			std::vector<SSHAPESUB> ShapeSub;
		};

		if (!ShapeData.HasMember("PathName"))
		{
			printf("[%s] <%s>: Cannot find <ShapeData><PathName>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& PathName = ShapeData["PathName"];
		if (!PathName.IsString())
		{
			printf("[%s] <%s>: <ShapeData><PathName> is not string.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		if (!ShapeData.HasMember("ShapeDataCount"))
		{
			printf("[%s] <%s>: Cannot find <ShapeData><ShapeDataCount>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& ShapeDataCount = ShapeData["ShapeDataCount"];
		if (!ShapeDataCount.IsNumber())
		{
			printf("[%s] <%s>: <ShapeData><ShapeDataCount> is not number.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		SSHAPEDATA mainData{};
		mainData.PathName = PathName.GetString();
		mainData.ShapeDataCount = ShapeDataCount.GetInt();

		mainData.ShapeSub.reserve(mainData.ShapeDataCount);

		//Hair Sub Data
		{
			for (int i = 0; i < mainData.ShapeDataCount; i++)
			{
				const std::string __ShapeDataSubName = std::format("ShapeData{:02}", i);
				if (!ShapeData.HasMember(__ShapeDataSubName.c_str()))
				{
					printf("[%s] <%s>: Cannot find <ShapeData><%s>\n",
						typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
					return;
				}

				const auto& ShapeSubData = ShapeData[__ShapeDataSubName.c_str()];
				if (!ShapeSubData.IsObject())
				{
					printf("[%s] <%s>: <ShapeData><%s> is not object.\n",
						typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
					return;
				}

				SSHAPESUB subData{};

				if (ShapeSubData.HasMember("ShapeIndex"))
				{
					const auto& ShapeSubDataIndex = ShapeSubData["ShapeIndex"];
					if (!ShapeSubDataIndex.IsNumber())
					{
						printf("[%s] <%s>: <ShapeData><%s><ShapeIndex> is not number.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.ShapeIndex = ShapeSubDataIndex.GetInt();
				}

				if (ShapeSubData.HasMember("Model"))
				{
					const auto& ShapeSubDataModel = ShapeSubData["Model"];
					if (!ShapeSubDataModel.IsString())
					{
						printf("[%s] <%s>: <ShapeData><%s><Model> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.Model = ShapeSubDataModel.GetString();
				}

				if (ShapeSubData.HasMember("SourceSkin"))
				{
					const auto& ShapeSubDataSourceSkin = ShapeSubData["SourceSkin"];
					if (!ShapeSubDataSourceSkin.IsString())
					{
						printf("[%s] <%s>: <ShapeData><%s><SourceSkin> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.SourceSkin = ShapeSubDataSourceSkin.GetString();
				}

				if (ShapeSubData.HasMember("TargetSkin"))
				{
					const auto& ShapeSubDataTargetSkin = ShapeSubData["TargetSkin"];
					if (!ShapeSubDataTargetSkin.IsString())
					{
						printf("[%s] <%s>: <ShapeData><%s><TargetSkin> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.TargetSkin = ShapeSubDataTargetSkin.GetString();
				}

				if (ShapeSubData.HasMember("SpecialPath"))
				{
					const auto& ShapeSubDataSpecialPath = ShapeSubData["SpecialPath"];
					if (!ShapeSubDataSpecialPath.IsString())
					{
						printf("[%s] <%s>: <ShapeData><%s><SpecialPath> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.SpecialPath = ShapeSubDataSpecialPath.GetString();
				}

				if (ShapeSubData.HasMember("SourceSkin2"))
				{
					const auto& ShapeSubDataSourceSkin2 = ShapeSubData["SourceSkin2"];
					if (!ShapeSubDataSourceSkin2.IsString())
					{
						printf("[%s] <%s>: <ShapeData><%s><SourceSkin2> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.SourceSkin2 = ShapeSubDataSourceSkin2.GetString();
				}

				if (ShapeSubData.HasMember("TargetSkin2"))
				{
					const auto& ShapeSubDataTargetSkin2 = ShapeSubData["TargetSkin2"];
					if (!ShapeSubDataTargetSkin2.IsString())
					{
						printf("[%s] <%s>: <ShapeData><%s><TargetSkin2> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __ShapeDataSubName.c_str());
						return;
					}

					subData.TargetSkin2 = ShapeSubDataTargetSkin2.GetString();
				}

				mainData.ShapeSub.emplace_back(std::move(subData));
			}
		}

		printf("<ShapeData><PathName>:%s\n", mainData.PathName.c_str());
		printf("<ShapeData><ShapeDataCount>:%d\n", mainData.ShapeDataCount);

		for (size_t i = 0; i < mainData.ShapeSub.size(); i++)
		{
			const auto& sub = mainData.ShapeSub.at(i);
			const std::string __ShapeDataSubName = std::format("ShapeData{:02}", i);
			printf(std::format("<ShapeData><{}><ShapeIndex>:{}\n", __ShapeDataSubName.c_str(), sub.ShapeIndex).c_str());
			printf(std::format("<ShapeData><{}><Model>:{}\n", __ShapeDataSubName.c_str(), sub.Model).c_str());
			printf(std::format("<ShapeData><{}><SourceSkin>:{}\n", __ShapeDataSubName.c_str(), sub.SourceSkin).c_str());
			printf(std::format("<ShapeData><{}><TargetSkin>:{}\n", __ShapeDataSubName.c_str(), sub.TargetSkin).c_str());
			printf(std::format("<ShapeData><{}><SpecialPath>:{}\n", __ShapeDataSubName.c_str(), sub.SpecialPath).c_str());
			printf(std::format("<ShapeData><{}><SourceSkin2>:{}\n", __ShapeDataSubName.c_str(), sub.SourceSkin).c_str());
			printf(std::format("<ShapeData><{}><TargetSkin2>:{}\n", __ShapeDataSubName.c_str(), sub.TargetSkin2).c_str());
		}
	}

	// Attaching Data
	{
		if (!d.HasMember("AttachingData"))
		{
			printf("[%s] <%s>: Cannot find <AttachingData>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& AttachingData = d["AttachingData"];
		if (!AttachingData.IsObject())
		{
			printf("[%s] <%s>: <AttachingData> is not object.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		struct SSPHEREDATA
		{
			float Radius;
			D3DXVECTOR3 Position;
		};

		struct SATTACHSUB
		{
			int AttachingDataType;
			int isAttaching;
			int AttachingModelIndex;
			std::string AttachingBoneName;
			int CollisionType;
			std::vector<SSPHEREDATA> Sphere;
		};

		struct SATTACHDATA
		{
			int AttachingDataCount;
			std::vector<SATTACHSUB> AttachingSub;
		};

		if (!AttachingData.HasMember("AttachingDataCount"))
		{
			printf("[%s] <%s>: Cannot find <AttachingData><AttachingDataCount>.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		const auto& AttachingDataCount = AttachingData["AttachingDataCount"];
		if (!AttachingDataCount.IsNumber())
		{
			printf("[%s] <%s>: <AttachingData><AttachingDataCount> is not number.\n",
				typeid(*this).name(), sFileName.c_str());
			return;
		}

		SATTACHDATA mainData{};
		mainData.AttachingDataCount = AttachingDataCount.GetInt();
		
		mainData.AttachingSub.reserve(mainData.AttachingDataCount);

		//Attach Sub Data
		{
			for (int i = 0; i < mainData.AttachingDataCount; i++)
			{
				const std::string __AttachingDataSubName = std::format("AttachingData{:02}", i);
				if (!AttachingData.HasMember(__AttachingDataSubName.c_str()))
				{
					printf("[%s] <%s>: Cannot find <AttachingData><%s>\n",
						typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
					return;
				}

				const auto& AttachingSubData = AttachingData[__AttachingDataSubName.c_str()];
				if (!AttachingSubData.IsObject())
				{
					printf("[%s] <%s>: <AttachingData><%s> is not object.\n",
						typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
					return;
				}

				SATTACHSUB subData{};

				if (AttachingSubData.HasMember("AttachingDataType"))
				{
					const auto& AttachingSubDataDataType = AttachingSubData["AttachingDataType"];
					if (!AttachingSubDataDataType.IsNumber())
					{
						printf("[%s] <%s>: <AttachingData><%s><AttachingDataType> is not number.\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
						return;
					}

					subData.AttachingDataType = AttachingSubDataDataType.GetInt();
				}

				if (AttachingSubData.HasMember("isAttaching"))
				{
					const auto& AttachingSubDataisAttaching = AttachingSubData["isAttaching"];
					if (!AttachingSubDataisAttaching.IsNumber())
					{
						printf("[%s] <%s>: <AttachingData><%s><isAttaching> is not number.\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
						return;
					}

					subData.isAttaching = AttachingSubDataisAttaching.GetInt();
				}

				if (AttachingSubData.HasMember("AttachingModelIndex"))
				{
					const auto& AttachingSubDataAttachingModelIndex = AttachingSubData["AttachingModelIndex"];
					if (!AttachingSubDataAttachingModelIndex.IsNumber())
					{
						printf("[%s] <%s>: <AttachingData><%s><AttachingModelIndex> is not number.\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
						return;
					}

					subData.AttachingModelIndex = AttachingSubDataAttachingModelIndex.GetInt();
				}

				if (AttachingSubData.HasMember("AttachingBoneName"))
				{
					const auto& AttachingSubDataAttachingBoneName = AttachingSubData["AttachingBoneName"];
					if (!AttachingSubDataAttachingBoneName.IsString())
					{
						printf("[%s] <%s>: <AttachingData><%s><AttachingBoneName> is not string.\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
						return;
					}

					subData.AttachingBoneName = AttachingSubDataAttachingBoneName.GetString();
				}

				if (AttachingSubData.HasMember("CollisionType"))
				{
					const auto& AttachingSubDataAttachingCollisionType = AttachingSubData["CollisionType"];
					if (!AttachingSubDataAttachingCollisionType.IsNumber())
					{
						printf("[%s] <%s>: <AttachingData><%s><CollisionType> is not number.\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str());
						return;
					}

					subData.CollisionType = AttachingSubDataAttachingCollisionType.GetInt();
				}

				subData.Sphere.reserve(subData.CollisionType);

				//Sphere Data
				for (int j = 0; j < subData.CollisionType; j++)
				{
					const std::string __SphereDataName = std::format("SphereData{:02}", j);
					if (!AttachingSubData.HasMember(__SphereDataName.c_str()))
					{
						printf("[%s] <%s>: Cannot find <AttachingData><%s><%s>\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str(), __SphereDataName.c_str());
						return;
					}

					const auto& SphereData = AttachingSubData[__SphereDataName.c_str()];
					if (!SphereData.IsObject())
					{
						printf("[%s] <%s>: <AttachingData><%s><%s> is not object.\n",
							typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str(), __SphereDataName.c_str());
						return;
					}

					SSPHEREDATA sphereData{};

					if (SphereData.HasMember("Radius"))
					{
						const auto& SphereDataRadius = SphereData["Radius"];
						if (!SphereDataRadius.IsFloat())
						{
							printf("[%s] <%s>: <AttachingData><%s><%s><Radius> is not float.\n",
								typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str(), __SphereDataName.c_str());
							return;
						}

						sphereData.Radius = SphereDataRadius.GetFloat();
					}

					if (SphereData.HasMember("Position"))
					{
						const auto& SphereDataPosition = SphereData["Position"];
						if (!SphereDataPosition.IsArray())
						{
							printf("[%s] <%s>: <AttachingData><%s><%s><Position> is not array.\n",
								typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str(), __SphereDataName.c_str());
							return;
						}

						if (SphereDataPosition.Size() != 3)
						{
							printf("[%s] <%s>: <AttachingData><%s><%s><Position> size is not 3.\n",
								typeid(*this).name(), sFileName.c_str(), __AttachingDataSubName.c_str(), __SphereDataName.c_str());
							return;
						}

						sphereData.Position.x = SphereDataPosition[0].GetFloat();
						sphereData.Position.y = SphereDataPosition[1].GetFloat();
						sphereData.Position.z = SphereDataPosition[2].GetFloat();
					}

					subData.Sphere.emplace_back(std::move(sphereData));
				}

				mainData.AttachingSub.emplace_back(std::move(subData));
			}

			printf("<AttachingData><AttachingDataCount>:%d\n", mainData.AttachingDataCount);

			for (size_t i = 0; i < mainData.AttachingSub.size(); i++)
			{
				const auto& sub = mainData.AttachingSub.at(i);
				const std::string __AttachingDataSubName = std::format("AttachingData{:02}", i);
				printf(std::format("<AttachingData><{}><AttachingDataType>:{}\n", __AttachingDataSubName.c_str(), sub.AttachingDataType).c_str());
				printf(std::format("<AttachingData><{}><isAttaching>:{}\n", __AttachingDataSubName.c_str(), sub.isAttaching).c_str());
				printf(std::format("<AttachingData><{}><AttachingModelIndex>:{}\n", __AttachingDataSubName.c_str(), sub.AttachingModelIndex).c_str());
				printf(std::format("<AttachingData><{}><AttachingBoneName>:{}\n", __AttachingDataSubName.c_str(), sub.AttachingBoneName).c_str());
				printf(std::format("<AttachingData><{}><CollisionType>:{}\n", __AttachingDataSubName.c_str(), sub.CollisionType).c_str());
			}
		}
	}
}