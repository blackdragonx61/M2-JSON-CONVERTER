#include "msm.hpp"

CConvertMSM::CConvertMSM(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertMSM::~CConvertMSM() = default;

#pragma warning(disable:26800)

bool CConvertMSM::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	CTextFileLoader TextFileLoader;
	if (!TextFileLoader.Load(sFileName.c_str()))
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}

	TextFileLoader.SetTop();

	std::string m_strBaseModelFileName;
	TextFileLoader.GetTokenString("basemodelfilename", &m_strBaseModelFileName);
	m_JsonData.emplace("BaseModelFileName", std::move(m_strBaseModelFileName));

	//Hair
	if (TextFileLoader.SetChildNode("hairdata"))
	{
		json jHair;
		
		std::string strPathName;
		DWORD dwHairDataCount = 0;

		if (TextFileLoader.GetTokenString("pathname", &strPathName) && TextFileLoader.GetTokenDoubleWord("hairdatacount", &dwHairDataCount))
		{
			jHair.emplace("PathName", std::move(strPathName));
			jHair["HairDataCount"] = dwHairDataCount;

			for (DWORD i = 0; i < dwHairDataCount; ++i)
			{
				if (!TextFileLoader.SetChildNode("hairdata", i))
				{
					TextFileLoader.SetParentNode();
					continue;
				}

				json jSub;

				DWORD dwShapeIndex;
				if (!TextFileLoader.GetTokenDoubleWord("hairindex", &dwShapeIndex))
				{
					TextFileLoader.SetParentNode();
					continue;
				}

				jSub["HairIndex"] = dwShapeIndex;

				std::string sGet{};

				TextFileLoader.GetTokenString("specialpath", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("SpecialPath", std::move(sGet));
				
				TextFileLoader.GetTokenString("model", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("Model", std::move(sGet));

				TextFileLoader.GetTokenString("sourceskin", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("SourceSkin", std::move(sGet));

				TextFileLoader.GetTokenString("targetskin", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("TargetSkin", std::move(sGet));

				TextFileLoader.GetTokenString("sourceskin2", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("SourceSkin2", std::move(sGet));

				TextFileLoader.GetTokenString("targetskin2", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("TargetSkin2", std::move(sGet));

				TextFileLoader.SetParentNode();

				jHair.emplace(std::format("HairData{:02}", i), std::move(jSub));
			}
		}
		
		TextFileLoader.SetParentNode();

		m_JsonData.emplace("HairData", std::move(jHair));
	}

	///Shape
	if (TextFileLoader.SetChildNode("shapedata"))
	{
		json jShape;
		
		std::string strPathName;
		DWORD dwShapeDataCount = 0;
		if (TextFileLoader.GetTokenString("pathname", &strPathName) && TextFileLoader.GetTokenDoubleWord("shapedatacount", &dwShapeDataCount))
		{
			jShape.emplace("PathName", std::move(strPathName));
			jShape["ShapeDataCount"] = dwShapeDataCount;
			
			for (DWORD i = 0; i < dwShapeDataCount; ++i)
			{
				if (!TextFileLoader.SetChildNode("shapedata", i))
				{
					TextFileLoader.SetParentNode();
					continue;
				}

				json jSub;

				DWORD dwShapeIndex;
				if (!TextFileLoader.GetTokenDoubleWord("shapeindex", &dwShapeIndex))
				{
					TextFileLoader.SetParentNode();
					continue;
				}

				jSub["ShapeIndex"] = dwShapeIndex;
				
				std::string sGet{};
				TextFileLoader.GetTokenString("specialpath", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("SpecialPath", std::move(sGet));

				if (!TextFileLoader.GetTokenString("model", &sGet)
					&& !TextFileLoader.GetTokenString("local_model", &sGet))
				{
					TextFileLoader.SetParentNode();
					continue;
				}

				if (sGet.empty() == false)
					jSub.emplace("Model", std::move(sGet));

				TextFileLoader.GetTokenString("sourceskin", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("SourceSkin", std::move(sGet));

				TextFileLoader.GetTokenString("targetskin", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("TargetSkin", std::move(sGet));

				TextFileLoader.GetTokenString("sourceskin2", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("SourceSkin2", std::move(sGet));

				TextFileLoader.GetTokenString("targetskin2", &sGet);
				if (sGet.empty() == false)
					jSub.emplace("TargetSkin2", std::move(sGet));

				TextFileLoader.SetParentNode();

				jShape.emplace(std::format("ShapeData{:02}", i), std::move(jSub));
			}
		}

		TextFileLoader.SetParentNode();

		m_JsonData.emplace("ShapeData", std::move(jShape));
	}

	if (TextFileLoader.SetChildNode("attachingdata"))
	{
		json jAttachData;
		
		DWORD dwDataCount;
		if (!TextFileLoader.GetTokenDoubleWord("attachingdatacount", &dwDataCount))
			return false;

		jAttachData["AttachingDataCount"] = dwDataCount;

		for (DWORD i = 0; i < dwDataCount; ++i)
		{
			if (!TextFileLoader.SetChildNode("attachingdata", i))
			{
				TextFileLoader.SetParentNode();
				continue;
			}

			json jSub;

			DWORD uAttachingDataType;
			if (!TextFileLoader.GetTokenDoubleWord("attachingdatatype", &uAttachingDataType))
				return false;

			jSub["AttachingDataType"] = uAttachingDataType;
			
			int32_t uIsAttaching;
			if (!TextFileLoader.GetTokenInteger("isattaching", &uIsAttaching))
				return false;

			jSub["isAttaching"] = uIsAttaching;
			
			DWORD uAttachingModelIndex;
			if (!TextFileLoader.GetTokenDoubleWord("attachingmodelindex", &uAttachingModelIndex))
				return false;

			jSub["AttachingModelIndex"] = uAttachingModelIndex;
			
			std::string sAttachingBoneName;
			if (!TextFileLoader.GetTokenString("attachingbonename", &sAttachingBoneName))
				return false;

			jSub.emplace("AttachingBoneName", std::move(sAttachingBoneName));
			
			DWORD uCollisionType;
			if (!TextFileLoader.GetTokenDoubleWord("collisiontype", &uCollisionType))
				return false;

			jSub["CollisionType"] = uCollisionType;
			
			DWORD uSphereDataCount;
			if (!TextFileLoader.GetTokenDoubleWord("spheredatacount", &uSphereDataCount))
				return false;

			jSub["CollisionType"] = uSphereDataCount;

			for (DWORD j = 0; j < uSphereDataCount; ++j)
			{
				if (!TextFileLoader.SetChildNode("spheredata", j))
				{
					TextFileLoader.SetParentNode();
					continue;
				}

				json jSubOfSub;

				float fRadius;
				if (!TextFileLoader.GetTokenFloat("radius", &fRadius))
					return false;

				jSubOfSub["Radius"] = fRadius;

				D3DXVECTOR3 v3Position;
				if (!TextFileLoader.GetTokenPosition("position", &v3Position))
					return false;

				jSubOfSub["Position"] = { v3Position.x, v3Position.y, v3Position.z };

				TextFileLoader.SetParentNode();

				jSub.emplace(std::format("SphereData{:02}", j), std::move(jSubOfSub));
			}

			TextFileLoader.SetParentNode();

			jAttachData.emplace(std::format("AttachingData{:02}", i), std::move(jSub));
		}

		TextFileLoader.SetParentNode();

		m_JsonData.emplace("AttachingData", std::move(jAttachData));
	}

	return true;
}
