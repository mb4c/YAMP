#pragma once

#include <Player.hpp>

class Modals
{
public:
	static void OpenNewPlaylistModal(int index, bool open, bool isAlbum, Player& player, std::string& playlistName, const std::vector<Song>& filteredSongs);
};
