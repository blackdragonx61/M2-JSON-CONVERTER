#include "text_file_loader.h"
#include <format>

CDynamicPool<CTextFileLoader::TGroupNode> CTextFileLoader::ms_groupNodePool;

static void stl_lowers(std::string& rstRet)
{
	for (size_t i = 0; i < rstRet.length(); ++i)
		rstRet[i] = std::tolower(rstRet[i]);
}

void CTextFileLoader::DestroySystem()
{
	ms_groupNodePool.Clear();
}

CTextFileLoader::CTextFileLoader()
	: m_dwcurLineIndex(0), mc_pData(nullptr)
{
	SetTop();

	m_globalNode.strGroupName = "global";
	m_globalNode.pParentNode = nullptr;
}

CTextFileLoader::~CTextFileLoader()
{
	DestroySystem();
}

const char* CTextFileLoader::GetFileName()
{
	return m_strFileName.c_str();
}

bool CTextFileLoader::Load(const char* c_szFileName)
{
	m_strFileName = c_szFileName;

	m_dwcurLineIndex = 0;

	FILE* fp = fopen(c_szFileName, "rb");

	if (nullptr == fp)
		return false;

	fseek(fp, 0L, SEEK_END);
	const size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	m_fileLoader.Bind(fileSize, pData);
	delete[] pData;

	LoadGroup(&m_globalNode);
	return true;
}

bool CTextFileLoader::LoadGroup(TGroupNode* pGroupNode)
{
	std::vector< std::string > stTokenVector;
	for (; m_dwcurLineIndex < m_fileLoader.GetLineCount(); ++m_dwcurLineIndex)
	{
		if (!m_fileLoader.SplitLine(m_dwcurLineIndex, &stTokenVector))
			continue;

		stl_lowers(stTokenVector[0]);

		if ('{' == stTokenVector[0][0])
			continue;

		if ('}' == stTokenVector[0][0])
			break;

		if (0 == stTokenVector[0].compare("group"))
		{
			if (2 != stTokenVector.size())
			{
				//auto _str = std::format("Invalid group syntax token size: {} != 2 (DO NOT SPACE IN NAME)", stTokenVector.size());
				//printf(_str.c_str());
				//for (size_t i = 0; i < stTokenVector.size(); ++i)
					//printf("  %u %s", i, stTokenVector[i].c_str());
				//exit(1);
				continue;
			}

			TGroupNode* pNewNode = ms_groupNodePool.Alloc();
			pNewNode->pParentNode = pGroupNode;
			pNewNode->strGroupName = stTokenVector[1];
			stl_lowers(pNewNode->strGroupName);
			pGroupNode->ChildNodeVector.push_back(pNewNode);

			++m_dwcurLineIndex;

			LoadGroup(pNewNode);
		}
		else if (0 == stTokenVector[0].compare("list"))
		{
			if (2 != stTokenVector.size())
			{
				assert(!"There is no list name!");
				continue;
			}

			std::vector< std::string > stSubTokenVector;

			stl_lowers(stTokenVector[1]);
			std::string key = stTokenVector[1];
			stTokenVector.clear();

			++m_dwcurLineIndex;
			for (; m_dwcurLineIndex < m_fileLoader.GetLineCount(); ++m_dwcurLineIndex)
			{
				if (!m_fileLoader.SplitLine(m_dwcurLineIndex, &stSubTokenVector))
					continue;

				if ('{' == stSubTokenVector[0][0])
					continue;

				if ('}' == stSubTokenVector[0][0])
					break;

				for (size_t j = 0; j < stSubTokenVector.size(); ++j)
				{
					stTokenVector.push_back(stSubTokenVector[j]);
				}
			}

			pGroupNode->LocalTokenVectorMap.insert(std::make_pair(key, stTokenVector));
		}
		else
		{
			std::string key = stTokenVector[0];

			if (1 == stTokenVector.size())
			{
				// printf("CTextFileLoader::LoadGroup : must have a value (filename: %s line: %d key: %s)",
				//	m_strFileName.c_str(),
				//	m_dwcurLineIndex,
				//	key.c_str());
				break;
			}

			stTokenVector.erase(stTokenVector.begin());
			pGroupNode->LocalTokenVectorMap.insert(std::make_pair(key, stTokenVector));
		}
	}

	return true;
}

void CTextFileLoader::SetTop()
{
	m_pcurNode = &m_globalNode;
}

DWORD CTextFileLoader::GetChildNodeCount()
{
	if (!m_pcurNode)
	{
		assert(!"Node to access has not set!");
		return 0;
	}

	return m_pcurNode->ChildNodeVector.size();
}

bool CTextFileLoader::SetChildNode(const char* c_szKey)
{
	if (!m_pcurNode)
	{
		assert(!"Node to access has not set!");
		return false;
	}

	for (size_t i = 0; i < m_pcurNode->ChildNodeVector.size(); ++i)
	{
		TGroupNode* pGroupNode = m_pcurNode->ChildNodeVector[i];
		if (0 == pGroupNode->strGroupName.compare(c_szKey))
		{
			m_pcurNode = pGroupNode;
			return true;
		}
	}

	return false;
}

bool CTextFileLoader::SetChildNode(const std::string& c_rstrKeyHead, DWORD dwIndex)
{
	char szKey[32];
	snprintf(szKey, sizeof(szKey), "%s%02u", c_rstrKeyHead.c_str(), dwIndex);
	return SetChildNode(szKey);
}

bool CTextFileLoader::SetChildNode(DWORD dwIndex)
{
	if (!m_pcurNode)
	{
		assert(!"Node to access has not set!");
		return false;
	}

	if (dwIndex >= m_pcurNode->ChildNodeVector.size())
	{
		assert(!"Node index to set is too large to access!");
		return false;
	}

	m_pcurNode = m_pcurNode->ChildNodeVector[dwIndex];

	return true;
}

bool CTextFileLoader::SetParentNode()
{
	if (!m_pcurNode)
	{
		assert(!"Node to access has not set!");
		return false;
	}

	if (nullptr == m_pcurNode->pParentNode)
	{
		return false;
	}

	m_pcurNode = m_pcurNode->pParentNode;

	return true;
}

bool CTextFileLoader::GetCurrentNodeName(std::string* pstrName)
{
	if (!m_pcurNode)
		return false;
	if (nullptr == m_pcurNode->pParentNode)
		return false;

	*pstrName = m_pcurNode->strGroupName;

	return true;
}

bool CTextFileLoader::IsToken(const std::string& c_rstrKey)
{
	if (!m_pcurNode)
	{
		assert(!"Node to access has not set!");
		return false;
	}

	return m_pcurNode->LocalTokenVectorMap.end() != m_pcurNode->LocalTokenVectorMap.find(c_rstrKey);
}

bool CTextFileLoader::GetTokenVector(const std::string& c_rstrKey, std::vector< std::string >** ppTokenVector)
{
	if (!m_pcurNode)
	{
		assert(!"Node to access has not set!");
		return false;
	}

	TTokenVectorMap::iterator it = m_pcurNode->LocalTokenVectorMap.find(c_rstrKey);
	if (m_pcurNode->LocalTokenVectorMap.end() == it)
	{
		// printf(" CTextFileLoader::GetTokenVector - Failed to find the key %s [%s :: %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*ppTokenVector = &it->second;

	return true;
}

bool CTextFileLoader::GetTokenBoolean(const std::string& c_rstrKey, bool* pData)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->empty())
	{
		// printf(" CTextFileLoader::GetTokenBoolean - Failed to find the value %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*pData = std::stoi(pTokenVector->at(0));

	return true;
}

bool CTextFileLoader::GetTokenByte(const std::string& c_rstrKey, BYTE* pData)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->empty())
	{
		// printf(" CTextFileLoader::GetTokenByte - Failed to find the value %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*pData = static_cast<BYTE>(std::stoi(pTokenVector->at(0)));

	return true;
}

bool CTextFileLoader::GetTokenWord(const std::string& c_rstrKey, WORD* pData)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->empty())
	{
		// printf(" CTextFileLoader::GetTokenWord - Failed to find the value %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*pData = static_cast<WORD>(std::stoi(pTokenVector->at(0)));

	return true;
}

bool CTextFileLoader::GetTokenInteger(const std::string& c_rstrKey, int* pData)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->empty())
	{
		// printf(" CTextFileLoader::GetTokenInteger - Failed to find the value %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*pData = std::stoi(pTokenVector->at(0));

	return true;
}

bool CTextFileLoader::GetTokenDoubleWord(const std::string& c_rstrKey, DWORD* pData)
{
	return GetTokenInteger(c_rstrKey, (int *) pData);
}

bool CTextFileLoader::GetTokenFloat(const std::string& c_rstrKey, float* pData)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->empty())
	{
		// printf(" CTextFileLoader::GetTokenFloat - Failed to find the value %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*pData = std::stof(pTokenVector->at(0));

	return true;
}

bool CTextFileLoader::GetTokenVector2(const std::string& c_rstrKey, D3DXVECTOR2* pVector2)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 2)
	{
		// printf(" CTextFileLoader::GetTokenVector2 - This key should have 2 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pVector2->x = std::stof(pTokenVector->at(0));
	pVector2->y = std::stof(pTokenVector->at(1));

	return true;
}

bool CTextFileLoader::GetTokenVector3(const std::string& c_rstrKey, D3DXVECTOR3* pVector3)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 3)
	{
		// printf(" CTextFileLoader::GetTokenVector3 - This key should have 3 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pVector3->x = std::stof(pTokenVector->at(0));
	pVector3->y = std::stof(pTokenVector->at(1));
	pVector3->z = std::stof(pTokenVector->at(2));

	return true;
}

bool CTextFileLoader::GetTokenVector4(const std::string& c_rstrKey, D3DXVECTOR4* pVector4)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 4)
	{
		// printf(" CTextFileLoader::GetTokenVector3 - This key should have 3 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pVector4->x = std::stof(pTokenVector->at(0));
	pVector4->y = std::stof(pTokenVector->at(1));
	pVector4->z = std::stof(pTokenVector->at(2));
	pVector4->w = std::stof(pTokenVector->at(3));

	return true;
}

bool CTextFileLoader::GetTokenPosition(const std::string& c_rstrKey, D3DXVECTOR3* pVector)
{
	return GetTokenVector3(c_rstrKey, pVector);
}

bool CTextFileLoader::GetTokenQuaternion(const std::string& c_rstrKey, D3DXQUATERNION* pQ)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 4)
	{
		// printf(" CTextFileLoader::GetTokenVector3 - This key should have 3 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pQ->x = std::stof(pTokenVector->at(0));
	pQ->y = std::stof(pTokenVector->at(1));
	pQ->z = std::stof(pTokenVector->at(2));
	pQ->w = std::stof(pTokenVector->at(3));

	return true;
}

bool CTextFileLoader::GetTokenDirection(const std::string& c_rstrKey, D3DVECTOR* pVector)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 3)
	{
		// printf(" CTextFileLoader::GetTokenDirection - This key should have 3 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pVector->x = std::stof(pTokenVector->at(0).c_str());
	pVector->y = std::stof(pTokenVector->at(1).c_str());
	pVector->z = std::stof(pTokenVector->at(2).c_str());

	return true;
}

bool CTextFileLoader::GetTokenColor(const std::string& c_rstrKey, D3DXCOLOR* pColor)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 4)
	{
		// printf(" CTextFileLoader::GetTokenColor - This key should have 4 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pColor->r = std::stof(pTokenVector->at(0));
	pColor->g = std::stof(pTokenVector->at(1));
	pColor->b = std::stof(pTokenVector->at(2));
	pColor->a = std::stof(pTokenVector->at(3));

	return true;
}

bool CTextFileLoader::GetTokenColor(const std::string& c_rstrKey, D3DCOLORVALUE* pColor)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->size() != 4)
	{
		// printf(" CTextFileLoader::GetTokenColor - This key should have 4 values %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	pColor->r = std::stof(pTokenVector->at(0));
	pColor->g = std::stof(pTokenVector->at(1));
	pColor->b = std::stof(pTokenVector->at(2));
	pColor->a = std::stof(pTokenVector->at(3));;

	return true;
}

bool CTextFileLoader::GetTokenString(const std::string& c_rstrKey, std::string* pString)
{
	std::vector< std::string >* pTokenVector;
	if (!GetTokenVector(c_rstrKey, &pTokenVector))
		return false;

	if (pTokenVector->empty())
	{
		// printf(" CTextFileLoader::GetTokenString - Failed to find the value %s [%s : %s]", m_strFileName.c_str(), m_pcurNode->strGroupName.c_str(), c_rstrKey.c_str());
		return false;
	}

	*pString = pTokenVector->at(0);

	return true;
}

CMemoryTextFileLoader& CTextFileLoader::GetMemoryTextFileLoader()
{
	return m_fileLoader;
}

