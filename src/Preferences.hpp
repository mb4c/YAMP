#pragma once

#include <string>
#include "Utils.hpp"

class Preferences
{
public:
    void Load();
    void Save();

    int m_WindowWidth = 1280;
    int m_WindowHeight = 720;
    float m_FontSize = 13;
    float m_Scale = 1;
	float m_Volume = 1;
    std::string m_LibraryPath = GetMusicFolderPath();
	std::string m_Theme = "Dark";
};
