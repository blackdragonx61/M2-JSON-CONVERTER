#include "game_config.hpp"

CConvertGAMECONFIG::CConvertGAMECONFIG(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertGAMECONFIG::~CConvertGAMECONFIG() = default;

namespace 
{
#define LOWER(c) (((c)>='A'  && (c) <= 'Z') ? ((c)+('a'-'A')) : (c))
#define ishan(ch) (((ch) & 0xE0) > 0x90)
#define isnhdigit(ch) (!ishan(ch) && isdigit(ch))
#define isnhspace(ch) (!ishan(ch) && isspace(ch))

	bool is_string_true(const char* string)
	{
		if (isnhdigit(*string))
		{
			int result = std::stoi(string);
			return result > 0 ? true : false;
		}
		else if (LOWER(*string) == 't')
			return true;
		
		return false;
	}
	
	void parse_token(char* src, char* token, char* value)
	{
		char* tmp;

		for (tmp = src; *tmp && *tmp != ':'; tmp++)
		{
			if (isnhspace(*tmp))
				continue;

			*(token++) = LOWER(*tmp);
		}

		*token = '\0';

		for (tmp += 2; *tmp; tmp++)
		{
			if (*tmp == '\n' || *tmp == '\r')
				continue;

			*(value++) = *tmp;
		}

		*value = '\0';
	}
}

bool CConvertGAMECONFIG::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	FILE* fp{ fopen(sFileName.c_str(), "rt") };

	if (fp == nullptr)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(*this).name(), sFileName.c_str());
		return false;
	}

	char buf[256]{};
	char token_string[256]{};
	char value_string[256]{};

	while (fgets(buf, sizeof(buf), fp))
	{
		parse_token(buf, token_string, value_string);

		try
		{
			if (!_strcmpi(token_string, "BLOCK_LOGIN"))
				m_JsonData["BLOCK_LOGIN"] = value_string;

			else if (!_strcmpi(token_string, "adminpage_ip"))
				m_JsonData["adminpage_ip"] = value_string;

			else if (!_strcmpi(token_string, "adminpage_ip1"))
				m_JsonData["adminpage_ip1"] = value_string;

			else if (!_strcmpi(token_string, "adminpage_ip2"))
				m_JsonData["adminpage_ip2"] = value_string;

			else if (!_strcmpi(token_string, "adminpage_ip3"))
				m_JsonData["adminpage_ip3"] = value_string;

			else if (!_strcmpi(token_string, "adminpage_password"))
				m_JsonData["adminpage_password"] = value_string;

			else if (!_strcmpi(token_string, "hostname"))
				m_JsonData["hostname"] = value_string;

			else if (!_strcmpi(token_string, "channel"))
				m_JsonData["channel"] = std::stoi(value_string);
			
			else if (!_strcmpi(token_string, "player_sql"))
				m_JsonData["player_sql"] = value_string;

			else if (!_strcmpi(token_string, "common_sql"))
				m_JsonData["common_sql"] = value_string;

			else if (!_strcmpi(token_string, "log_sql"))
				m_JsonData["log_sql"] = value_string;

			else if (!_strcmpi(token_string, "LOCALE_SERVICE"))
				m_JsonData["LOCALE_SERVICE"] = value_string;

			else if (!_strcmpi(token_string, "WEB_AUTH"))
				m_JsonData["WEB_AUTH"] = value_string;

			else if (!_strcmpi(token_string, "empire_whisper"))
				m_JsonData["empire_whisper"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "mark_server"))
				m_JsonData["mark_server"] = is_string_true(value_string);

			else if (!_strcmpi(token_string, "mark_min_level"))
				m_JsonData["mark_min_level"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "port"))
				m_JsonData["port"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "log_keep_days"))
				m_JsonData["log_keep_days"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "passes_per_sec"))
				m_JsonData["passes_per_sec"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "p2p_port"))
				m_JsonData["p2p_port"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "db_port"))
				m_JsonData["db_port"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "save_event_second_cycle"))
				m_JsonData["save_event_second_cycle"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "ping_event_second_cycle"))
				m_JsonData["ping_event_second_cycle"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "table_postfix"))
				m_JsonData["table_postfix"] = value_string;

			else if (!_strcmpi(token_string, "db_addr"))
				m_JsonData["db_addr"] = value_string;

			else if (!_strcmpi(token_string, "test_server"))
				m_JsonData["test_server"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "speed_server"))
				m_JsonData["speed_server"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "auction_server"))
				m_JsonData["auction_server"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "distribution_test_server"))
				m_JsonData["distribution_test_server"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "china_event_server"))
				m_JsonData["china_event_server"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "map_allow"))
			{
				char* p = value_string;
				std::string stNum;

				for (; *p; p++)
				{
					if (isnhspace(*p))
					{
						if (stNum.length()) {
							m_JsonData["map_allow"].push_back(std::stoi(stNum));
							stNum.clear();
						}
					}
					else
						stNum += *p;
				}

				if (stNum.length())
					m_JsonData["map_allow"].push_back(std::stoi(stNum));
			}

			else if (!_strcmpi(token_string, "user_limit"))
				m_JsonData["user_limit"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "skill_disable"))
				m_JsonData["skill_disable"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "auth_server"))
				m_JsonData["auth_server"] = value_string;

			else if (!_strcmpi(token_string, "quest_dir"))
				m_JsonData["quest_dir"] = value_string;

			else if (!_strcmpi(token_string, "quest_object_dir"))
				m_JsonData["quest_object_dir"] = value_string;

			else if (!_strcmpi(token_string, "teen_addr"))
				m_JsonData["teen_addr"] = value_string;

			else if (!_strcmpi(token_string, "teen_port"))
				m_JsonData["teen_port"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "synchack_limit_count"))
				m_JsonData["synchack_limit_count"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "speedhack_limit_count"))
				m_JsonData["speedhack_limit_count"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "speedhack_limit_bonus"))
				m_JsonData["speedhack_limit_bonus"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "server_id"))
				m_JsonData["server_id"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "mall_url"))
				m_JsonData["mall_url"] = value_string;

			else if (!_strcmpi(token_string, "bind_ip"))
				m_JsonData["bind_ip"] = value_string;

			else if (!_strcmpi(token_string, "view_range"))
				m_JsonData["view_range"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "spam_block_duration"))
				m_JsonData["spam_block_duration"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "spam_block_score"))
				m_JsonData["spam_block_score"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "spam_block_reload_cycle"))
				m_JsonData["spam_block_reload_cycle"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "check_multihack"))
				m_JsonData["check_multihack"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "spam_block_max_level"))
				m_JsonData["spam_block_max_level"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "protect_normal_player"))
				m_JsonData["protect_normal_player"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "notice_battle_zone"))
				m_JsonData["notice_battle_zone"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "xtrap_enable"))
				m_JsonData["xtrap_enable"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "pk_protect_level"))
				m_JsonData["pk_protect_level"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "max_level"))
				m_JsonData["max_level"] = std::stoi(value_string);

			else if (!_strcmpi(token_string, "block_char_creation"))
				m_JsonData["block_char_creation"] = std::stoi(value_string);
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s>: %s (token_string:%s), (value_string:%s)\n", 
				typeid(*this).name(), sFileName.c_str(), ex.what(), token_string, value_string);
			fclose(fp);
			return false;
		}
	}

	fclose(fp);
	return true;
}