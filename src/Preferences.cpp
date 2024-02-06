#include <fstream>
#include "Preferences.h"
#include <nlohmann/json.hpp>
#include "Log.h"
using namespace nlohmann;

void Preferences::Save()
{
    json json;
    json["WindowWidth"] = m_WindowWidth;
    json["WindowHeight"] = m_WindowHeight;
    json["Scale"] = m_Scale;
    json["FontSize"] = m_FontSize;
    json["LibraryPath"] = m_LibraryPath;
    json["Volume"] = m_Volume;
	json["Theme"] = m_Theme;

    std::ofstream o("res/preferences.json");
    o << json;
    o.close();

	Log::Trace("Preferences saved");
}

void Preferences::Load()
{
    if (!std::filesystem::exists("res/preferences.json"))
    {
        Save();
    }

    std::ifstream i("res/preferences.json");
    json json;
    i >> json;

    m_WindowWidth = json["WindowWidth"];
    m_WindowHeight = json["WindowHeight"];
    m_Scale = json["Scale"];
    m_FontSize = json["FontSize"];
    m_LibraryPath = json["LibraryPath"];
	m_Volume = json["Volume"];
	m_Theme = json["Theme"];

	Log::Trace("Preferences loaded");
}