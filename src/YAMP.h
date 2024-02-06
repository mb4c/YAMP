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
#include "LibraryInfoModal.hpp"
#include <filesystem>
#include <Panels/StatusPanel.hpp>
#include <Panels/ArtistsPanel.hpp>

#include <Filter.hpp>
#include <Panels/AlbumPanel.hpp>
#include <Panels/TracksPanel.hpp>
#include <Panels/PlaylistsPanel.hpp>


class YAMP : public Application
{
private:
	void OnInit() override;
	void OnUpdate() override;
	void OnShutdown() override;

    Preferences m_Preferences;
    Player m_Player;

    bool m_ShowArtistPanel = true;
    bool m_ShowAlbumPanel = true;
    bool m_ShowTracksPanel = true;
    bool m_ShowPlaylistsPanel = true;
    bool m_ShowPreferencesPanel = false;
	bool m_ShouldFilterTracks = false;
    ImFont* m_Font = nullptr;
	std::string m_NewPlayListName;
    bool m_PlaylistClicked = false;
    Playlist m_SelectedPlaylist;
	Playlist m_Playlist;
	std::vector<std::string> m_FilteredAlbums;
	std::vector<Song> m_FilteredSongs;

	SearchProps m_SearchProps;

	Themes m_Themes;

	void Dockspace();

	StatusPanel m_StatusPanel{&m_Preferences};
	ArtistsPanel m_ArtistPanel{};
	AlbumPanel m_AlbumPanel;
	TracksPanel m_TracksPanel;
	PlaylistsPanel m_PlaylistsPanel;

	LibraryInfoModal m_LibraryInfoModal{"Library info"};

	void PreferencesPanel();

	void BuildFont();


public:
    YAMP(const std::string &title, int width, int height, bool vsync, int argc, char** argv);
	~YAMP();
};
