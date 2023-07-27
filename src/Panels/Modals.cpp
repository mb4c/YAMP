#include "Modals.hpp"

void Modals::OpenNewPlaylistModal(int index, bool open, bool isAlbum, Player& player, std::string& playlistName, const std::vector<Song>& filteredSongs)
{
	if (open)
	{
		ImGui::OpenPopup("Create playlist");
	}

	if (ImGui::BeginPopupModal("Create playlist",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputTextWithHint("Name", "Name", &playlistName);
		if (ImGui::Button("Create"))
		{
			Playlist playlist;
			playlist.name = playlistName;
			playlistName.clear();

			if (isAlbum)
			{

			} else
			{
				Playlist::AddSongToPlaylist(&playlist, index, filteredSongs);
			}
			player.m_Library.m_Playlists.push_back(playlist);
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
}
