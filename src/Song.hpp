#pragma once
#include <string>
#include <filesystem>

class Song
{
public:
	std::string title;
	std::string artist;
	std::string album;
	unsigned int year;
	unsigned int track;
	std::string genre;
	std::filesystem::path path;
	int duration;
    std::string uuid;
};
