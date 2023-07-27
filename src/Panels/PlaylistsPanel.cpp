#include "PlaylistsPanel.hpp"

void
PlaylistsPanel::RenderPanel(Player& player, Playlist& selectedPlaylist, bool& playlistClicked, bool& shouldFilterTracks)
{
	ImGui::Begin("Playlists");

	if(ImGui::BeginTable("Playlist", 1, m_PanelFlag))
	{
		ImGui::TableSetupColumn("Playlist", ImGuiTableColumnFlags_DefaultSort);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (player.m_Library.m_Playlists.size() > 1)
					std::sort(player.m_Library.m_Playlists.begin(),player.m_Library.m_Playlists.end(), Compare::ComparePlaylist);
				sorts_specs->SpecsDirty = false;
			}

		bool playlistRenamePopup = false;
		bool playlistInfoPopup = false;

		ImGuiListClipper clipper;
		clipper.Begin(player.m_Library.m_Playlists.size());
		while (clipper.Step())
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				Playlist playlist = player.m_Library.m_Playlists[row_n];
				ImGui::PushID(playlist.name.c_str());
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				bool selected = false;
				ImGui::Selectable(playlist.name.c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					selectedPlaylist = player.m_Library.m_Playlists[row_n];
					playlistClicked = true;
					shouldFilterTracks = true;
				}

				Reorder(player.m_Library.m_Playlists, row_n);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("playlistpopup");
				}

				if (ImGui::BeginPopup("playlistpopup"))
				{
					if(ImGui::Selectable("Remove playlist"))
					{
						if (selectedPlaylist.name == player.m_Library.m_Playlists.at(row_n).name)
						{
							playlistClicked = false;
						}

						player.m_Library.m_Playlists.erase(player.m_Library.m_Playlists.begin() + row_n);
						player.m_Library.SavePlaylists();

					}

					if(ImGui::Selectable("Rename"))
					{
						playlistRenamePopup = true;
						m_PlaylistName = player.m_Library.m_Playlists.at(row_n).name;
					}

					if(ImGui::Selectable("Info"))
					{
						playlistInfoPopup = true;
					}

					ImGui::EndPopup();
				}

				if (playlistRenamePopup)
				{
					ImGui::OpenPopup("Rename playlist");
					playlistRenamePopup = false;
				}
				if (playlistInfoPopup)
				{
					ImGui::OpenPopup("Info");
					playlistInfoPopup = false;
				}

				if (ImGui::BeginPopupModal("Rename playlist",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::InputText("Name", &m_PlaylistName);

					if (ImGui::Button("Rename"))
					{
						player.m_Library.m_Playlists.at(row_n).name = m_PlaylistName;
						player.m_Library.SavePlaylists();
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Info",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Playlist name: %s", playlist.name.c_str());
					ImGui::Text("Songs: %zu", playlist.songs.size());
					ImGui::Text("Total duration: %s", SecondsToTimeHMS(playlist.duration).c_str());

					ImGui::SameLine();
					if (ImGui::Button("Close"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
		ImGui::EndTable();
	}
	ImGui::End();
}
