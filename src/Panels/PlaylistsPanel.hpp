#pragma once


#include <Player.hpp>
#include <Compare.hpp>

class PlaylistsPanel
{
public:

	std::string m_PlaylistName;

	static const ImGuiTableFlags m_PanelFlag =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;

	void RenderPanel(Player& player, Playlist& selectedPlaylist, bool& playlistClicked, bool& shouldFilterTracks);
};
