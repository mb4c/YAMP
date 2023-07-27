#pragma once

#include <Player.h>


class AlbumPanel
{
public:
	void Init(bool& shouldFilterTracks, std::vector<std::string>& filteredAlbums, SearchProps& searchProps);
	void RenderPanel(Player& player, std::vector<std::string>& filteredAlbums, const std::vector<Song>& filteredSongs, bool& playlistClicked, bool& shouldFilterTracks, SearchProps& searchProps);
	static void AddAlbumToPlaylist(Player& player, Playlist *playlist, const std::string& albumName);
	static std::vector<Song> GetSongsFromAlbumName(const std::string& name, Player& player);

	std::string m_PlaylistName;
	static const ImGuiTableFlags m_PanelFlag =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;


};
