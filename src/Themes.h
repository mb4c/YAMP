#pragma once
#include <imgui.h>
#include <string>
#include <vector>

class ThemeData
{
public:

	std::string name;
	std::string author;
	ImGuiStyle style;
};

class Themes
{
public:
	static void DumpTheme(const std::string& file);
	static ThemeData LoadTheme(const std::string& file);
	void ScanThemes();
	void LoadThemeFromName(const std::string& name);

	std::vector<ThemeData> m_Themes;
};
