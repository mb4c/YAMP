#pragma once

#include <Player.h>

class Modals
{
public:
	static void OpenNewPlaylistModal(int index, bool open, bool isAlbum, Player& player, std::string& playlistName, const std::vector<Song>& filteredSongs);
};
