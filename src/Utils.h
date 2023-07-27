#pragma once
#include <regex>
#include <string>
#include <csignal>
#include <pwd.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <imgui.h>

inline std::string GetMusicFolderPath()
{
    std::string path;
    if ((path = getenv("HOME")).empty())
    {
        path = getpwuid(getuid())->pw_dir;
    }

    return path + "/Music";
}

inline std::string SecondsToTime(int seconds)
{
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    std::string timeStr;

    if (h > 0)
	{
        timeStr = std::to_string(h) + ":";
    }

    if (m < 10 && h > 0)
	{
        timeStr += "0";
    }

    timeStr += std::to_string(m) + ":";

    if (s < 10)
	{
        timeStr += "0";
    }

    timeStr += std::to_string(s);
    return timeStr;
}

inline std::string SecondsToTimeHMS(int seconds, bool longFormat = false)
{
	int h = seconds / 3600;
	int m = (seconds % 3600) / 60;
	int s = seconds % 60;

	std::string timeStr;

	if (h > 0)
	{
		if (longFormat)
			timeStr += std::to_string(h) + " hours ";
		else
			timeStr += std::to_string(h) + "h ";
	}

	if (m < 10 && h > 0)
	{
		timeStr += "0";
	}

	if (longFormat)
		timeStr += std::to_string(m) + " minutes ";
	else
		timeStr += std::to_string(m) + "m ";

	if (s < 10)
	{
		timeStr += "0";
	}

	if (longFormat)
		timeStr += std::to_string(s) + " seconds ";
	else
		timeStr += std::to_string(s) + "s ";

	return timeStr;
}

inline void OpenInFileExplorer(std::filesystem::path path)
{
#ifdef __unix__
	std::string command = "xdg-open \"" + path.parent_path().string() + "\"";
	system(command.c_str());
#endif
}

inline void TextCentered(const std::string &text)
{
	float win_width = ImGui::GetWindowSize().x;
	float text_width = ImGui::CalcTextSize(text.c_str()).x;

	// calculate the indentation that centers the text on one line, relative
	// to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
	float text_indentation = (win_width - text_width) * 0.5f;

	// if text is too long to be drawn on one line, `text_indentation` can
	// become too small or even negative, so we check a minimum indentation
	float min_indentation = 20.0f;
	if (text_indentation <= min_indentation) {
		text_indentation = min_indentation;
	}

	ImGui::SameLine(text_indentation);
	ImGui::PushTextWrapPos(win_width - text_indentation);
	ImGui::TextWrapped("%s", text.c_str());
	ImGui::PopTextWrapPos();
}

inline std::vector<std::string> Search(std::string searchText, const std::vector<std::string>& strings)
{
	std::vector<std::string> filteredStrings;
	std::transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);

	for (const auto& str : strings)
	{
		std::string str_copy = str;
		std::transform(str_copy.begin(), str_copy.end(), str_copy.begin(), ::tolower);
		if (str_copy.find(searchText) != std::string::npos)
		{
			filteredStrings.push_back(str);
		}
	}
	return filteredStrings;
}

struct SearchProps
{
	std::string artistSearchbar;
	std::string albumSearchbar;
	std::vector<std::string> searchArtists;
	std::vector<std::string> searchAlbums;
};

template<typename T>
void Reorder(std::vector<T> &vec, int index)
{
	T type = vec[index];
	if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
	{
		int n_next = index + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		if (n_next >= 0 && n_next < vec.size())
		{
			vec[index] = vec[n_next];
			vec[n_next] = type;
			ImGui::ResetMouseDragDelta();
		}
	}
}