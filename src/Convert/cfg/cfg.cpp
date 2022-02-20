#include "cfg.hpp"

CConvertCFG::CConvertCFG(const char* FileName, int32_t Indent)
	: DumpInterface{ FileName, Indent }
{
}

CConvertCFG::~CConvertCFG() = default;

bool CConvertCFG::BuildJson() /*override*/
{
	const std::string& sFileName{ GetFileName() };
	FILE* fp{ fopen(sFileName.c_str(), "rt") };

	if (fp == nullptr)
	{
		printf("[%s] Cannot Open: <%s>\n", typeid(this).name(), sFileName.c_str());
		return false;
	}

	char buf[256]{};
	char command[256]{};
	char value[256]{};

	while (fgets(buf, sizeof(buf), fp))
	{
		if (sscanf(buf, " %s %s\n", command, value) == EOF)
			break;

		try
		{
			if (!_strcmpi(command, "WIDTH"))
				m_JsonData["WIDTH"] = std::stoi(value);

			else if (!_strcmpi(command, "HEIGHT"))
				m_JsonData["HEIGHT"] = std::stoi(value);

			else if (!_strcmpi(command, "BPP"))
				m_JsonData["BPP"] = std::stoi(value);

			else if (!_strcmpi(command, "FREQUENCY"))
				m_JsonData["FREQUENCY"] = std::stoi(value);

			else if (!_strcmpi(command, "SOFTWARE_CURSOR"))
				m_JsonData["SOFTWARE_CURSOR"] = std::stoi(value);

			else if (!_strcmpi(command, "OBJECT_CULLING"))
				m_JsonData["OBJECT_CULLING"] = std::stoi(value);

			else if (!_strcmpi(command, "VISIBILITY"))
				m_JsonData["VISIBILITY"] = std::stoi(value);

			else if (!_strcmpi(command, "MUSIC_VOLUME")) {
				float MUSIC_VOLUME{};

				if (strchr(value, '.') == 0) { // Old compatiability
					MUSIC_VOLUME = pow(10.0f, (-1.0f + (std::stof(value) / 5.0f)));
					if (std::stoi(value) == 0)
						MUSIC_VOLUME = {};
				}
				else
					MUSIC_VOLUME = std::stof(value);

				m_JsonData["MUSIC_VOLUME"] = MUSIC_VOLUME;
			}

			else if (!_strcmpi(command, "VOICE_VOLUME"))
				m_JsonData["VOICE_VOLUME"] = std::stoi(value);

			else if (!_strcmpi(command, "GAMMA"))
				m_JsonData["GAMMA"] = std::stoi(value);

			else if (!_strcmpi(command, "IS_SAVE_ID"))
				m_JsonData["IS_SAVE_ID"] = std::stoi(value);

			else if (!_strcmpi(command, "SAVE_ID")) {
				char saveid[20];
				strncpy(saveid, value, sizeof(saveid));
				m_JsonData["SAVE_ID"] = saveid;
			}

			else if (!_strcmpi(command, "PRE_LOADING_DELAY_TIME"))
				m_JsonData["PRE_LOADING_DELAY_TIME"] = std::stoi(value);

			else if (!_strcmpi(command, "WINDOWED"))
				m_JsonData["WINDOWED"] = std::stoi(value);

			else if (!_strcmpi(command, "USE_DEFAULT_IME"))
				m_JsonData["USE_DEFAULT_IME"] = std::stoi(value);

			else if (!_strcmpi(command, "SOFTWARE_TILING"))
				m_JsonData["SOFTWARE_TILING"] = std::stoi(value);

			else if (!_strcmpi(command, "SHADOW_LEVEL"))
				m_JsonData["SHADOW_LEVEL"] = std::stoi(value);

			else if (!_strcmpi(command, "DECOMPRESSED_TEXTURE"))
				m_JsonData["DECOMPRESSED_TEXTURE"] = std::stoi(value);
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s>: %s (command:%s), (value:%s)\n", 
				typeid(this).name(), sFileName.c_str(), ex.what(), command, value);
			fclose(fp);
			return false;
		}
	}

	fclose(fp);
	return true;
}