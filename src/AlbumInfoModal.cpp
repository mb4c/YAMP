#include "AlbumInfoModal.hpp"

#include <utility>
#include "Utils.hpp"

void AlbumInfoModal::Render()
{
	if (m_ShouldOpen)
	{
		ImGui::OpenPopup(m_Name.c_str());
		m_ShouldOpen = false;
	}

	if (ImGui::BeginPopupModal(m_Name.c_str(),  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Name: %s", m_Playlist.name.c_str());
		ImGui::Text("Songs: %zu", m_Playlist.songs.size());
		ImGui::Text("Duration: %s", SecondsToTimeHMS(m_Playlist.duration).c_str());


		if (ImGui::Button("Close"))
		{
			Close();
		}

		ImGui::EndPopup();
	}
}

void AlbumInfoModal::Open(Playlist playlist)
{
	m_Playlist = std::move(playlist);
	Modal::Open();
}
