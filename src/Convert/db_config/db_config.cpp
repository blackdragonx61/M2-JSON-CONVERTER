#include "db_config.hpp"

CConvertDBCONFIG::CConvertDBCONFIG(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertDBCONFIG::~CConvertDBCONFIG() = default;

void CConvertDBCONFIG::NextLine(FILE* fp)
{
	int c;

	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			return;
	}
}

bool CConvertDBCONFIG::GetWord(FILE* fp, char* tar)
{
	int i = 0;
	int c;

	int semicolon_mode = 0;

	while ((c = fgetc(fp)) != EOF)
	{
		if (c == 13)
			continue;

		if (semicolon_mode)
		{
			if (c == '"')
			{
				tar[i] = '\0';
				return true;
			}

			tar[i++] = c;
			continue;
		}
		else
		{
			if (i == 0)
			{
				if (c == '"')
				{
					semicolon_mode = 1;
					continue;
				}

				if (c == ' ' || c == '\t' || c == '\n')
				{
					continue;
				}
			}

			if ((c == ' ' || c == '\t' || c == '\n'))
			{
				tar[i] = '\0';
				return true;
			}

			tar[i] = c;
			++i;
		}
	}

	return (i != 0);
}

bool CConvertDBCONFIG::GetLine(FILE* fp, char* dest)
{
	int c;
	int i = 0;
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			return true;
		dest[i] = c;
		++i;
	}
	return true;
}

bool CConvertDBCONFIG::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	FILE* fp{ fopen(sFileName.c_str(), "rt") };

	if (fp == nullptr)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}

	char szTmp[256]{};
	char comment[256]{};

	std::uint8_t mode = 0;

	while (GetWord(fp, szTmp))
	{
		if (strcmp(szTmp, "//") == 0)
		{
			NextLine(fp);
			continue;
		}

		switch (mode)
		{
		case 0:
			strncpy(comment, szTmp, sizeof(comment));
			++mode;
			break;

		case 1:
			if (*szTmp == '=')
				++mode;
			break;

		case 2:
			mode = 0;
			m_valueMap.insert(std::make_pair(comment, szTmp));
			break;
		}

		if (mode == 2 && strcmp(comment, "ITEM_ID_RANGE") == 0)
		{
			GetLine(fp, szTmp);
			m_valueMap.insert(std::make_pair(comment, szTmp));
			mode = 0;
		}
	}

	fclose(fp);

	for (const auto& [k, v] : m_valueMap)
	{
		try
		{
			if (!k.compare("WELCOME_MSG"))
				m_JsonData["WELCOME_MSG"] = v;

			else if (!k.compare("SQL_ACCOUNT"))
				m_JsonData["SQL_ACCOUNT"] = v;

			else if (!k.compare("SQL_PLAYER"))
				m_JsonData["SQL_PLAYER"] = v;

			else if (!k.compare("SQL_COMMON"))
				m_JsonData["SQL_COMMON"] = v;

			else if (!k.compare("SQL_HOTBACKUP"))
				m_JsonData["SQL_HOTBACKUP"] = v;

			else if (!k.compare("NAME_COLUMN"))
				m_JsonData["NAME_COLUMN"] = v;

			else if (!k.compare("TABLE_POSTFIX"))
				m_JsonData["TABLE_POSTFIX"] = v;

			else if (!k.compare("LOCALE"))
				m_JsonData["LOCALE"] = v;

			else if (!k.compare("BIND_PORT"))
				m_JsonData["BIND_PORT"] = std::stoul(v);

			else if (!k.compare("BIND_IP"))
				m_JsonData["BIND_IP"] = std::stoul(v);

			else if (!k.compare("DB_SLEEP_MSEC"))
				m_JsonData["DB_SLEEP_MSEC"] = std::stoul(v);

			else if (!k.compare("CLIENT_HEART_FPS"))
				m_JsonData["CLIENT_HEART_FPS"] = std::stoul(v);

			else if (!k.compare("HASH_PLAYER_LIFE_SEC"))
				m_JsonData["HASH_PLAYER_LIFE_SEC"] = std::stoul(v);

			else if (!k.compare("BACKUP_LIMIT_SEC"))
				m_JsonData["BACKUP_LIMIT_SEC"] = std::stoul(v);

			else if (!k.compare("PLAYER_ID_START"))
				m_JsonData["PLAYER_ID_START"] = std::stoul(v);

			else if (!k.compare("PLAYER_DELETE_LEVEL_LIMIT"))
				m_JsonData["PLAYER_DELETE_LEVEL_LIMIT"] = std::stoul(v);

			else if (!k.compare("PLAYER_DELETE_LEVEL_LIMIT_LOWER"))
				m_JsonData["PLAYER_DELETE_LEVEL_LIMIT_LOWER"] = std::stoul(v);

			else if (!k.compare("PLAYER_DELETE_CHECK_SIMPLE"))
				m_JsonData["PLAYER_DELETE_CHECK_SIMPLE"] = std::stoul(v);

			else if (!k.compare("CHINA_EVENT_SERVER"))
				m_JsonData["CHINA_EVENT_SERVER"] = std::stoul(v);

			else if (!k.compare("POLY_POWER"))
				m_JsonData["POLY_POWER"] = std::stoul(v);

			else if (!k.compare("POLY_HANDICAP"))
				m_JsonData["POLY_HANDICAP"] = std::stoul(v);

			else if (!k.compare("TEST_SERVER"))
				m_JsonData["TEST_SERVER"] = std::stoul(v);

			else if (!k.compare("CACHE_FLUSH_LIMIT_PER_SECOND"))
				m_JsonData["CACHE_FLUSH_LIMIT_PER_SECOND"] = std::stoul(v);

			else if (!k.compare("ITEM_PRICELIST_CACHE_FLUSH_SECONDS"))
				m_JsonData["ITEM_PRICELIST_CACHE_FLUSH_SECONDS"] = std::stoul(v);

			else if (!k.compare("ITEM_CACHE_FLUSH_SECONDS"))
				m_JsonData["ITEM_CACHE_FLUSH_SECONDS"] = std::stoul(v);

			else if (!k.compare("PLAYER_CACHE_FLUSH_SECONDS"))
				m_JsonData["PLAYER_CACHE_FLUSH_SECONDS"] = std::stoul(v);

			else if (!k.compare("DISABLE_HOTBACKUP"))
				m_JsonData["DISABLE_HOTBACKUP"] = std::stoul(v);

			else if (!k.compare("LOG_KEEP_DAYS"))
				m_JsonData["LOG_KEEP_DAYS"] = std::stoul(v);

			else if (!k.compare("LOG"))
				m_JsonData["LOG"] = std::stoul(v);

			else if (!k.compare("ITEM_ID_RANGE"))
			{
				const auto pos = v.find(' ');
				m_JsonData["ITEM_ID_RANGE"] = { std::stoul(v.substr(0, pos)), std::stoul(v.substr(pos)) };
			}
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s>: %s (k:%s), (v:%s)\n", 
				typeid(this).name(), sFileName.c_str(), ex.what(), k.c_str(), v.c_str());
			return false;
		}
	}

	return true;
}