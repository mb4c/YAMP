#pragma once
#include <regex>
#include <string>
#include <csignal>
#include <pwd.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>

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

// Extracts artist, title, and track from a song title.
// Expected format of the title string is "Number. Artist - Title".
// Returns false if fails.
inline bool GetMetadataFromTitle(std::string filename, unsigned int* track, std::string* artist, std::string* title)
{
	std::regex trackRegex("(\\d+)\\.? ?(.*) - (.*)");
	std::smatch match;
	if (std::regex_search(filename, match, trackRegex))
		*track = std::stoi(match[1]);
	else
	{
		std::cout << "Failed to get metadata from title:  " << filename << std::endl;
		return false;

	}

	*artist = match[2];
	*title = match[3];
	return true;
}