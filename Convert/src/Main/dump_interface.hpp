#pragma once

#include <iostream>
#include <fstream>
#include <format>
#include <memory>
#include <cstdint>
#include <chrono>

#include <filesystem>
namespace fs = std::filesystem;

// https://github.com/nlohmann/json (3.10.5)
#include <nlohmann/json.hpp>
using json = nlohmann::ordered_json;

// From M2
#include "../M2/text_file_loader.h"

constexpr auto OUTDIR = "out";
#define SHOW_ELAPSED_TIME

class DumpInterface
{
public:
	DumpInterface(const char* FileName, int32_t Indent) :
		m_FileName{ FileName },
		m_Indent{ Indent },
#if defined(SHOW_ELAPSED_TIME)
		m_TimeBegin{ std::chrono::steady_clock::now() },
#endif
		m_JsonData{}
	{
	}

	virtual ~DumpInterface() = default;
	virtual bool BuildJson() = 0;

	static void CreateOutDir()
	{
		if (!fs::is_directory(OUTDIR) || !fs::exists(OUTDIR))
			fs::create_directory(OUTDIR);
	}
	
	void Dump()
	{
		const std::string sFileName{ GetOutFileName() };
		std::ofstream file{ sFileName };
		if (!file.is_open())
		{
			printf("Cannot Open: <%s> For Writing...\n", sFileName.c_str());
			return;
		}

		try
		{
			file << m_JsonData.dump(m_Indent, ' ', false, json::error_handler_t::ignore);
		}
		catch (const std::exception& ex)
		{
			printf("[%s] <%s> Error: %s\n", typeid(this).name(), m_FileName.c_str(), ex.what());
			file.close();
			return;
		}
		
		file.close();

#if defined(SHOW_ELAPSED_TIME)
		const decltype(m_TimeBegin) TimeEnd{ std::chrono::steady_clock::now() };
		const auto ms{ std::chrono::duration_cast<std::chrono::milliseconds>(TimeEnd - m_TimeBegin).count() };
#endif

		std::cout
			<< "<"
			<< sFileName
			<< "> Finished."
#if defined(SHOW_ELAPSED_TIME)
			<< "("
			<< ms
			<< " ms)"
#endif
			<< std::endl;
	};

protected:
	const std::string& GetFileName() const
	{
		return m_FileName;
	}

	std::string GetOutFileName() const
	{
		std::string sOutFileName{ OUTDIR + std::string("/") };
		const auto pos{ m_FileName.find_last_of('.') };
		sOutFileName += (pos != std::string::npos) ? m_FileName.substr(0, pos) : m_FileName;
		sOutFileName += ".json";
		return sOutFileName;
	}
	
private:
	std::string m_FileName;
	int32_t m_Indent;

#if defined(SHOW_ELAPSED_TIME)
	std::chrono::steady_clock::time_point m_TimeBegin;
#endif

protected:
	json m_JsonData;
};