#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <nlohmann/json.hpp>
#include "Song.hpp"
#include "Playlist.hpp"
#include "Log.hpp"

#include "Utils.hpp"
#include "imgui_notify.h"


class Library
{
public:
    void Scan();
	void Save();
	void Load();
	void GenerateAlbums();
	void CalculateTotalSongTime();

    std::string m_LibraryPath;
	std::vector<Song> m_Songs;

    // Filtering
    std::vector<std::string> m_Artists;
    std::vector<std::string> m_Albums;

    std::string m_SelectedArtist = "All";
    std::string m_SelectedAlbum = "All";

    std::vector<Playlist> m_Playlists;
	int m_TotalSongTime = 0;

	void SavePlaylists();
	void LoadPlaylists();
	Song UUID2Song(const std::string& id);

	static bool GetMetadataFromTitle(const std::string& filename, Song& song);
	static bool GetMetadataFromInternet(const std::string& filename, Song& song);
	static bool GetMetadataFromRegex(const std::string& filename, Song& song);
};
