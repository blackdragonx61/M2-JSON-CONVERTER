#ifndef __INC_METIN_II_TEXTFILELOADER_H__
#define __INC_METIN_II_TEXTFILELOADER_H__

#include "file_loader.h"
#include "pool.h"
#include <map>

typedef struct D3DXVECTOR2
{
	float x, y;
} D3DXVECTOR2, * LPD3DXVECTOR2;

typedef struct D3DXVECTOR3
{
	float x, y, z;
} D3DXVECTOR3, * LPD3DXVECTOR3;

typedef struct D3DXVECTOR4
{
	float x, y, z, w;
} D3DXVECTOR4, * LPD3DXVECTOR4;

typedef struct D3DXQUATERNION
{
	float x, y, z, w;
} D3DXQUATERNION, * LPD3DXQUATERNION;

typedef struct D3DXCOLOR
{
	float r, g, b, a;
} D3DXCOLOR, * LPD3DXCOLOR;

typedef struct _D3DCOLORVALUE
{
	float r;
	float g;
	float b;
	float a;
} D3DCOLORVALUE;

typedef D3DXVECTOR3 D3DVECTOR;

typedef std::map<std::string, TTokenVector>	TTokenVectorMap;

class CTextFileLoader
{
public:
	typedef struct SGroupNode
	{
		std::string strGroupName;

		TTokenVectorMap LocalTokenVectorMap;

		SGroupNode* pParentNode;
		std::vector<SGroupNode*> ChildNodeVector;
	} TGroupNode;

	typedef std::vector<TGroupNode*> TGroupNodeVector;

public:
	static void DestroySystem();

public:
	CTextFileLoader();
	virtual ~CTextFileLoader();

	bool Load(const char* c_szFileName);
	const char* GetFileName();

	void SetTop();
	DWORD GetChildNodeCount();
	bool SetChildNode(const char* c_szKey);
	bool SetChildNode(const std::string& c_rstrKeyHead, DWORD dwIndex);
	bool SetChildNode(DWORD dwIndex);
	bool SetParentNode();
	bool GetCurrentNodeName(std::string* pstrName);

	bool IsToken(const std::string& c_rstrKey);
	bool GetTokenVector(const std::string& c_rstrKey, std::vector< std::string >** ppTokenVector);
	bool GetTokenBoolean(const std::string& c_rstrKey, bool* pData);
	bool GetTokenByte(const std::string& c_rstrKey, BYTE* pData);
	bool GetTokenWord(const std::string& c_rstrKey, WORD* pData);
	bool GetTokenInteger(const std::string& c_rstrKey, int* pData);
	bool GetTokenDoubleWord(const std::string& c_rstrKey, DWORD* pData);
	bool GetTokenFloat(const std::string& c_rstrKey, float* pData);

	bool GetTokenVector2(const std::string& c_rstrKey, D3DXVECTOR2* pVector2);
	bool GetTokenVector3(const std::string& c_rstrKey, D3DXVECTOR3* pVector3);
	bool GetTokenVector4(const std::string& c_rstrKey, D3DXVECTOR4* pVector4);

	bool GetTokenPosition(const std::string& c_rstrKey, D3DXVECTOR3* pVector);
	bool GetTokenQuaternion(const std::string& c_rstrKey, D3DXQUATERNION* pQ);
	bool GetTokenDirection(const std::string& c_rstrKey, D3DVECTOR* pVector);
	bool GetTokenColor(const std::string& c_rstrKey, D3DXCOLOR* pColor);
	bool GetTokenColor(const std::string& c_rstrKey, D3DCOLORVALUE* pColor);
	bool GetTokenString(const std::string& c_rstrKey, std::string* pString);

	CMemoryTextFileLoader& GetMemoryTextFileLoader();

protected:
	bool LoadGroup(TGroupNode* pGroupNode);

protected:
	std::string					m_strFileName;
	DWORD						m_dwcurLineIndex;
	const void* mc_pData;

	CMemoryTextFileLoader		m_fileLoader;

	TGroupNode					m_globalNode;
	TGroupNode* m_pcurNode;

private:
	static CDynamicPool<TGroupNode>	ms_groupNodePool;
};

#endif