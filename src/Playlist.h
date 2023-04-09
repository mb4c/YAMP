#pragma once
#include <string>
#include <vector>
#include "Song.h"

class Playlist
{
public:
	std::string name;
	std::vector<Song> songs;
	int duration = 0;
};