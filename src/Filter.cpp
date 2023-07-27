#include "Filter.hpp"

void Filter::FilterAlbum(Player& player, std::vector<std::string>& filteredAlbums, std::vector<std::string>& searchAlbums, const std::string& albumSearchbar)
{
	filteredAlbums.clear();

	for (auto & song : player.m_Library.m_Songs)
	{
		if (song.artist == player.m_Library.m_SelectedArtist || player.m_Library.m_SelectedArtist == "All")
		{
			if (std::find(filteredAlbums.begin(), filteredAlbums.end(), song.album) == filteredAlbums.end())
			{
				filteredAlbums.push_back(song.album);
			}
		}
	}

	searchAlbums = Search(albumSearchbar, filteredAlbums);
}

void
Filter::FilterTracks(Player& player, Playlist& playlist, const Playlist& selectedPlaylist, std::vector<Song>& filteredSongs, const bool& playlistClicked)
{
	playlist.name = "Queue";
	playlist.songs.clear();
	filteredSongs.clear();

	if (!playlistClicked)
	{
		for (auto & song : player.m_Library.m_Songs)
		{
			if (song.album == player.m_Library.m_SelectedAlbum || player.m_Library.m_SelectedAlbum == "All")
			{
				filteredSongs.push_back(song);
				playlist.songs.push_back(song);
			}
		}
	}
	else
	{
		filteredSongs = selectedPlaylist.songs;
		playlist = selectedPlaylist;
	}
}
