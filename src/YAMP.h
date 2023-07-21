#pragma once
#include <string>
#include "Player.h"
#include "Library.h"
#include "IconsFontAwesome6.h"
#include "Preferences.h"
#include "Themes.h"
#include <imgui_stdlib.h>
#include <thread>
#include <imgui_internal.h>
#include <Application.h>
#include "portable-file-dialogs.h"
#include <filesystem>


class YAMP : public Application
{
private:

	void OnInit() override;
	void OnUpdate() override;
	void OnShutdown() override;

    Preferences m_Preferences;
    Player m_Player;
    bool m_Holding = false;

    bool m_ShowArtistPanel = true;
    bool m_ShowAlbumPanel = true;
    bool m_ShowTracksPanel = true;
    bool m_ShowPlaylistsPanel = true;
    bool m_ShowPreferencesPanel = false;
    ImFont* m_Font = nullptr;
    char m_NewPlayListName[256] = "";
    bool m_PlaylistClicked = false;
    Playlist* m_SelectedPlaylist = nullptr;
    bool m_Repeat = false;
	Playlist m_Playlist;
	std::vector<std::string> m_FilteredAlbums;
	std::vector<Song> m_FilteredSongs;
	std::string m_ArtistSearchbar;
	std::string m_AlbumSearchbar;
	std::vector<std::string> m_SearchArtists;
	std::vector<std::string> m_SearchAlbums;
	static const ImGuiTableFlags m_PanelFlag =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;

	Themes m_Themes;

	void Dockspace();
	void StatusPanel();
	void ArtistsPanel();
	void AlbumPanel();
	void TracksPanel();
	void PreferencesPanel();
	void PlaylistsPanel();
	void FilterAlbum();
	void FilterTracks();

	std::vector<Song> GetSongsFromAlbumName(const std::string& name);
	void OpenNewPlaylistModal(int index, bool open, bool isAlbum);
	void AddSongToPlaylist(Playlist* playlist, int index);
	void AddAlbumToPlaylist(Playlist* playlist, const std::string& albumName);
	static std::vector<std::string> Search(std::string searchText, const std::vector<std::string>& strings);
	template<typename T> void Reorder(std::vector<T>& vec, int index);

	static int ComparePlaylist(const Playlist& lhs, const Playlist& rhs);
	static bool CompareString(const std::string& lhs, const std::string& rhs, bool descending = false);
	static bool CompareSong(const Song& lhs, const Song& rhs, ImGuiTableSortSpecs* specs);

	static void TextCentered(const std::string& text);

	void BuildFont();

	enum ColumnID
	{
		ColumnID_Track,
		ColumnID_Name,
		ColumnID_Artist,
		ColumnID_Album,
		ColumnID_Genre,
		ColumnID_Year
	};



public:
    YAMP(const std::string &title, int width, int height, bool vsync);
};
