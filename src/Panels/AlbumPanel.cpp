#include "AlbumPanel.hpp"
#include "Modals.hpp"
#include <imgui_stdlib.h>
#include <Compare.hpp>

void AlbumPanel::RenderPanel(Player& player, std::vector<std::string>& filteredAlbums, const std::vector<Song>& filteredSongs, bool& playlistClicked, bool& shouldFilterTracks, SearchProps& searchProps)
{
	ImGui::Begin("Album");

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2);
	if(ImGui::InputTextWithHint("##artistsearchbar", "Search...", &searchProps.albumSearchbar))
		searchProps.searchAlbums = Search(searchProps.albumSearchbar, filteredAlbums);

    if(ImGui::BeginTable("Album", 1, m_PanelFlag))
    {
        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (searchProps.searchAlbums.size() > 1)
					std::sort(searchProps.searchAlbums.begin(), searchProps.searchAlbums.end(), [&sorts_specs](const std::string& lhs, const std::string& rhs){
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
		ImGui::Selectable("All", "", ImGuiSelectableFlags_SpanAllColumns);
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			player.m_Library.m_SelectedAlbum = "All";
			playlistClicked = false;

			shouldFilterTracks = true;
		}
		ImGui::PopID();

        ImGuiListClipper clipper;
        clipper.Begin(searchProps.searchAlbums.size());
        while (clipper.Step())
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
            {
                std::string album = searchProps.searchAlbums[row_n];
                ImGui::PushID(album.c_str());
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
				bool selected = album == player.m_Library.m_SelectedAlbum;

				ImGui::Selectable(album.c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
					player.m_Library.m_SelectedAlbum = album;
                    playlistClicked = false;

					shouldFilterTracks = true;
                }

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("AlbumPopup");
				}
				bool popup = false;

				if (ImGui::BeginPopup("AlbumPopup"))
				{
					if (ImGui::BeginMenu("Add to playlist"))
					{
						if (ImGui::MenuItem("Create new..."))
						{
							popup = true;
						}

						for (auto &playlist: player.m_Library.m_Playlists)
						{
							if (ImGui::MenuItem(playlist.name.c_str()))
							{
								AddAlbumToPlaylist(player, &playlist, album);
								player.m_Library.SavePlaylists();
							}
						}
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Open in file explorer"))
					{
						OpenInFileExplorer(GetSongsFromAlbumName(searchProps.searchAlbums[row_n], player)[0].path);
					}
					if (ImGui::MenuItem("Info"))
					{
						m_AlbumInfo.m_Playlist = Playlist::GetPlaylistFromAlbumName(searchProps.searchAlbums[row_n], m_Songs);
						m_AlbumInfo.Open(Playlist::GetPlaylistFromAlbumName(searchProps.searchAlbums[row_n], m_Songs));
					}

					ImGui::EndPopup();
				}

				m_AlbumInfo.Render();
				Modals::OpenNewPlaylistModal(row_n, popup, true, player, m_PlaylistName, filteredSongs);

                ImGui::PopID();
            }
        ImGui::EndTable();
    }
	ImGui::End();
}

void AlbumPanel::AddAlbumToPlaylist(Player &player, Playlist *playlist, const std::string &albumName)
{
	std::vector<Song> songs = GetSongsFromAlbumName(albumName, player);

	for (const auto& song : songs)
	{
		playlist->songs.push_back(song);
		playlist->duration += song.duration;
	}
}

std::vector<Song> AlbumPanel::GetSongsFromAlbumName(const std::string &name, Player &player)
{

	std::vector<Song> songs;

	for (auto & song : player.m_Library.m_Songs)
	{
		if (song.album == name)
		{
			songs.push_back(song);
		}
	}

	return songs;
}

void AlbumPanel::Init(bool &shouldFilterTracks, std::vector<std::string>& filteredAlbums, SearchProps& searchProps, const std::vector<Song>& songs)
{
	searchProps.searchAlbums = Search(searchProps.albumSearchbar, filteredAlbums);
	shouldFilterTracks = true;
	m_Songs = songs;
}
