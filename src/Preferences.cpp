#include <fstream>
#include "Preferences.h"
#include "json.hpp"

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
}