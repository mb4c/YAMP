#include "TracksPanel.hpp"

void TracksPanel::RenderPanel(Player& player, std::vector<Song>& filteredSongs, Playlist& playlist, Playlist& selectedPlaylist, const bool& playlistClicked, bool& shouldFilterTracks)
{
	ImGui::Begin("Tracks");

	if(ImGui::BeginTable("Tracks", 6, m_PanelFlag))
	{
		ImGui::TableSetupColumn("Track", ImGuiTableColumnFlags_DefaultSort, 0.0f, Compare::ColumnID_Track);
		ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Name);
		ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Artist);
		ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Album);
		ImGui::TableSetupColumn("Genre");
		ImGui::TableSetupColumn("Year");
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (filteredSongs.size() > 1)
					std::sort(filteredSongs.begin(), filteredSongs.end(),[sorts_specs](const Song& lhs, const Song& rhs){
						return Compare::CompareSong(lhs, rhs, sorts_specs);
					});

				if (playlist.songs.size() > 1)
					std::sort(playlist.songs.begin(), playlist.songs.end(),[sorts_specs](const Song& lhs, const Song& rhs){
						return Compare::CompareSong(lhs, rhs, sorts_specs);
					});
				sorts_specs->SpecsDirty = false;
			}

		ImGuiListClipper clipper;
		clipper.Begin(filteredSongs.size());
		while (clipper.Step())
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				Song* song = &filteredSongs[row_n];
				ImGui::PushID(song->path.c_str());
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				bool selected = song == &filteredSongs[player.m_CurrentSongIndex];

				ImGui::Selectable(std::to_string(song->track).c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					player.SetPlaylist(playlist, row_n);
					shouldFilterTracks = true;
				}

				if(playlistClicked)
				{
					Reorder(filteredSongs, row_n);
					Reorder(playlist.songs, row_n);
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("TrackPopup");
				}
				bool popup = false;

				if (ImGui::BeginPopup("TrackPopup"))
				{
					if (!playlistClicked)
					{
						if (ImGui::BeginMenu("Add to playlist"))
						{
							if (ImGui::MenuItem("Create new..."))
							{
								popup = true;
							}

							for (auto &playlist2: player.m_Library.m_Playlists)
							{
								if (ImGui::MenuItem(playlist2.name.c_str()))
								{
									Playlist::AddSongToPlaylist(&playlist2, row_n, filteredSongs);

									player.m_Library.SavePlaylists();
								}
							}
							ImGui::EndMenu();
						}
						if (ImGui::MenuItem("Open in file explorer"))
						{
							OpenInFileExplorer(filteredSongs[row_n].path);
						}
						if (ImGui::MenuItem("Select album"))
						{
							player.m_Library.m_SelectedAlbum = filteredSongs[row_n].album;
							shouldFilterTracks = true;
						}
					}
					else
					{
						if(ImGui::Selectable("Remove from playlist"))
						{
							selectedPlaylist.duration -= selectedPlaylist.songs.at(row_n).duration;
							selectedPlaylist.songs.erase(selectedPlaylist.songs.begin() + row_n);
							player.m_Library.SavePlaylists();
							shouldFilterTracks = true;

						}
					}

					ImGui::EndPopup();
				}


				Modals::OpenNewPlaylistModal(row_n, popup, false, player, m_PlaylistName, filteredSongs);

				ImGui::TableNextColumn();
				ImGui::Text("%s", song->title.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", song->artist.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", song->album.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", song->genre.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%d", song->year);
				ImGui::PopID();
			}
		ImGui::EndTable();
	}
	ImGui::End();
}
