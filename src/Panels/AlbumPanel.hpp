#pragma once

#include <Player.h>
#include <AlbumInfoModal.hpp>


class AlbumPanel
{
public:
	void Init(bool& shouldFilterTracks, std::vector<std::string>& filteredAlbums, SearchProps& searchProps, const std::vector<Song>& songs);
	void RenderPanel(Player& player, std::vector<std::string>& filteredAlbums, const std::vector<Song>& filteredSongs, bool& playlistClicked, bool& shouldFilterTracks, SearchProps& searchProps);
	static void AddAlbumToPlaylist(Player& player, Playlist *playlist, const std::string& albumName);
	static std::vector<Song> GetSongsFromAlbumName(const std::string& name, Player& player);

	std::vector<Song> m_Songs;
	AlbumInfoModal m_AlbumInfo{"Album info"};
	std::string m_PlaylistName;
	static const ImGuiTableFlags m_PanelFlag =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;


};
