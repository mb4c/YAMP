#pragma once
#include <string>

class Song
{
public:
	std::string title;
	std::string artist;
	std::string album;
	unsigned int year;
	unsigned int track;
	std::string genre;
	std::string path;
	int duration;
    std::string uuid;
};
