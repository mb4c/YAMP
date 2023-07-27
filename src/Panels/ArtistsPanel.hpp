#pragma once


#include <vector>
#include <Player.h>

class ArtistsPanel
{
public:
	void Init(Player& player, std::vector<std::string>& filteredAlbums, SearchProps& searchProps);
	void RenderPanel(Player& player, std::vector<std::string>& filteredAlbums, SearchProps& searchProps);

	static const ImGuiTableFlags m_PanelFlag =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;

};
