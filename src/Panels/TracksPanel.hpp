#pragma once


#include <imgui.h>
#include <Compare.hpp>
#include "Modals.hpp"
#include <TrackInfoModal.hpp>

class TracksPanel
{
public:
	static const ImGuiTableFlags m_PanelFlag =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;
	std::string m_PlaylistName;
	ImGuiTableSortSpecs* m_SortsSpecs = nullptr;
	TrackInfoModal m_TrackInfoModal{"Track info"};

	void RenderPanel(Player& player, std::vector<Song>& filteredSongs, Playlist& playlist, Playlist& selectedPlaylist, const bool& playlistClicked, bool& shouldFilterTracks);
};
