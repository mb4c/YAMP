#pragma once

#include <string>
#include <csignal>
#include <pwd.h>
#include <unistd.h>

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