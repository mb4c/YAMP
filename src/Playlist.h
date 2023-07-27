#pragma once
#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "Song.h"

class Playlist
{
public:
	std::string name;
	std::vector<Song> songs;
	int duration = 0;

	inline static void AddSongToPlaylist(Playlist *playlist, int index, std::vector<Song> filteredSongs)
	{
		playlist->songs.push_back(filteredSongs.at(index));
		playlist->duration += filteredSongs.at(index).duration;
	}
};