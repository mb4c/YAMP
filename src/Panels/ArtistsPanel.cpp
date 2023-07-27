#include <imgui.h>
#include <imgui_stdlib.h>
#include <algorithm>
#include <string>
#include <Compare.hpp>
#include <Filter.hpp>
#include "ArtistsPanel.hpp"

void ArtistsPanel::RenderPanel(Player& player, std::vector<std::string>& filteredAlbums, SearchProps& searchProps)
{
	ImGui::Begin("Artist");

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2);
	if(ImGui::InputTextWithHint("##artistsearchbar", "Search...", &searchProps.artistSearchbar))
		searchProps.searchArtists = Search(searchProps.artistSearchbar, player.m_Library.m_Artists);

	if(ImGui::BeginTable("Artist", 1, m_PanelFlag))
	{
		ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_DefaultSort);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (searchProps.searchArtists.size() > 1)
					std::sort(searchProps.searchArtists.begin(), searchProps.searchArtists.end(), [&sorts_specs](const std::string& lhs, const std::string& rhs){
						if (sorts_specs->Specs == nullptr) {
							return false;
						}
						return Compare::CompareString(lhs, rhs, sorts_specs->Specs->SortDirection == ImGuiSortDirection_Descending);
					});

				sorts_specs->SpecsDirty = false;
			}

		ImGui::PushID("All");
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Selectable("All", "All" == player.m_Library.m_SelectedArtist, ImGuiSelectableFlags_SpanAllColumns);
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			player.m_Library.m_SelectedArtist = "All";
			Filter::FilterAlbum(player, filteredAlbums, searchProps.searchAlbums, searchProps.albumSearchbar);
		}
		ImGui::PopID();

		ImGuiListClipper clipper;
		clipper.Begin(searchProps.searchArtists.size());
		while (clipper.Step())
		{
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				std::string artist = searchProps.searchArtists[row_n];
				ImGui::PushID(artist.c_str());
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				bool selected = artist == player.m_Library.m_SelectedArtist;
				ImGui::Selectable(artist.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					player.m_Library.m_SelectedArtist = artist;
					Filter::FilterAlbum(player, filteredAlbums, searchProps.searchAlbums, searchProps.albumSearchbar);
				}
				ImGui::PopID();
			}
		}
		ImGui::EndTable();
	}

	ImGui::End();
}

void ArtistsPanel::Init(Player& player, std::vector<std::string>& filteredAlbums, SearchProps& searchProps)
{
	searchProps.searchArtists = Search(searchProps.artistSearchbar, player.m_Library.m_Artists);
	Filter::FilterAlbum(player, filteredAlbums, searchProps.searchAlbums, searchProps.albumSearchbar);
}

