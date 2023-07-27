#pragma once


#include "Player.h"

class Filter
{
public:
	static void FilterAlbum(Player& player, std::vector<std::string>& filteredAlbums, std::vector<std::string>& searchAlbums, const std::string& albumSearchbar);
	static void FilterTracks(Player &player, Playlist &playlist, const Playlist& selectedPlaylist, std::vector<Song> &filteredSongs, const bool &playlistClicked);
};
