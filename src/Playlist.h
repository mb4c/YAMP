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
	inline void AddSong(Song song)
	{
		songs.push_back(song);
		duration += song.duration;
	}

	inline static Playlist GetPlaylistFromAlbumName(const std::string& album, std::vector<Song> songs)
	{
		Playlist playlist;
		playlist.name = album;

		for (int i = 0; i < songs.size(); ++i)
		{
			if (songs[i].album == album)
			{
				playlist.AddSong(songs[i]);
			}
		}

		return playlist;
	}
};